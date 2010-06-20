#ifndef ih_core_objectey_h
#define ih_core_objectey_h

#include "ih/core/object.h"

struct ih_core_objectey_t {
  ih_core_compare_f compare;
  ih_core_copy_f copy;
  ih_core_destroy_f destroy;
  ih_core_get_as_string_f get_as_string;
};
typedef struct ih_core_objectey_t ih_core_objectey_t;

void ih_core_objectey_init(ih_core_objectey_t *objectey,
    ih_core_compare_f compare, ih_core_copy_f copy,
    ih_core_destroy_f destroy,
    ih_core_get_as_string_f get_as_string);

#endif
