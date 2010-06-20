#include "ih/core/long.h"
#include "ih/core/message.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

ih_core_bool_t ih_core_long_add_to_message(void *long_object,
    ih_core_message_t *message)
{
  return ih_core_message_add_long(message, long_object);
}

int ih_core_long_compare(void *long_object_a,
    void *long_object_b)
{
  long *long_a;
  long *long_b;
  int compare;

  long_a = long_object_a;
  long_b = long_object_b;

  if (*long_a > *long_b) {
    compare = 1;
  } else if (*long_a < *long_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *ih_core_long_copy(void *long_object)
{
  assert(long_object);
  long *long_value;
  long *long_copy;

  long_value = long_object;
  long_copy = malloc(sizeof *long_copy);
  if (long_copy) {
    *long_copy = *long_value;
  } else {
    ih_core_trace("malloc");
  }

  return long_copy;
}

void *ih_core_long_create_from_message(ih_core_message_t *message)
{
  return ih_core_message_take_long(message);
}

void ih_core_long_destroy(void *long_object)
{
  assert(long_object);
  free(long_object);
}

char *ih_core_long_get_as_string(void *long_object)
{
  assert(long_object);
  long *long_value;
  char *string;

  long_value = long_object;

  if (-1 == asprintf(&string, "%li", *long_value)) {
    string = NULL;
  }

  return string;
}

void ih_core_long_init_objectey(ih_core_objectey_t *objectey)
{
  assert(objectey);

  objectey->get_as_string = ih_core_long_get_as_string;
  objectey->compare = ih_core_long_compare;
  objectey->copy = ih_core_long_copy;
  objectey->destroy = ih_core_long_destroy;
}

void ih_core_long_print(void *long_object)
{
  ih_core_print(long_object, ih_core_long_get_as_string);
}
