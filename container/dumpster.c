#include "ih/container/dumpster.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

struct ih_container_dumpster_t {
  ih_core_objectey_t *objectey;
  ih_container_list_t *objects;
};

ih_core_bool_t ih_container_dumpster_add(ih_container_dumpster_t *dumpster,
    void *object)
{
  assert(dumpster);
  assert(object);
  ih_core_bool_t success;

  if (ih_container_list_add_last(dumpster->objects, object)) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
  }

  return success;
}

ih_container_dumpster_t *ih_container_dumpster_create
(ih_core_objectey_t *objectey)
{
  assert(objectey);
  ih_container_dumpster_t *dumpster;

  dumpster = malloc(sizeof *dumpster);
  if (dumpster) {
    dumpster->objectey = objectey;
    dumpster->objects = ih_container_list_create(objectey->compare,
        objectey->copy, objectey->destroy);
    if (!dumpster->objects) {
      free(dumpster);
      dumpster = NULL;
    }
  } else {
    ih_core_trace("malloc");
  }

  return dumpster;
}

void ih_container_dumpster_destroy(ih_container_dumpster_t *dumpster)
{
  assert(dumpster);

  ih_container_list_destroy(dumpster->objects);
  free(dumpster);
}

ih_core_bool_t ih_container_dumpster_take_objects_from_list
(ih_container_dumpster_t *dumpster, ih_container_list_t *list)
{
  assert(dumpster);
  assert(list);
  ih_core_bool_t success;
  void *object;

  success = ih_core_bool_true;

  ih_container_list_dont_destroys(list);
  ih_container_list_iterate_start(list);
  while ((object = ih_container_list_iterate_next(list))) {
    if (ih_container_dumpster_add(dumpster, object)) {
      ih_container_list_iterate_remove(list);
    } else {
      success = ih_core_bool_false;
      ih_core_trace("ih_container_dumpster_add_object");
      break;
    }
  }

  return success;
}
