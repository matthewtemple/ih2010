#include "ih/container/set.h"
#include "ih/container/shardset.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

struct ih_container_shardset_t {
  ih_core_hasih_f hasih_object;
  unsigned short shard_count;
  ih_container_set_t *shards[IH_CONTAINER_SHARDSET_MAX_SHARDS];
  pthread_mutex_t shard_mutexes[IH_CONTAINER_SHARDSET_MAX_SHARDS];

  ih_core_bool_t iterate_remove;
  ih_core_bool_t iterate_first;
  unsigned short iterator_shard;
  void *iterator;

  pthread_mutex_t mutex;
};

static void ih_container_shardset_create_rollback
(ih_container_shardset_t *shardset);

static unsigned short get_shard_id_for_object(ih_container_shardset_t *shardset,
    void *object);

ih_core_bool_t ih_container_shardset_add(ih_container_shardset_t *shardset,
    void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  ih_core_bool_t success;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  success = ih_container_set_add(*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return success;
}

ih_core_bool_t ih_container_shardset_add_replace
(ih_container_shardset_t *shardset, void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  ih_core_bool_t success;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  success
    = ih_container_set_add_replace(*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return success;
}

void ih_container_shardset_clear(ih_container_shardset_t *shardset)
{
  assert(shardset);
  unsigned short each_shard;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {
    pthread_mutex_lock(shardset->shard_mutexes + each_shard);
    ih_container_set_clear(*(shardset->shards + each_shard));
    pthread_mutex_unlock(shardset->shard_mutexes + each_shard);
  }
}

/*
  TODO: simplify
  TODO: ugh...some rollback code is in the rollback method, some not...fix
*/
ih_container_shardset_t *ih_container_shardset_create
(ih_core_compare_f compare, ih_core_copy_f copy,
    ih_core_destroy_f destroy, ih_core_hasih_f hasih_object,
    unsigned short shard_count)
{
  assert(shard_count > 0);
  ih_container_shardset_t *shardset;
  ih_core_bool_t so_far_so_good;
  unsigned short each_shard;
  ih_core_bool_t mutex_needs_destroy;

  mutex_needs_destroy = ih_core_bool_false;

  if (shard_count > IH_CONTAINER_SHARDSET_MAX_SHARDS) {
    shard_count = IH_CONTAINER_SHARDSET_MAX_SHARDS;
  }

  shardset = malloc(sizeof *shardset);
  if (shardset) {
    shardset->shard_count = shard_count;
    shardset->hasih_object = hasih_object;
    for (each_shard = 0; each_shard < shard_count; each_shard++) {
      *(shardset->shards + each_shard) = NULL;
    }
    so_far_so_good = ih_core_bool_true;
  } else {
    ih_core_trace("malloc");
    so_far_so_good = ih_core_bool_false;
  }

  if (so_far_so_good) {
    for (each_shard = 0; each_shard < shard_count; each_shard++) {
      *(shardset->shards + each_shard) = ih_container_set_create
        (compare, copy, destroy);
      if (!*(shardset->shards + each_shard)) {
        ih_core_trace("ih_container_set_create");
        so_far_so_good = ih_core_bool_false;
        break;
      }
    }
  }

  if (so_far_so_good) {
    for (each_shard = 0; each_shard < shard_count; each_shard++) {
      if (0 != pthread_mutex_init
          (shardset->shard_mutexes + each_shard, NULL)) {
        ih_core_trace("pthread_mutex_init");
        so_far_so_good = ih_core_bool_false;
        break;
      }
    }
  }

  if (so_far_so_good) {
    if (0 == pthread_mutex_init(&shardset->mutex, NULL)) {
      mutex_needs_destroy = ih_core_bool_true;
    } else {
      ih_core_trace("pthread_mutex_init");
    }
  }

  if (!so_far_so_good && shardset) {
    ih_container_shardset_create_rollback(shardset);
    if (mutex_needs_destroy) {
      if (!pthread_mutex_destroy(&shardset->mutex)) {
        ih_core_trace("pthread_mutex_destroy");
      }
    }
    free(shardset);
    shardset = NULL;
  }

  return shardset;
}

void ih_container_shardset_create_rollback(ih_container_shardset_t *shardset)
{
  assert(shardset);
  unsigned short each_shard;

  if (shardset->shards) {
    for (each_shard = 0; each_shard < shardset->shard_count;
         each_shard++) {
      if (*(shardset->shards + each_shard)) {
        ih_container_set_destroy(*(shardset->shards + each_shard));
      }
    }
    free(shardset->shards);
  }

  if (shardset->shard_mutexes) {
    for (each_shard = 0; each_shard < shardset->shard_count;
         each_shard++) {
      if (0 != pthread_mutex_destroy
          (shardset->shard_mutexes + each_shard)) {
        ih_core_trace("pthread_mutex_destroy");
      }
    }
  }
}

void ih_container_shardset_destroy(ih_container_shardset_t *shardset)
{
  assert(shardset);
  unsigned short each_shard;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {
    ih_container_set_destroy(*(shardset->shards + each_shard));
    if (0 != pthread_mutex_destroy(shardset->shard_mutexes + each_shard)) {
      ih_core_trace("pthread_mutex_destroy");
    }
  }

  if (0 != pthread_mutex_destroy(&shardset->mutex)) {
    ih_core_trace("pthread_mutex_destroy");
  }

  free(shardset);
}

void *ih_container_shardset_find(ih_container_shardset_t *shardset,
    void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  void *found_object;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  found_object = ih_container_set_find(*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return found_object;
}

void *ih_container_shardset_find_copy
(ih_container_shardset_t *shardset, void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  void *found_object;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  found_object = ih_container_set_find_copy
    (*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return found_object;
}

unsigned long ih_container_shardset_get_size(ih_container_shardset_t *shardset)
{
  assert(shardset);
  unsigned short each_shard;
  unsigned long total_size;

  total_size = 0;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {
    pthread_mutex_lock(shardset->shard_mutexes + each_shard);
    total_size += ih_container_set_get_size(*(shardset->shards + each_shard));
    pthread_mutex_unlock(shardset->shard_mutexes + each_shard);
  }

  return total_size;
}

void *ih_container_shardset_iterate_next
(ih_container_shardset_t *shardset)
{
  assert(shardset);
  ih_container_set_t *set;
  void *next_object;

  if (shardset->iterator) {
    if (shardset->iterate_first) {
      shardset->iterate_first = ih_core_bool_false;
      next_object = shardset->iterator;
    } else {
      set = shardset->shards[shardset->iterator_shard];
      if (shardset->iterate_remove) {
        ih_container_set_iterate_remove(set);
        shardset->iterate_remove = ih_core_bool_false;
      }
      shardset->iterator = ih_container_set_iterate_next(set);
      next_object = shardset->iterator;
    }
  } else {
    next_object = NULL;
  }

  /*
    TODO: there may be a cleaner implementation...this block (almost) is
    repeated in ih_container_shardset_iterate_start().  this implementation is
    sortof a hybrid of the way ih_core_set/list/array do iteration, crossed with
    the changes necessary in this context.  it may be that in this context
    there is an implementation with much less happening in
    ih_container_shardset_iterate_start().
  */
  while (!shardset->iterator) {
    pthread_mutex_unlock(shardset->shard_mutexes + shardset->iterator_shard);
    if (shardset->iterator_shard < (shardset->shard_count - 1)) {
      shardset->iterator_shard++;
      pthread_mutex_lock(shardset->shard_mutexes + shardset->iterator_shard);
      set = shardset->shards[shardset->iterator_shard];
      ih_container_set_iterate_start(set);
      shardset->iterator = ih_container_set_iterate_next(set);
      next_object = shardset->iterator;
    } else {
      break;
    }
  }

  return next_object;
}

void ih_container_shardset_iterate_remove(ih_container_shardset_t *shardset)
{
  shardset->iterate_remove = ih_core_bool_true;
}

void ih_container_shardset_iterate_start(ih_container_shardset_t *shardset)
{
  assert(shardset);
  ih_container_set_t *set;

  shardset->iterator_shard = 0;
  set = shardset->shards[shardset->iterator_shard];

  pthread_mutex_lock(shardset->shard_mutexes + shardset->iterator_shard);
  ih_container_set_iterate_start(set);
  shardset->iterator = ih_container_set_iterate_next(set);
  while (!shardset->iterator) {
    pthread_mutex_unlock(shardset->shard_mutexes + shardset->iterator_shard);
    if (shardset->iterator_shard < (shardset->shard_count - 1)) {
      shardset->iterator_shard++;
      pthread_mutex_lock(shardset->shard_mutexes + shardset->iterator_shard);
      set = shardset->shards[shardset->iterator_shard];
      ih_container_set_iterate_start(set);
      shardset->iterator = ih_container_set_iterate_next(set);
    } else {
      break;
    }
  }

  shardset->iterate_remove = ih_core_bool_false;
  shardset->iterate_first = ih_core_bool_true;
}

void ih_container_shardset_lock(ih_container_shardset_t *shardset)
{
  pthread_mutex_lock(&shardset->mutex);
}

void ih_container_shardset_print(ih_container_shardset_t *shardset,
    ih_core_get_as_string_f get_object_as_string)
{
  assert(shardset);
  unsigned short each_shard;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {
    pthread_mutex_lock(shardset->shard_mutexes + each_shard);
    ih_container_set_print(*(shardset->shards + each_shard),
        get_object_as_string);
    pthread_mutex_unlock(shardset->shard_mutexes + each_shard);
  }
}

ih_core_bool_t ih_container_shardset_remove(ih_container_shardset_t *shardset,
    void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  ih_core_bool_t success;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  success = ih_container_set_remove(*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return success;
}

unsigned long ih_container_shardset_remove_if(ih_container_shardset_t *shardset,
    ih_core_condition_f object_condition)
{
  assert(shardset);
  unsigned short each_shard;
  ih_container_set_t *set;
  void *object;
  unsigned long total_items_removed;

  total_items_removed = 0;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {

    pthread_mutex_lock(shardset->shard_mutexes + each_shard);
    {
      set = *(shardset->shards + each_shard);
      ih_container_set_iterate_start(set);
      while ((object = ih_container_set_iterate_next(set))) {
        if (object_condition(object)) {
          ih_container_set_iterate_remove(set);
          total_items_removed++;
        }
      }
    }
    pthread_mutex_unlock(shardset->shard_mutexes + each_shard);

  }

  return total_items_removed;
}

void ih_container_shardset_unlock(ih_container_shardset_t *shardset)
{
  pthread_mutex_unlock(&shardset->mutex);
}

unsigned short get_shard_id_for_object(ih_container_shardset_t *shardset,
    void *object)
{
  assert(shardset);
  assert(object);
  unsigned long hash;
  unsigned short id;

  hash = shardset->hasih_object(object);
  id = hash % shardset->shard_count;

  return id;
}
