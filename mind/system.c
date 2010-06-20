#include "ih/core/tools.h"
#include "ih/mind/system.h"

struct ih_mind_system_t {
};

ih_mind_system_t *ih_mind_system_create()
{
  ih_mind_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    ih_core_trace("malloc");
  }

  return system;
}

void ih_mind_system_destroy(ih_mind_system_t *system)
{
  assert(system);
  free(system);
}
