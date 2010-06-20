#include "ih/core/tools.h"
#include "ih/mthread/system.h"

struct ih_mthread_system_t {
};

ih_mthread_system_t *ih_mthread_system_create()
{
  ih_mthread_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    ih_core_trace("malloc");
  }

  return system;
}

void ih_mthread_system_destroy(ih_mthread_system_t *system)
{
  assert(system);
  free(system);
}
