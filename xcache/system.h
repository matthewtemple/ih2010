#ifndef ih_xcache_system_h
#define ih_xcache_system_h

#include "ih/core/object.h"
#include "ih/core/uuid.h"

struct ih_xcache_system_t;
typedef struct ih_xcache_system_t ih_xcache_system_t;

ih_core_bool_t ih_xcache_system_add(ih_xcache_system_t *system,
    ih_core_uuid_t *object_uuid, void *object,
    ih_core_copy_f copy, ih_core_destroy_f destroy,
    unsigned long lifetime_seconds);

void ih_xcache_system_clear(ih_xcache_system_t *system);

ih_xcache_system_t *ih_xcache_system_create();

void ih_xcache_system_destroy(ih_xcache_system_t *system);

void *ih_xcache_system_find_wait_copy_expire(ih_xcache_system_t *system,
    ih_core_uuid_t *object_uuid, unsigned long max_wait_seconds);

unsigned long ih_xcache_system_get_size(ih_xcache_system_t *system);

void ih_xcache_system_remove_objects(ih_xcache_system_t *system);

#endif
