#include "ih/core/message.h"
#include "ih/core/short.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

ih_core_bool_t ih_core_short_add_to_message(void *short_object,
    ih_core_message_t *message)
{
  return ih_core_message_add_short(message, short_object);
}

int ih_core_short_compare(void *short_object_a,
    void *short_object_b)
{
  short *short_a;
  short *short_b;
  int compare;

  short_a = short_object_a;
  short_b = short_object_b;

  if (*short_a > *short_b) {
    compare = 1;
  } else if (*short_a < *short_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *ih_core_short_copy(void *short_object)
{
  assert(short_object);
  short *short_value;
  short *short_copy;

  short_value = short_object;
  short_copy = malloc(sizeof *short_copy);
  if (short_copy) {
    *short_copy = *short_value;
  } else {
    ih_core_trace("malloc");
  }

  return short_copy;
}

void *ih_core_short_take_from_message(ih_core_message_t *message)
{
  return ih_core_message_take_short(message);
}

void ih_core_short_destroy(void *short_object)
{
  assert(short_object);
  free(short_object);
}

void ih_core_short_print(void *short_object)
{
  short *short_value;

  short_value = short_object;
  printf("%d", *short_value);
}
