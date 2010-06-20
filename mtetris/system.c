#include "ih/core/tools.h"
#include "ih/mtetris/system.h"

struct ih_mtetris_system_t {
};

ih_mtetris_system_t *ih_mtetris_system_create()
{
  ih_mtetris_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    ih_core_trace("malloc");
  }

  return system;
}

void ih_mtetris_system_destroy(ih_mtetris_system_t *system)
{
  assert(system);
  free(system);
}
