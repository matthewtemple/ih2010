#include "ih/container/shardset.h"
#include "ih/core/string.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

#define SHARD_COUNT IH_CONTAINER_SHARDSET_MAX_SHARDS

int main(int argc, char *argv[])
{
  ih_container_shardset_t *shardset;
  char *string;

  shardset = ih_container_shardset_create(ih_core_string_compare,
      ih_core_string_copy, IH_CORE_NO_DESTROY_FUNCTION, ih_core_string_hash,
      SHARD_COUNT);
  if (!shardset) {
    ih_core_trace_exit("ih_container_shardset_create");
  }

  ih_container_shardset_add(shardset, "apple");
  ih_container_shardset_add(shardset, "bicycle");
  ih_container_shardset_add(shardset, "color");

  ih_container_shardset_iterate_start(shardset);
  while ((string = ih_container_shardset_iterate_next(shardset))) {
    printf("%s\n", string);
    if (0 == strcmp("bicycle", string)) {
      ih_container_shardset_iterate_remove(shardset);
    }
  }

  printf("\n");

  ih_container_shardset_iterate_start(shardset);
  while ((string = ih_container_shardset_iterate_next(shardset))) {
    printf("%s\n", string);
  }

  return 0;
}
