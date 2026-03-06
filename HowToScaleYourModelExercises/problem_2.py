import jax
import numpy as np
import jax.sharding as shd
import jax.numpy as jnp
from utils import compile_first, profile, compile_all, compare

# File to implement a collective matmul and compare it with a base implementation
# Of the multiplication A[B_X, D_Y] * W[D, F_Y] -> Out[B_X, F_Y]

jax.distributed.initialize()

RANK = jax.process_index()
WORLD_SIZE = jax.process_count()
IS_MAIN = RANK == 0

def log(*args, **kwargs):
  if IS_MAIN:
    print(*args, **kwargs, flush=True)

def gather_print(sharded_array):
  g_array = np.array(jax.experimental.multihost_utils.process_allgather(sharded_array, tiled=True))
  log(g_array)

mesh = jax.make_mesh(axis_shapes=(16,), axis_names=('X',), axis_types=(shd.AxisType.Auto,))
jax.set_mesh(mesh)

# All numbers divisible by 16 so the sharding goes nicely
E = 16
S = 2048
D = 8192
F = 28672

X = mesh.shape['X']

key = jax.random.PRNGKey(0)
wkey, bkey = jax.random.split(key)

W = jax.random.normal(wkey, (E,D,F), dtype=jnp.float32)
W = jax.device_put(W, jax.P('X', None, None))
A = jnp.arange(np.prod((S,D)), dtype=jnp.float32).reshape((S,D))
A = jax.device_put(A, jax.P('X', None))

B = jax.random.randint(bkey, (S,), minval=0, maxval=E, dtype=jnp.int32)
B = jax.device_put(B, jax.P('X',))

# We compute Out[i] = W[B[i]] @ A[i]
def local_moe(A, B, W):
  out = jnp.zeros((S,F))
  for i in range(E):
    mask = B[:] == jnp.array(i)
    selected = jnp.where(mask[:,None], A, jnp.array(0))
    out += jnp.einsum("sd, df->sf",selected,W[i])
  return out

@jax.shard_map(mesh=mesh, in_specs=jax.P('X',), out_specs=jax.P('X',)) 
def sharded_moe(A, B, W):
  # Set up A as a ragged array
  sort_indices = jnp.argsort(B, axis=0)
  unsort_indices = jnp.argsort(sort_indices, axis=0)
  sorted_A = A[sort_indices] # Shape still (S//X, D)
  # Used to take sorted_A and treat it as a ragged array
  sizes = jnp.sum(jax.nn.one_hot(B, num_classes=E).astype(jnp.int32), axis=0) # Shape (E,)
  offsets = jnp.cumsum(sizes, axis=-1) - sizes

  chunk_size = 128
  # We do a single synchronization here to figure out how many total chunks we need
  # If we instead used a while loop with a chunk.any() check, we'd instead need to
  # synchronize every loop iteration
  max_chunk_size = jax.lax.pmax(sizes.max(), axis_name='X')
  num_chunks = (max_chunk_size + chunk_size - 1) // chunk_size

  def get_next_chunk(chunk_index):
    # Internal to the part of the ragged array for each expert
    internal_offsets = chunk_index*chunk_size + jnp.arange(chunk_size)
    chunk_indices = offsets[:, None] + internal_offsets[None, :] # Shape (E, chunk_size)
    mask = internal_offsets[None, :] < sizes[:, None] # Shape (E, chunk_size)
    return jnp.where(mask[...,None], sorted_A[chunk_indices], 0)

  def loop_body(chunk_index, output):
    chunk = get_next_chunk(chunk_index)
    # Take data from (E, chunk_size_X, D) -> (E_X, chunk_size, D)
    chunk = jax.lax.all_to_all(chunk, axis_name='X', split_axis=0, concat_axis=1, tiled=True)
    output_slice = jnp.einsum("ecd, edf-> ecf", chunk, W)
    output_slice = jax.lax.all_to_all(output_slice, axis_name='X', split_axis=1, concat_axis=0, tiled=True)

    internal_offsets = chunk_index*chunk_size + jnp.arange(chunk_size)
    indices = offsets[:, None] + internal_offsets[None, :] # Shape (E, chunk_size)
    mask = internal_offsets[None, :] < sizes[:, None] # Shape (E, chunk_size)
    # S//X is the index for our dummy row at the end
    indices = jnp.where(mask, indices, S//X)

    output = output.at[indices.reshape(-1)].set(output_slice.reshape(-1,F))
    
    return output

  # We use an extra sentinel row at the end to take care of writes from
  # experts who have no more data to process from this shard (and thus are producing rows of padding 0s)
  output = jax.lax.pcast(jnp.zeros((S // X + 1, F)), 'X', to='varying')
  output = jax.lax.fori_loop(0, num_chunks, loop_body, output)

  return output[:-1][unsort_indices]

VARIANTS = {'local': local_moe, 'shard': sharded_moe }

compiled_functions = compile_all(VARIANTS, A, B, W)
outputs = compare(compiled_functions, A, B, W)

"""
compiled_function = compile_first(VARIANTS, A, B, W)

log(compiled_function.cost_analysis())
for p in (A,B,W):
  log(jax.typeof(p))

out = profile(compiled_function, A, B, W)

log(jax.typeof(out))
"""
#log(compiled_function.as_text())
