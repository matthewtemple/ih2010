#ifndef ih_xcache_item_h
#define ih_xcache_item_h

#include "ih/core/object.h"
#include "ih/core/uuid.h"

struct ih_xcache_item_t;
typedef struct ih_xcache_item_t ih_xcache_item_t;

int ih_xcache_item_compare(void *item_object_a, void *item_object_b);

void *ih_xcache_item_copy(void *item_object);

ih_xcache_item_t *ih_xcache_item_create(ih_core_uuid_t *object_uuid,
    void *object, ih_core_copy_f copy,
    ih_core_destroy_f destroy, unsigned long lifespan_seconds);

ih_xcache_item_t *ih_xcache_item_create_decoy(ih_core_uuid_t *object_uuid);

void ih_xcache_item_destroy(void *item_object);

void ih_xcache_item_destroy_decoy(void *item_object);

void ih_xcache_item_expire(ih_xcache_item_t *item);

void *ih_xcache_item_get_object(ih_xcache_item_t *item);

void *ih_xcache_item_get_object_copy(ih_xcache_item_t *item);

ih_core_uuid_t *ih_xcache_item_get_object_uuid(ih_xcache_item_t *item);

ih_core_bool_t ih_xcache_item_is_expired(void *item_object);

#endif
