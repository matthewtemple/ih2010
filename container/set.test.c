#include "ih/container/set.h"
#include "ih/core/long.h"
#include "ih/core/tools.h"
#include "ih/core/uuid.h"
#include "ih/external/external.h"

#define ITERATIONS 10000
#define TEST_USING_LONGS ih_core_bool_false

static void print_uuid(ih_core_uuid_t *uuid, const char *name);

static void print_uuid(ih_core_uuid_t *uuid, const char *name)
{
  assert(uuid);
  assert(name);
  char *uuid_string;

  uuid_string = ih_core_uuid_get_string(uuid);
  printf("%s=:%s:\n", name, uuid_string);
}

int main(int argc, char *argv[])
{
  ih_container_set_t *set;
  long *l;
  unsigned long n;
  unsigned long items_added;
  ih_core_uuid_t *uuid;

  printf("creating set...\n");
  if (TEST_USING_LONGS) {
    set = ih_container_set_create(ih_core_long_compare, ih_core_long_copy,
        ih_core_long_destroy);
  } else {
    set = ih_container_set_create(ih_core_uuid_compare, ih_core_uuid_copy,
        ih_core_uuid_destroy);
  }
  assert(set);

  printf("adding to set...\n");
  items_added = 0;
  for (n = 0; n < ITERATIONS; n++) {
    if (TEST_USING_LONGS) {
      l = malloc(sizeof *l);
      *l = random() % ITERATIONS;
      if (ih_container_set_add(set, l)) {
        items_added++;
      } else {
        ih_core_long_destroy(l);
      }
    } else {
      uuid = ih_core_uuid_create();
      if (ih_core_bool_false) {
        print_uuid(uuid, "uuid");
      }
      if (ih_container_set_add(set, uuid)) {
        items_added++;
      } else {
        ih_core_uuid_destroy(uuid);
      }
    }
  }
  printf("items added: %lu\n", items_added);
  printf("set size: %lu\n", ih_container_set_get_size(set));

  printf("removing from set...\n");
  for (n = 0; n < ITERATIONS; n++) {
    if (TEST_USING_LONGS) {
      l = malloc(sizeof *l);
      *l = random() % ITERATIONS;
      ih_container_set_remove(set, l);
      free(l);
    }
  }
  printf("set size: %lu\n", ih_container_set_get_size(set));

  if (ih_core_bool_true) {
    printf("doing iterate_remove on the rest of the set\n");
    ih_container_set_iterate_start(set);
    if (TEST_USING_LONGS) {
      while ((l = ih_container_set_iterate_next(set))) {
        ih_container_set_iterate_remove(set);
      }
    }
    while ((uuid = ih_container_set_iterate_next(set))) {
      ih_container_set_iterate_remove(set);
    }
    printf("set size: %lu\n", ih_container_set_get_size(set));
    assert(0 == ih_container_set_get_size(set));
  }

  printf("clearing set...\n");
  ih_container_set_clear(set);
  printf("set size: %lu\n", ih_container_set_get_size(set));
  assert(0 == ih_container_set_get_size(set));

  printf("destroying set...\n");
  ih_container_set_destroy(set);

  return 0;
}
