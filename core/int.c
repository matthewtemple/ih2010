#include "ih/core/int.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

struct ih_core_int_t {};

int ih_core_int_compare(void *int_object_a,
    void *int_object_b)
{
  int *int_a;
  int *int_b;
  int compare;

  int_a = int_object_a;
  int_b = int_object_b;

  if (*int_a > *int_b) {
    compare = 1;
  } else if (*int_a < *int_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *ih_core_int_copy(void *int_object)
{
  assert(int_object);
  int *int_value;
  int *int_copy;

  int_value = int_object;
  int_copy = malloc(sizeof *int_copy);
  if (int_copy) {
    *int_copy = *int_value;
  } else {
    ih_core_trace("malloc");
  }

  return int_copy;
}

ih_core_int_t *ih_core_int_create()
{
  return NULL;
}

void ih_core_int_destroy(void *int_object)
{
  free(int_object);
}

char *ih_core_int_get_as_string(void *int_object)
{
  assert(int_object);
  int *i;
  char *string;

  i = int_object;

  string = malloc(10 + 1);
  if (string) {
    snprintf(string, 10 + 1, "%i", *i);
  } else {
    ih_core_trace_exit("malloc");
  }

  return string;
}

void ih_core_int_init_objectey(ih_core_objectey_t *objectey)
{
  assert(objectey);

  objectey->get_as_string = ih_core_int_get_as_string;
  objectey->compare = ih_core_int_compare;
  objectey->copy = ih_core_int_copy;
  objectey->destroy = ih_core_int_destroy;
}
