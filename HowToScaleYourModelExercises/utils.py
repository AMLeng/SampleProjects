import argparse
import time
import numpy as np
import jax

def _parse_cli_args(variants):
    parser = argparse.ArgumentParser()
    parser.add_argument('variant', type=str, nargs='+',
                        help='Which variant(s) to run (names or numbers)')
    parser.add_argument('--trace', action='store_true',
                        help='Save a trace instead of benchmarking')
    parser.add_argument('--runs', type=int, default=5,
                        help='Number of timed runs (default: 5)')
    parser.add_argument('--warmup', type=int, default=3,
                        help='Number of warmup runs (default: 3)')
    parser.add_argument('--trace-dir', type=str, default='/tmp/jax-trace',
                        help='Directory for trace output (default: /tmp/jax-trace)')
    args = parser.parse_args()

    keys = []
    for v in args.variant:
        try:
            key = int(v)
        except ValueError:
            key = v
        if key not in variants:
            parser.error(f"Unknown variant '{v}'. Choose from: {list(variants.keys())}")
        keys.append(key)

    args.variants = keys
    return args

_cli_args = None

def _init_args(variants):
    global _cli_args
    _cli_args = _parse_cli_args(variants)

def compile_first(variants, *args, **kwargs):
    """Parses args and compiles only the first selected variant. Returns compiled function.

    Usage:
        compiled = compile_first({0: matmul_xy, 1: matmul_xn}, In, W)
        log(compiled.cost_analysis())
        log(compiled.as_text())
        out = profile(compiled, In, W)
        log(jax.typeof(out))
    """
    _init_args(variants)
    key = _cli_args.variants[0]
    fn = variants[key]
    compiled = jax.jit(fn).lower(*args, **kwargs).compile()
    return compiled

def compile_all(variants, *args, **kwargs):
    """Parses args and compiles all selected variants. Returns dict of {key: compiled}.

    Usage:
        compiled = compile_all({0: matmul_xy, 1: matmul_xn, 2: matmul_fast}, In, W)
        for key, c in compiled.items():
            log(c.cost_analysis())
            out = profile(c, In, W)
    """
    _init_args(variants)
    compiled = {}
    for key in _cli_args.variants:
        fn = variants[key]
        compiled[key] = jax.jit(fn).lower(*args, **kwargs).compile()
    return compiled

def _gather(x):
    """Fully gather a sharded array to a local numpy array."""
    return np.array(jax.experimental.multihost_utils.process_allgather(x, tiled=True))

def _sharding_str(x):
    """Human-readable sharding description."""
    if hasattr(x, 'sharding'):
        return str(x.sharding)
    return 'unsharded'

def _compare_leaves(ref, out, path="output"):
    """Compare two pytree leaves. Returns list of error strings (empty = match)."""
    errors = []

    # Type check
    if type(ref) != type(out):
        errors.append(f"  {path}: type mismatch: {type(ref).__name__} vs {type(out).__name__}")
        return errors

    if not isinstance(ref, jax.Array):
        # For non-array leaves, just check equality
        if ref != out:
            errors.append(f"  {path}: value mismatch: {ref} vs {out}")
        return errors

    # Shape check
    if ref.shape != out.shape:
        errors.append(f"  {path}: shape mismatch: {ref.shape} vs {out.shape}")
        return errors

    # Dtype check
    if ref.dtype != out.dtype:
        errors.append(f"  {path}: dtype mismatch: {ref.dtype} vs {out.dtype}")
        return errors

    # Sharding check
    ref_sharding = _sharding_str(ref)
    out_sharding = _sharding_str(out)
    if ref_sharding != out_sharding:
        errors.append(f"  {path}: sharding mismatch:\n"
                      f"    reference: {ref_sharding}\n"
                      f"    got:       {out_sharding}")

    # Value check — gather to host and compare
    ref_np = _gather(ref)
    out_np = _gather(out)

    if not np.array_equal(ref_np, out_np):
        if np.issubdtype(ref_np.dtype, np.floating) or np.issubdtype(ref_np.dtype, np.complexfloating):
            close = np.isclose(ref_np, out_np, rtol=1e-5, atol=1e-5)
            n_mismatch = int(np.sum(~close))
            n_total = ref_np.size
            max_abs = float(np.max(np.abs(ref_np - out_np)))
            max_rel = float(np.max(np.abs(ref_np - out_np) / np.maximum(np.abs(ref_np), 1e-12)))
            if np.allclose(ref_np, out_np, rtol=1e-3, atol=1e-3):
                errors.append(
                    f"  {path}: values CLOSE but not exact — "
                    f"{n_mismatch}/{n_total} elements differ "
                    f"(max abs err: {max_abs:.2e}, max rel err: {max_rel:.2e})")
            else:
                errors.append(
                    f"  {path}: values DIFFER — "
                    f"{n_mismatch}/{n_total} elements differ "
                    f"(max abs err: {max_abs:.2e}, max rel err: {max_rel:.2e})")
        else:
            mismatch_mask = ref_np != out_np
            n_mismatch = int(np.sum(mismatch_mask))
            n_total = ref_np.size
            errors.append(
                f"  {path}: values DIFFER — "
                f"{n_mismatch}/{n_total} elements differ (integer/bool dtype)")

    return errors

def compare(compiled_dict, *args, **kwargs):
    """Run all compiled variants and compare outputs against the first (reference).

    Args:
        compiled_dict: dict from compile_all, or any {key: compiled_fn} dict
        *args, **kwargs: inputs to pass to each compiled function

    Returns:
        dict of {key: output} for all variants

    Usage:
        compiled = compile_all({0: ref_impl, 1: fast_impl, 2: experimental}, In, W)
        outputs = compare(compiled, In, W)
    """
    keys = list(compiled_dict.keys())
    if len(keys) < 2:
        raise ValueError("compare requires at least 2 variants")

    ref_key = keys[0]
    is_root = jax.process_index() == 0

    # Run all variants
    outputs = {}
    for key in keys:
        outputs[key] = compiled_dict[key](*args, **kwargs)
        outputs[key].block_until_ready() if isinstance(outputs[key], jax.Array) else jax.block_until_ready(outputs[key])

    ref_out = outputs[ref_key]
    ref_leaves, ref_treedef = jax.tree.flatten(ref_out)

    all_ok = True
    for key in keys[1:]:
        out = outputs[key]
        out_leaves, out_treedef = jax.tree.flatten(out)

        errors = []

        # Structure check
        if ref_treedef != out_treedef:
            errors.append(f"  output structure mismatch:\n"
                          f"    reference ({ref_key}): {ref_treedef}\n"
                          f"    variant   ({key}):     {out_treedef}")
        else:
            # Compare leaf-by-leaf
            for i, (r, o) in enumerate(zip(ref_leaves, out_leaves)):
                path = f"leaf[{i}]" if len(ref_leaves) > 1 else "output"
                errors.extend(_compare_leaves(r, o, path))

        if is_root:
            if errors:
                all_ok = False
                print(f"MISMATCH: variant '{key}' vs reference '{ref_key}':")
                for e in errors:
                    print(e)
            else:
                print(f"OK: variant '{key}' matches reference '{ref_key}'")

    if is_root and all_ok:
        print("All variants match.")

    return outputs

def profile(compiled, *args, **kwargs):
    """Benchmark or profile the compiled function. Returns last output."""
    cli_args = _cli_args

    for _ in range(cli_args.warmup):
        compiled(*args, **kwargs).block_until_ready()

    if cli_args.trace:
        jax.block_until_ready(args)
        jax.profiler.start_trace(cli_args.trace_dir)
        for _ in range(cli_args.runs):
            out = compiled(*args, **kwargs)
            out.block_until_ready()
        jax.profiler.stop_trace()
        if jax.process_index() == 0:
            print(f"Trace saved to {cli_args.trace_dir}")
    else:
        jax.block_until_ready(args)
        times = []
        for _ in range(cli_args.runs):
            start = time.perf_counter()
            out = compiled(*args, **kwargs)
            out.block_until_ready()
            times.append(time.perf_counter() - start)
        if jax.process_index() == 0:
            times = np.array(times)
            print(f"variant {cli_args.variants[0]}: "
                  f"{times.mean()*1000:.2f}ms ± {times.std()*1000:.2f}ms "
                  f"(min: {times.min()*1000:.2f}ms)")

    return out
