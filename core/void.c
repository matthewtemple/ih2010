#include "ih/core/tools.h"
#include "ih/core/void.h"

int ih_core_void_compare(void *object_a, void *object_b)
{
  int compare;

  if (object_a > object_b) {
    compare = 1;
  } else if (object_a < object_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *ih_core_void_copy(void *object)
{
  return object;
}

void ih_core_void_destroy(void *object)
{
  free(object);
}

char *ih_core_void_get_as_string(void *object)
{
  ih_core_trace_exit("TODO: implement/test");

  assert(object);
  char *string;

  string = malloc(10 + 1);
  if (string) {
    snprintf(string, 10 + 1, "%p", object);
  } else {
    ih_core_trace_exit("malloc");
  }

  return string;
}

void ih_core_void_init_objectey(ih_core_objectey_t *objectey)
{
  assert(objectey);

  objectey->get_as_string = ih_core_void_get_as_string;
  objectey->compare = ih_core_void_compare;
  objectey->copy = ih_core_void_copy;
  objectey->destroy = ih_core_void_destroy;
}
