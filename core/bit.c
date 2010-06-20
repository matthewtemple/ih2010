#include "ih/core/bit.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

int ih_core_bit_compare(void *bit_object_a,
    void *bit_object_b)
{
  ih_core_bit_t *bit_a;
  ih_core_bit_t *bit_b;
  int compare;

  bit_a = bit_object_a;
  bit_b = bit_object_b;
  if (*bit_a > *bit_b) {
    compare = 1;
  } else if (*bit_a < *bit_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *ih_core_bit_copy(void *bit_object)
{
  assert(bit_object);
  ih_core_bit_t *bit;
  ih_core_bit_t *bit_copy;

  bit = bit_object;
  bit_copy = malloc(sizeof *bit_copy);
  if (bit_copy) {
    *bit_copy = *bit;
  } else {
    ih_core_trace("mallloc");
  }

  return bit_copy;
}

void ih_core_bit_destroy(void *bit_object)
{
  assert(bit_object);
  free(bit_object);
}
