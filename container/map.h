#ifndef ih_container_map_h
#define ih_container_map_h

#include "ih/core/bool.h"
#include "ih/core/object.h"
#include "ih/core/objectey.h"

#define IH_CONTAINER_MAP_DESTROY ih_core_bool_true
#define IH_CONTAINER_MAP_DONT_DESTROY ih_core_bool_false

struct ih_container_map_t;
typedef struct ih_container_map_t ih_container_map_t;

ih_core_bool_t ih_container_map_add(ih_container_map_t *map, void *left,
    void *right);

int ih_container_map_compare(void *map_object_a, void *map_object_b);

void *ih_container_map_copy(void *map_object);

ih_container_map_t *ih_container_map_create(ih_core_objectey_t *left_objectey,
    ih_core_objectey_t *right_objectey, ih_core_bool_t destroys);

void ih_container_map_destroy(void *map_object);

void *ih_container_map_find(ih_container_map_t *map, void *left_object_decoy);

void ih_container_map_print(ih_container_map_t *map);

ih_core_bool_t ih_container_map_remove(ih_container_map_t *map,
    void *left_object_decoy);

#endif
