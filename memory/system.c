#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/memory/system.h"

struct ih_memory_system_t {
};

ih_core_bool_t ih_memory_system_add(ih_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary)
{
  return ih_core_bool_false;
}

ih_memory_system_t *ih_memory_system_create(unsigned long node_count,
    unsigned short link_density)
{
  ih_memory_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    ih_core_trace("malloc");
  }

  return system;
}

void ih_memory_system_destroy(ih_memory_system_t *system)
{
  assert(system);
  free(system);
}

ih_core_bool_t ih_memory_system_find(ih_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary)
{
  return ih_core_bool_false;
}

ih_container_array_t *ih_memory_system_find_cloud(ih_memory_system_t *system,
    unsigned long base, unsigned long cloud_size_max)
{
  return NULL;
}

ih_core_bool_t ih_memory_system_remove(ih_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary)
{
  return ih_core_bool_false;
}
