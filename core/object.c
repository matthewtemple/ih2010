#include "ih/core/object.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

void ih_core_print(void *object, ih_core_get_as_string_f get_object_as_string)
{
  assert(object);
  assert(get_object_as_string);
  char *object_string;

  object_string = get_object_as_string(object);
  if (object_string) {
    printf("%s\n", object_string);
    free(object_string);
  } else {
    ih_core_trace("get_as_string");
  }
}
