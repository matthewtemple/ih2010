#include "ih/core/tools.h"
#include "ih/stravel/system.h"

struct ih_stravel_system_t {
};

ih_stravel_system_t *ih_stravel_system_create()
{
  ih_stravel_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    ih_core_trace("malloc");
  }

  return system;
}

void ih_stravel_system_destroy(ih_stravel_system_t *system)
{
  assert(system);
  free(system);
}
