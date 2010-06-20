#ifndef ih_core_actorey_h
#define ih_core_actorey_h

#include "ih/core/bitarray.h"

typedef void *(*ih_core_actorey_create_f)(void *search_system,
    ih_core_bitarray_t *solution);

typedef void *(*ih_core_actorey_create_random_f)(void *search_system);

typedef ih_core_bitarray_t *(*ih_core_actorey_get_solution_f)(void *actor);

struct ih_core_actorey_t {
  ih_core_actorey_create_f create;
  ih_core_actorey_create_random_f create_random;
  ih_core_compare_f compare;
  ih_core_copy_f copy;
  ih_core_destroy_f destroy;
  ih_core_get_object_f get_box_cell;
  ih_core_set_object_f set_box_cell;
};
typedef struct ih_core_actorey_t ih_core_actorey_t;

void ih_core_actorey_free(ih_core_actorey_t *actorey);

void ih_core_actorey_init(ih_core_actorey_t *actorey,
    ih_core_actorey_create_f create,
    ih_core_actorey_create_random_f create_random,
    ih_core_compare_f compare, ih_core_copy_f copy, ih_core_destroy_f destroy,
    ih_core_get_object_f get_box_cell, ih_core_set_object_f set_box_cell);

#endif
