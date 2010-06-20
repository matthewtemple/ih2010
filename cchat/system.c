#include "ih/cchat/system.h"
#include "ih/core/tools.h"

struct ih_cchat_system_t {
};

ih_cchat_system_t *ih_cchat_system_create()
{
  ih_cchat_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    ih_core_trace("malloc");
  }

  return system;
}

void ih_cchat_system_destroy(ih_cchat_system_t *system)
{
  assert(system);
  free(system);
}
