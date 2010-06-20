#ifndef ih_container_dumpster_h
#define ih_container_dumpster_h

#include "ih/container/array.h"
#include "ih/container/list.h"
#include "ih/container/set.h"
#include "ih/core/objectey.h"

struct ih_container_dumpster_t;
typedef struct ih_container_dumpster_t ih_container_dumpster_t;

ih_core_bool_t ih_container_dumpster_add(ih_container_dumpster_t *dumpster,
    void *object);

ih_container_dumpster_t *ih_container_dumpster_create
(ih_core_objectey_t *objectey);

void ih_container_dumpster_destroy(ih_container_dumpster_t *dumpster);

ih_core_bool_t ih_container_dumpster_take_objects_from_list
(ih_container_dumpster_t *dumpster, ih_container_list_t *list);

#endif
