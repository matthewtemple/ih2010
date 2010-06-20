#include "ih/core/long.h"
#include "ih/core/objects.h"
#include "ih/core/pair.h"
#include "ih/core/string.h"
#include "ih/core/unsigned_long.h"
#include "ih/core/void.h"
#include "ih/external/external.h"

void ih_core_objects_free(ih_core_objects_t *objects) {}

void ih_core_objects_init(ih_core_objects_t *objects)
{
  assert(objects);

  ih_core_long_init_objectey(&objects->long_objectey);
  ih_core_pair_init_objectey(&objects->pair_objectey);
  ih_core_string_init_objectey(&objects->string_objectey);
  ih_core_unsigned_long_init_objectey(&objects->unsigned_long_objectey);
  ih_core_void_init_objectey(&objects->void_objectey);
}
