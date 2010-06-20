#include "ih/container/map.h"
#include "ih/core/long.h"
#include "ih/core/objects.h"
#include "ih/core/string.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

int main(int argc, char *argv[])
{
  ih_container_map_t *map;
  ih_core_objects_t objects;

  ih_core_objects_init(&objects);

  char *a_left = "apple";
  unsigned long a_right = 0;

  char *b_left = "bear";
  unsigned long b_right = 1;

  unsigned long *x_right;

  map = ih_container_map_create(&objects.string_objectey,
      &objects.long_objectey, IH_CONTAINER_MAP_DONT_DESTROY);
  if (!map) {
    ih_core_trace_exit("ih_container_map_create");
  }

  if (!ih_container_map_add(map, a_left, &a_right)) {
    ih_core_trace_exit("ih_container_map_add");
  }

  if (!ih_container_map_add(map, b_left, &b_right)) {
    ih_core_trace_exit("ih_container_map_add");
  }

  ih_container_map_print(map);

  x_right = ih_container_map_find(map, b_left);
  if (x_right) {
    printf(":%lu:\n", *x_right);
  } else {
    ih_core_trace_exit("ih_container_map_find");
  }

  ih_container_map_destroy(map);
  ih_core_objects_free(&objects);

  return 0;
}
