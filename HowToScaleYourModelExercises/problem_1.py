import jax
import numpy as np
import jax.sharding as shd
import jax.numpy as jnp
from utils import compile_first, profile
from jax.experimental.multihost_utils import process_allgather

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

S = 1024
D = 1024

A = jnp.arange(np.prod((S,D)), dtype=jnp.float32).reshape((S,D))
A = jax.device_put(A, jax.P('X', 'Y'))

def shard_average_jit(A):
  X = mesh.shape['X']
  Y = mesh.shape['Y']
  B = A.reshape(( X, A.shape[0]//X,Y,A.shape[1]//Y,), out_sharding=jax.P('X',None,'Y',None))
  return jnp.mean(B, axis=(1,3))

@jax.shard_map(mesh=mesh, in_specs=jax.P('X','Y'), out_specs=jax.P('X','Y')) 
def shard_average_shard_map(A):
  return jnp.mean(A, keepdims=True)

@jax.shard_map(mesh=mesh, in_specs=jax.P('X','Y'), out_specs=jax.P('X','Y')) 
def roll_shard_map(A, shift=1):
  return jnp.roll(A, shift, axis=0) - A

VARIANTS = {'1_jit': shard_average_jit, '1_sm': shard_average_shard_map, 2: roll_shard_map}

compiled_function = compile_first(VARIANTS, A)
log(compiled_function.cost_analysis())

log(jax.typeof(A))

out = profile(compiled_function, A)
log(jax.typeof(out))
log(compiled_function.as_text())

"""
out_1 = shard_average_jit(A)
out_2 = shard_average_shard_map(A)
log(jax.typeof(out_1))
log(jax.typeof(out_2))
np.testing.assert_array_equal(process_allgather(out_1, tiled=True), process_allgather(out_2, tiled=True))
"""
