#ifndef ih_memory_system_h
#define ih_memory_system_h

#include "ih/container/array.h"
#include "ih/core/bool.h"
#include "ih/external/external.h"

struct ih_memory_system_t;
typedef struct ih_memory_system_t ih_memory_system_t;

ih_core_bool_t ih_memory_system_add(ih_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary);

ih_memory_system_t *ih_memory_system_create(unsigned long size,
    unsigned short density);

void ih_memory_system_destroy(ih_memory_system_t *system);

ih_core_bool_t ih_memory_system_find(ih_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary);

ih_container_array_t *ih_memory_system_find_cloud(ih_memory_system_t *system,
    unsigned long base, unsigned long cloud_size_max);

ih_core_bool_t ih_memory_system_remove(ih_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary);

#endif
