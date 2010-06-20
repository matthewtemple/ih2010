#ifndef ih_core_objects_h
#define ih_core_objects_h

#include "ih/core/objectey.h"

struct ih_core_objects_t {
  ih_core_objectey_t long_objectey;
  ih_core_objectey_t pair_objectey;
  ih_core_objectey_t string_objectey;
  ih_core_objectey_t unsigned_long_objectey;
  ih_core_objectey_t void_objectey;
};
typedef struct ih_core_objects_t ih_core_objects_t;

void ih_core_objects_free(ih_core_objects_t *objects);

void ih_core_objects_init(ih_core_objects_t *objects);

#endif
