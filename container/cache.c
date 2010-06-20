#include "ih/container/cache.h"
#include "ih/container/set.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

struct ih_container_cache_t {
  ih_container_set_t *objects;
  ih_core_qutex_t *objects_qutex;
  ih_core_condition_f remove_condition;
};

ih_core_bool_t ih_container_cache_add(ih_container_cache_t *cache,
    void *object)
{
  assert(cache);
  assert(object);
  ih_core_bool_t success;

  ih_core_qutex_lock_exclusive(cache->objects_qutex);
  success = ih_container_set_add(cache->objects, object);
  ih_core_qutex_unlock_exclusive(cache->objects_qutex);

  if (!success) {
    ih_core_trace("ih_container_set_add");
  }

  return success;
}

void ih_container_cache_clear(ih_container_cache_t *cache)
{
  ih_core_qutex_lock_exclusive(cache->objects_qutex);
  ih_container_set_clear(cache->objects);
  ih_core_qutex_unlock_exclusive(cache->objects_qutex);
}

ih_container_cache_t *ih_container_cache_create
(ih_core_compare_f compare, ih_core_copy_f copy,
    ih_core_destroy_f destroy,
    ih_core_condition_f remove_condition)
{
  ih_container_cache_t *cache;
  ih_core_bool_t so_far_so_good;

  cache = malloc(sizeof *cache);
  if (cache) {
    cache->remove_condition = remove_condition;
    cache->objects_qutex = NULL;
    cache->objects = ih_container_set_create(compare, copy,
        destroy);
    if (cache->objects) {
      so_far_so_good = ih_core_bool_true;
    } else {
      so_far_so_good = ih_core_bool_false;
      ih_core_trace("ih_container_set_create");
    }
  } else {
    so_far_so_good = ih_core_bool_false;
    ih_core_trace("malloc() failed");
  }

  if (so_far_so_good) {
    cache->objects_qutex = ih_core_qutex_create();
    if (!cache->objects_qutex) {
      so_far_so_good = ih_core_bool_false;
      ih_core_trace("ih_core_qutex_create");
    }
  }

  if (!so_far_so_good) {
    if (cache->objects) {
      ih_container_set_destroy(cache->objects);
    }
    if (cache->objects_qutex) {
      ih_core_qutex_destroy(cache->objects_qutex);
    }
    free(cache);
    cache = NULL;
  }

  return cache;
}

void ih_container_cache_destroy(ih_container_cache_t *cache)
{
  assert(cache);
  ih_container_set_destroy(cache->objects);
  ih_core_qutex_destroy(cache->objects_qutex);
}

void *ih_container_cache_find(ih_container_cache_t *cache,
    void *object)
{
  assert(cache);
  assert(object);
  void *found_object;

  ih_core_qutex_lock_shared(cache->objects_qutex);
  found_object = ih_container_set_find(cache->objects, object);
  ih_core_qutex_unlock_shared(cache->objects_qutex);

  return found_object;
}

void *ih_container_cache_find_copy(ih_container_cache_t *cache,
    void *object)
{
  assert(cache);
  assert(object);
  void *object_copy;

  ih_core_qutex_lock_shared(cache->objects_qutex);
  object_copy = ih_container_set_find_copy(cache->objects, object);
  ih_core_qutex_unlock_shared(cache->objects_qutex);

  return object_copy;
}

unsigned long ih_container_cache_get_size(ih_container_cache_t *cache)
{
  assert(cache);
  unsigned long size;

  ih_core_qutex_lock_shared(cache->objects_qutex);
  size = ih_container_set_get_size(cache->objects);
  ih_core_qutex_unlock_shared(cache->objects_qutex);

  return size;
}

void ih_container_cache_remove_objects(ih_container_cache_t *cache)
{
  assert(cache);
  void *object;

  ih_core_qutex_lock_shared(cache->objects_qutex);
  {
    ih_container_set_iterate_start(cache->objects);
    while ((object = ih_container_set_iterate_next(cache->objects))) {
      if (cache->remove_condition(object)) {
        ih_container_set_iterate_remove(cache->objects);
      }
    }
  }
  ih_core_qutex_unlock_shared(cache->objects_qutex);
}
