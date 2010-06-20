#include "ih/core/actorey.h"
#include "ih/external/external.h"

void ih_core_actorey_free(ih_core_actorey_t *actorey) {}

void ih_core_actorey_init(ih_core_actorey_t *actorey,
    ih_core_actorey_create_f create,
    ih_core_actorey_create_random_f create_random,
    ih_core_compare_f compare, ih_core_copy_f copy, ih_core_destroy_f destroy,
    ih_core_get_object_f get_box_cell, ih_core_set_object_f set_box_cell)
{
  assert(actorey);

  actorey->create = create;
  actorey->create_random = create_random;
  actorey->compare = compare;
  actorey->copy = copy;
  actorey->destroy = destroy;
  actorey->get_box_cell = get_box_cell;
  actorey->set_box_cell = set_box_cell;
}
