#include "ih/core/tools.h"
#include "ih/mnet/system.h"

struct ih_mnet_system_t {
};

ih_mnet_system_t *ih_mnet_system_create()
{
  ih_mnet_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    ih_core_trace("malloc");
  }

  return system;
}

void ih_mnet_system_destroy(ih_mnet_system_t *system)
{
  assert(system);
  free(system);
}
