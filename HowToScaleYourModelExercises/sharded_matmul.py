import jax
import jax.sharding as shd
import jax.numpy as jnp
from utils import compile_first, profile

# File to play around with basic sharded matmuls in jax
# Uses code from the first part of https://jax-ml.github.io/scaling-book/jax-stuff/

jax.distributed.initialize()

RANK = jax.process_index()
WORLD_SIZE = jax.process_count()
IS_MAIN = RANK == 0

def log(*args, **kwargs):
  if IS_MAIN:
    print(*args, **kwargs, flush=True)

mesh = jax.make_mesh(axis_shapes=(8, 2), axis_names=('X', 'Y'), axis_types=(shd.AxisType.Explicit, shd.AxisType.Explicit))
jax.set_mesh(mesh)
In = jnp.zeros((8192, 2048), dtype=jnp.bfloat16, device=jax.NamedSharding(mesh, jax.P('X', 'Y')))
W = jnp.zeros((2048, 8192), dtype=jnp.bfloat16, device=jax.NamedSharding(mesh, jax.P('Y', None)))

def matmul_all_reduce(In, W):
  return jnp.einsum('bd,df->bf', jnp.square(In), W, out_sharding=jax.P('X', None))

def matmul_scatter_reduce(In, W):
   return jnp.einsum('bd,df->bf', jnp.square(In), W, out_sharding=jax.P('X', 'Y'))

VARIANTS = {0:matmul_all_reduce, 1: matmul_scatter_reduce}

jit_matmul = compile_first(VARIANTS, In, W)

log(jit_matmul.cost_analysis())
log(jax.typeof(In))
log(jax.typeof(W))

out = profile(jit_matmul, In, W)

log(jax.typeof(out))
# log(jit_matmul.as_text())
