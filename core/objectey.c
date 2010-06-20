#include "ih/core/objectey.h"
#include "ih/external/external.h"

void ih_core_objectey_init(ih_core_objectey_t *objectey,
    ih_core_compare_f compare, ih_core_copy_f copy,
    ih_core_destroy_f destroy,
    ih_core_get_as_string_f get_as_string)
{
  assert(objectey);

  objectey->compare = compare;
  objectey->copy = copy;
  objectey->destroy = destroy;
  objectey->get_as_string = get_as_string;
}
