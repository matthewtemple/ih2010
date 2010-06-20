#ifndef ih_container_shardset_h
#define ih_container_shardset_h

#include "ih/core/object.h"
#include "ih/core/types.h"

#define IH_CONTAINER_SHARDSET_MAX_SHARDS 32768

struct ih_container_shardset_t;
typedef struct ih_container_shardset_t ih_container_shardset_t;

ih_core_bool_t ih_container_shardset_add(ih_container_shardset_t *shardset,
    void *object);

ih_core_bool_t ih_container_shardset_add_replace
(ih_container_shardset_t *shardset, void *object);

void ih_container_shardset_clear(ih_container_shardset_t *shardset);

ih_container_shardset_t *ih_container_shardset_create(ih_core_compare_f compare,
    ih_core_copy_f copy, ih_core_destroy_f destroy, ih_core_hasih_f hasih_object,
    unsigned short shard_count);

void ih_container_shardset_destroy(ih_container_shardset_t *shardset);

void *ih_container_shardset_find(ih_container_shardset_t *shardset,
    void *object);

void *ih_container_shardset_find_copy(ih_container_shardset_t *shardset,
    void *object);

unsigned long ih_container_shardset_get_size(ih_container_shardset_t *shardset);

void *ih_container_shardset_iterate_next(ih_container_shardset_t *shardset);

void ih_container_shardset_lock(ih_container_shardset_t *shardset);

void ih_container_shardset_iterate_remove(ih_container_shardset_t *shardset);

void ih_container_shardset_iterate_start(ih_container_shardset_t *shardset);

void ih_container_shardset_print(ih_container_shardset_t *shardset,
    ih_core_get_as_string_f get_object_as_string);

ih_core_bool_t ih_container_shardset_remove(ih_container_shardset_t *shardset,
    void *object);

unsigned long ih_container_shardset_remove_if(ih_container_shardset_t *shardset,
    ih_core_condition_f object_condition);

void ih_container_shardset_unlock(ih_container_shardset_t *shardset);

#endif
