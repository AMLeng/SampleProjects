import jax
import jax.sharding as shd
import jax.numpy as jnp
import numpy as np
from utils import compile_first, profile

# File to implement a collective matmul and compare it with a base implementation
# Of the multiplication A[B_X, D_Y] * W[D, F_Y] -> Out[B_X, F_Y]

jax.distributed.initialize()

RANK = jax.process_index()
WORLD_SIZE = jax.process_count()
IS_MAIN = RANK == 0

def log(*args, **kwargs):
  if IS_MAIN:
    print(*args, **kwargs, flush=True)

mesh = jax.make_mesh(axis_shapes=(4, 4), axis_names=('X', 'Y'), axis_types=(shd.AxisType.Explicit, shd.AxisType.Explicit))
jax.set_mesh(mesh)

B = 1024
D = 2048
F = 8192

A = jnp.arange(np.prod((B,D))).reshape((B,D))
A = jax.device_put(A, jax.P('X', 'Y'))
W = jnp.arange(np.prod((D,F))).reshape((D,F))
W = jax.device_put(W, jax.P(None, 'Y'))

def matmul_all_gather(In, W):
  return jnp.einsum('bd,df->bf', In, W, out_sharding=jax.P('X', 'Y'))

@jax.shard_map(mesh=mesh, in_specs=(jax.P('X','Y'), jax.P(None,'Y')), out_specs=jax.P('X','Y'))
def matmul_collective(In, W):
  # Setup
  N = mesh.shape['Y']
  shift_perm = [(i, (i+1) %N) for i in range(N)]
  W_shard_length = W.shape[0]//N
  In_shard = In
  assert W_shard_length == In.shape[1]
  yi = jax.lax.axis_index('Y')
  W_shard = jax.lax.dynamic_slice_in_dim(W, yi * W_shard_length, W_shard_length, axis=0)
  result = jnp.einsum('bd,df->bf', In_shard, W_shard)

  # Main Loop
  for _ in range(N-1):
    In_shard = jax.lax.ppermute(In_shard, 'Y', perm=shift_perm)
    yi = (yi+1) % N
    W_shard = jax.lax.dynamic_slice_in_dim(W, yi * W_shard_length, W_shard_length, axis=0)
    result += jnp.einsum('bd,df->bf', In_shard, W_shard)
  return result

@jax.shard_map(mesh=mesh, in_specs=(jax.P('X','Y'), jax.P(None,'Y')), out_specs=jax.P('X','Y'))
def reference_impl_collective_matmul(lhs, rhs):
  # collective_matmul_allgather_lhs_contracting
  # lhs is the looped operand; rhs is the local operand
  axis_size = jax.lax.axis_size('Y')  # axis_size = 4 for this example
  idx = jax.lax.axis_index('Y')

  chunk_size = lhs.shape[1]
  assert rhs.shape[0] % chunk_size == 0

  def f(i, carrys):
    accum, lhs = carrys
    rhs_chunk = jax.lax.dynamic_slice_in_dim(rhs, (idx + i) % axis_size * chunk_size, chunk_size)
    # Matmul for a chunk
    update = lhs @ rhs_chunk
    # Circular shift to the left
    lhs = jax.lax.ppermute(
        lhs,
        axis_name='Y',
        perm=[(j, (j - 1) % axis_size) for j in range(axis_size)]
    )
    return accum + update, lhs

  accum = jnp.zeros((lhs.shape[0], rhs.shape[1]), dtype=lhs.dtype)
  accum = jax.lax.pvary(accum, ('X', 'Y'))
  accum, lhs = jax.lax.fori_loop(0, axis_size - 1, f, (accum, lhs), unroll=True)

  # Compute the last chunk after the final permute to leave lhs in the state we found it
  i = axis_size - 1
  rhs_chunk = jax.lax.dynamic_slice_in_dim(rhs, (idx + i) % axis_size * chunk_size, chunk_size)
  update = lhs @ rhs_chunk
  return accum + update

@jax.shard_map(mesh=mesh, in_specs=(jax.P('X','Y'), jax.P(None,'Y')), out_specs=jax.P('X','Y'))
def matmul_collective_fori_loop_unroll(In, W):
  # Setup
  N = mesh.shape['Y']
  shift_perm = [(i, (i+1) %N) for i in range(N)]
  W_shard_length = W.shape[0]//N
  In_shard = In
  assert W_shard_length == In.shape[1]
  yi = jax.lax.axis_index('Y')
  W_shard = jax.lax.dynamic_slice_in_dim(W, yi * W_shard_length, W_shard_length, axis=0)
  result = jnp.einsum('bd,df->bf', In_shard, W_shard)

  # Main Loop
  def body(i, carry):
    result, In_shard, yi = carry
    In_shard = jax.lax.ppermute(In_shard, 'Y', perm=shift_perm)
    yi = (yi + 1) % N
    W_shard = jax.lax.dynamic_slice_in_dim(W, yi * W_shard_length, W_shard_length, axis=0)
    result += jnp.einsum('bd,df->bf', In_shard, W_shard)
    return result, In_shard, yi

  result, _, _ = jax.lax.fori_loop(0, N - 1, body, (result, In_shard, yi), unroll=True)
  return result

VARIANTS = {0:matmul_all_gather, 1: matmul_collective, 2: reference_impl_collective_matmul, 3: matmul_collective_fori_loop_unroll}

jit_matmul = compile_first(VARIANTS, A, W)

log(jit_matmul.cost_analysis())
log(jax.typeof(A))
log(jax.typeof(W))

out = profile(jit_matmul, A, W)

log(jax.typeof(out))
#log(jit_matmul.as_text())
