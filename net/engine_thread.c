#include "ih/core/types.h"
#include "ih/external/external.h"
#include "ih/net/engine_thread.h"

ih_net_engine_thread_t *ih_net_engine_create_thread(void *engine_object,
    unsigned short thread_index)
{
  ih_net_engine_thread_t *engine_thread;

  engine_thread = malloc(sizeof *engine_thread);
  if (engine_thread) {
    engine_thread->engine_object = engine_object;
    engine_thread->thread_index = thread_index;
  }

  return engine_thread;
}

void ih_net_engine_destroy_thread(ih_net_engine_thread_t *engine_thread)
{
  assert(engine_thread);
  free(engine_thread);
}
