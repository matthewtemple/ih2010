#include "ih/container/map.h"
#include "ih/container/set.h"
#include "ih/core/pair.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

struct ih_container_map_t {
  ih_container_set_t *pairs;
  ih_core_objectey_t *left_objectey;
  ih_core_objectey_t *right_objectey;
  ih_core_bool_t destroy;
};

ih_core_bool_t ih_container_map_add(ih_container_map_t *map, void *left,
    void *right)
{
  assert(map);
  assert(left);
  assert(right);
  ih_core_pair_t *pair;
  ih_core_bool_t success;

  pair = ih_core_pair_create(left, map->left_objectey, right,
      map->right_objectey, map->destroy);
  if (pair) {
    success = ih_core_bool_true;
    ih_container_set_add(map->pairs, pair);
  } else {
    success = ih_core_bool_false;
    ih_core_trace("ih_core_pair_create");
  }

  return success;
}

int ih_container_map_compare(void *map_object_a, void *map_object_b)
{
  assert(map_object_a);
  assert(map_object_b);
  ih_container_map_t *map_a;
  ih_container_map_t *map_b;

  map_a = map_object_a;
  map_b = map_object_b;

  return ih_container_set_compare(map_a->pairs, map_b->pairs);
}

void *ih_container_map_copy(void *map_object)
{
  assert(map_object);
  ih_container_map_t *map;
  ih_container_map_t *map_copy;

  map = map_object;

  map_copy = ih_container_map_create(map->left_objectey, map->right_objectey,
      map->destroy);
  if (map_copy) {
    map_copy->pairs = ih_container_set_copy(map->pairs);
    if (!map_copy->pairs) {
      ih_core_trace("ih_container_set_copy");
      free(map_copy);
      map_copy = NULL;
    }
  } else {
    ih_core_trace("ih_container_map_create");
  }

  return map_copy;
}

ih_container_map_t *ih_container_map_create(ih_core_objectey_t *left_objectey,
    ih_core_objectey_t *right_objectey, ih_core_bool_t destroy)
{
  assert(left_objectey);
  assert(right_objectey);
  ih_container_map_t *map;

  map = malloc(sizeof *map);
  if (map) {
    map->left_objectey = left_objectey;
    map->right_objectey = right_objectey;
    map->destroy = destroy;
    map->pairs = ih_container_set_create(ih_core_pair_compare_left,
        ih_core_pair_copy, ih_core_pair_destroy);
    if (!map->pairs) {
      ih_core_trace("ih_container_set_create");
      free(map);
      map = NULL;
    }
  } else {
    ih_core_trace("malloc");
  }

  return map;
}

void ih_container_map_destroy(void *map_object)
{
  assert(map_object);
  ih_container_map_t *map;

  map = map_object;

  ih_container_set_destroy(map->pairs);
  free(map);
}

void *ih_container_map_find(ih_container_map_t *map,
    void *left_object_decoy)
{
  assert(map);
  assert(left_object_decoy);
  ih_core_pair_t *found_pair;
  void *found_object;
  ih_core_pair_t *pair_decoy;

  pair_decoy = ih_core_pair_create_decoy(left_object_decoy, map->left_objectey);
  if (pair_decoy) {
    found_pair = ih_container_set_find(map->pairs, pair_decoy);
    ih_core_pair_destroy_decoy(pair_decoy);
    if (found_pair) {
      found_object = ih_core_pair_get_right(found_pair);
    } else {
      found_object = NULL;
    }
  } else {
    found_object = NULL;
    ih_core_trace("pair_create_decoy");
  }

  return found_object;
}

void ih_container_map_print(ih_container_map_t *map)
{
  ih_container_set_print(map->pairs, ih_core_pair_get_as_string);
}

ih_core_bool_t ih_container_map_remove(ih_container_map_t *map,
    void *left_object_decoy)
{
  assert(map);
  assert(left_object_decoy);
  ih_core_bool_t success;
  ih_core_pair_t *pair_decoy;

  pair_decoy = ih_core_pair_create_decoy(left_object_decoy, map->left_objectey);
  if (pair_decoy) {
    success = ih_container_set_remove(map->pairs, pair_decoy);
  } else {
    success = ih_core_bool_false;
    ih_core_trace("pair_create_decoy");
  }

  return success;
}
