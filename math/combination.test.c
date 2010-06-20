#include "ih/container/set.h"
#include "ih/core/string.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/math/combination.h"

int main(int argc, char *argv[])
{
  ih_matih_combination_t *combination;
  ih_container_set_t *set;
  ih_container_set_t *subset;

  set = ih_container_set_create(ih_core_string_compare, ih_core_string_copy,
      IH_CORE_NO_DESTROY_FUNCTION);
  if (!set) {
    ih_core_trace_exit("ih_container_set_create");
  }

  ih_container_set_add(set, "rooster");
  ih_container_set_add(set, "cow");
  ih_container_set_add(set, "pig");
  ih_container_set_add(set, "duck");

  combination = ih_matih_combination_create(set);
  if (!combination) {
    ih_core_trace_exit("ih_matih_combination_create");
  }

  ih_matih_combination_iterate_start(combination);
  while ((subset = ih_matih_combination_iterate_next(combination))) {
    ih_container_set_print(subset, ih_core_string_get_as_string);
    ih_container_set_destroy(subset);
  }

  ih_matih_combination_destroy(combination);
  ih_container_set_destroy(set);

  return 0;
}
