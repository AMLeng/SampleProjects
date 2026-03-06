import jax
import jax.numpy as jnp
import numpy as np

jax.distributed.initialize()

RANK = jax.process_index()
WORLD_SIZE = jax.process_count()
IS_MAIN = RANK == 0

def log(*args, **kwargs):
  if IS_MAIN:
    print(*args, **kwargs, flush=True)

#############################################
# List all available devices
devices = jax.devices()

log(f"All devices: {devices}")
if IS_MAIN:
  log(f"Memory stats for device 0: {devices[0].memory_stats()}")

# Specifically check for TPUs
tpu_devices = jax.devices('tpu')
log(f"TPU devices: {tpu_devices}")
log(f"Number of TPUs: {len(tpu_devices)}")

# Get the default backend
log(f"Default backend: {jax.default_backend()}")

mesh = jax.make_mesh(axis_shapes=(16,), axis_names=('X',))
jax.set_mesh(mesh)

key = jax.random.PRNGKey(0)
#A = jnp.arange(16, dtype=jnp.float32)
A = jax.random.randint(key, (16,), minval=0, maxval=4, dtype=jnp.int32)
A = jax.device_put(A, jax.P('X',))

log(jnp.array(jax.experimental.multihost_utils.process_allgather(A, tiled=True)))
