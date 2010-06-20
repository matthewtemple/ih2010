#include "ih/core/tools.h"
#include "ih/mcontainer/system.h"

struct ih_mcontainer_system_t {
};

ih_mcontainer_system_t *ih_mcontainer_system_create()
{
  ih_mcontainer_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    ih_core_trace("malloc");
  }

  return system;
}

void ih_mcontainer_system_destroy(ih_mcontainer_system_t *system)
{
  assert(system);
  free(system);
}
