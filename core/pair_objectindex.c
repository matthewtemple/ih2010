#include "ih/core/pair.h"
#include "ih/core/pair_objectindex.h"
#include "ih/core/string.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

int ih_core_pair_objectindex_compare(void *pair_object_a,
    void *pair_object_b)
{
  assert(pair_object_a);
  assert(pair_object_b);
  ih_core_pair_t *pair_a;
  ih_core_pair_t *pair_b;
  unsigned long *object_a;
  unsigned long *object_b;
  int compare_result;
  ih_core_compare_f compare;
  ih_core_objectey_t *left_objectey;

  pair_a = pair_object_a;
  pair_b = pair_object_b;

  object_a = ih_core_pair_get_left(pair_a);
  object_b = ih_core_pair_get_left(pair_b);

  left_objectey = ih_core_pair_get_left_objectey(pair_a);
  compare = left_objectey->compare;
  compare_result = compare(object_a, object_b);

  return compare_result;
}

void *ih_core_pair_objectindex_copy(void *pair_object)
{
  ih_core_trace_exit("TODO: implement");
  return NULL;
}
