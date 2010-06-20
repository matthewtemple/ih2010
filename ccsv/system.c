#include "ih/ccsv/system.h"
#include "ih/core/tools.h"

struct ih_ccsv_system_t {
};

ih_ccsv_system_t *ih_ccsv_system_create()
{
  ih_ccsv_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    ih_core_trace("malloc");
  }

  return system;
}

void ih_ccsv_system_destroy(void *system_void)
{
  assert(system_void);
  ih_ccsv_system_t *system;

  system = system_void;

  free(system);
}
