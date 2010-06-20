#include "ih/container/array.h"
#include "ih/core/bitarray.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/math/combination.h"

struct ih_matih_combination_t {
  ih_container_set_t *elements_set;
  ih_container_array_t *elements;
  unsigned long element_count;
  ih_core_bitarray_t *flags;

  ih_core_compare_f compare;
  ih_core_copy_f copy;
  ih_core_destroy_f destroy;

  ih_core_bool_t iterate_first;
};

static ih_container_set_t *create_set_based_on_flags
(ih_matih_combination_t *combination);

ih_container_set_t *create_set_based_on_flags(ih_matih_combination_t *combination)
{
  assert(combination);
  ih_container_set_t *set;
  unsigned long each_element;
  void *object;
  void *object_copy;

  set = ih_container_set_create(combination->compare,
      combination->copy, combination->destroy);
  if (set) {
    for (each_element = 0; each_element < combination->element_count;
         each_element++) {
      if (1 == ih_core_bitarray_get_bit(combination->flags, each_element)) {
        object = ih_container_array_find(combination->elements, each_element);
        object_copy = combination->copy(object);
        if (object_copy) {
          if (!ih_container_set_add(set, object_copy)) {
            ih_core_trace("ih_container_set_add");
          }
        } else {
          ih_core_trace("combination->copy");
        }
      }
    }
  } else {
    ih_core_trace("ih_container_set_create");
  }

  return set;
}

ih_matih_combination_t *ih_matih_combination_create(ih_container_set_t *set)
{
  assert(set);
  assert(ih_container_set_get_size(set) > 0);
  ih_matih_combination_t *combination;
  ih_core_bool_t so_far_so_good;

  combination = malloc(sizeof *combination);
  if (combination) {
    combination->flags = NULL;
    combination->iterate_first = ih_core_bool_false;
    combination->element_count = ih_container_set_get_size(set);
    combination->compare = ih_container_set_get_compare_function(set);
    combination->copy = ih_container_set_get_copy_function(set);
    combination->destroy = ih_container_set_get_destroy_function(set);

    combination->elements_set = ih_container_set_copy(set);
    if (combination->elements_set) {
      so_far_so_good = ih_core_bool_true;
    } else {
      so_far_so_good = ih_core_bool_false;
      ih_core_trace("ih_container_set_copy");
    }

  } else {
    so_far_so_good = ih_core_bool_false;

    ih_core_trace("malloc");
  }

  if (so_far_so_good) {
    combination->elements = ih_container_set_get_as_array
      (combination->elements_set);
    if (!combination->elements) {
      so_far_so_good = ih_core_bool_false;
      ih_core_trace("ih_container_array_create_reference_from_set");
    }
  }

  if (so_far_so_good) {
    combination->flags = ih_core_bitarray_create
      (combination->element_count);
    if (!combination->flags) {
      so_far_so_good = ih_core_bool_false;
    }
  }

  if (!so_far_so_good && combination) {
    if (combination->elements_set) {
      ih_container_set_destroy(combination->elements_set);
    }
    if (combination->elements) {
      ih_container_array_destroy(combination->elements);
    }
    if (combination->flags) {
      ih_core_bitarray_destroy(combination->flags);
    }
  }

  return combination;
}

void ih_matih_combination_destroy(ih_matih_combination_t *combination)
{
  assert(combination);

  ih_container_set_destroy(combination->elements_set);
  ih_container_array_destroy(combination->elements);
  ih_core_bitarray_destroy(combination->flags);
  free(combination);
}

ih_container_set_t *ih_matih_combination_iterate_next
(ih_matih_combination_t *combination)
{
  assert(combination);
  ih_container_set_t *set;

  if (combination->iterate_first) {
    set = create_set_based_on_flags(combination);
    if (!set) {
      ih_core_trace("create_set_based_on_flags");
    }
    combination->iterate_first = ih_core_bool_false;
  } else {
    if (ih_core_bitarray_increment(combination->flags)) {
      set = create_set_based_on_flags(combination);
      if (!set) {
        ih_core_trace("create_set_based_on_flags");
      }
    } else {
      set = NULL;
    }
  }

  return set;
}

void ih_matih_combination_iterate_start(ih_matih_combination_t *combination)
{
  assert(combination);

  ih_core_bitarray_unset_all(combination->flags);
  ih_core_bitarray_set_bit(combination->flags, 0, 1);
  combination->iterate_first = ih_core_bool_true;
}
