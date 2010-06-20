#ifndef ih_container_cache_h
#define ih_container_cache_h

#include "ih/core/object.h"
#include "ih/core/qutex.h"

struct ih_container_cache_t;
typedef struct ih_container_cache_t ih_container_cache_t;

ih_core_bool_t ih_container_cache_add(ih_container_cache_t *cache, void *object);

void ih_container_cache_clear(ih_container_cache_t *cache);

ih_container_cache_t *ih_container_cache_create(ih_core_compare_f compare,
    ih_core_copy_f copy, ih_core_destroy_f destroy,
    ih_core_condition_f remove_condition);

void ih_container_cache_destroy(ih_container_cache_t *cache);

void *ih_container_cache_find(ih_container_cache_t *cache, void *object);

void *ih_container_cache_find_copy(ih_container_cache_t *cache, void *object);

unsigned long ih_container_cache_get_size(ih_container_cache_t *cache);

void ih_container_cache_remove_objects(ih_container_cache_t *cache);

#endif
