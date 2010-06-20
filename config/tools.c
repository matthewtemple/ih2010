#include "ih/config/tools.h"
#include "ih/config/options.h"
#include "ih/core/tools.h"

void ih_config_disable_if_running_batcih_tests(int argc, char *argv[])
{
  if (ih_config_running_batcih_tests(argc, argv)) {
    printf("  %s disabled in batch test mode\n", *argv);
    exit(0);
  }
}

ih_core_bool_t ih_config_running_batcih_tests(int argc, char *argv[])
{
  ih_config_options_t *options;
  ih_core_bool_t batch;
  ih_core_objects_t objects;

  batch = ih_core_bool_false;
  ih_core_objects_init(&objects);

  options = ih_config_options_create(argc, argv, &objects);
  if (options) {
    if (ih_config_options_find(options, "running-batch-tests")) {
      batch = ih_core_bool_true;
    }
    ih_config_options_destroy(options);
  } else {
    ih_core_trace("ih_config_options_create");
  }

  ih_core_objects_free(&objects);

  return batch;
}
