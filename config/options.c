#include "ih/config/options.h"
#include "ih/container/dumpster.h"
#include "ih/container/map.h"
#include "ih/core/string.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

struct ih_config_options_t {
  ih_container_map_t *options;
  ih_container_dumpster_t *dumpster;
};

static void parse_options(ih_config_options_t *options, int argc, char *argv[]);

ih_config_options_t *ih_config_options_create(int argc, char *argv[],
    ih_core_objects_t *objects)
{
  ih_config_options_t *options;
  ih_core_bool_t so_far_so_good;

  options = malloc(sizeof *options);
  if (options) {
    options->dumpster = NULL;
    options->options = ih_container_map_create(&objects->string_objectey,
        &objects->string_objectey, IH_CONTAINER_MAP_DONT_DESTROY);
    if (options->options) {
      so_far_so_good = ih_core_bool_true;
    } else {
      ih_core_trace("ih_container_map_create");
      so_far_so_good = ih_core_bool_false;
    }
  } else {
    ih_core_trace("malloc");
    so_far_so_good = ih_core_bool_false;
  }

  if (so_far_so_good) {
    options->dumpster = ih_container_dumpster_create(&objects->string_objectey);
    if (!options->dumpster) {
      ih_core_trace("ih_container_dumpster_create");
    }
  }

  if (so_far_so_good) {
    parse_options(options, argc, argv);
  }

  if (!so_far_so_good && options) {
    ih_container_map_destroy(options->options);
    if (options->dumpster) {
      ih_container_dumpster_destroy(options->dumpster);
    }
    free(options);
    options = NULL;
  }

  return options;
}

void ih_config_options_destroy(ih_config_options_t *options)
{
  assert(options);

  ih_container_map_destroy(options->options);
  ih_container_dumpster_destroy(options->dumpster);
  free(options);
}

ih_core_bool_t ih_config_options_find(ih_config_options_t *options, char *name)
{
  assert(options);
  assert(name);
  ih_core_bool_t found;

  if (ih_container_map_find(options->options, name)) {
    found = ih_core_bool_true;
  } else {
    found = ih_core_bool_false;
  }

  return found;
}

ih_core_bool_t ih_config_options_find_as_double(ih_config_options_t *options,
    char *name, double *value, double default_value)
{
  assert(options);
  assert(name);
  assert(value);
  ih_core_bool_t found;
  char *value_string;

  value_string = ih_container_map_find(options->options, name);
  if (value_string) {
    found = ih_core_bool_true;
    *value = atof(value_string);
  } else {
    found = ih_core_bool_false;
    *value = default_value;
  }

  return found;
}

ih_core_bool_t ih_config_options_find_as_string(ih_config_options_t *options,
    char *name, char **value, char *default_value)
{
  assert(options);
  assert(name);
  assert(value);
  assert(default_value);
  ih_core_bool_t found;

  *value = ih_container_map_find(options->options, name);
  if (*value) {
    found = ih_core_bool_true;
  } else {
    found = ih_core_bool_false;
    *value = default_value;
  }

  return found;
}

ih_core_bool_t ih_config_options_find_as_unsigned_short
(ih_config_options_t *options, char *name, unsigned short *value,
    unsigned short default_value)
{
  assert(options);
  assert(name);
  assert(value);
  ih_core_bool_t found;
  char *value_string;

  value_string = ih_container_map_find(options->options, name);
  if (value_string) {
    found = ih_core_bool_true;
    *value = atoi(value_string);
  } else {
    found = ih_core_bool_false;
    *value = default_value;
  }

  return found;
}

ih_core_bool_t ih_config_options_find_as_unsigned_long
(ih_config_options_t *options, char *name, unsigned long *value,
    unsigned long default_value)
{
  assert(options);
  assert(name);
  assert(value);
  ih_core_bool_t found;
  char *value_string;

  value_string = ih_container_map_find(options->options, name);
  if (value_string) {
    found = ih_core_bool_true;
    *value = atol(value_string);
  } else {
    found = ih_core_bool_false;
    *value = default_value;
  }

  return found;
}

ih_core_bool_t ih_config_options_find_list_as_strings
(ih_config_options_t *options, char *name, ih_container_list_t **list)
{
  ih_core_trace_exit("TODO: implement");
  return ih_core_bool_false;
}

void parse_options(ih_config_options_t *options, int argc, char *argv[])
{
  assert(options);
  int each_word;
  char *substring;
  char *name;
  char *value;

  for (each_word = 1; (each_word + 1) < argc; each_word += 2) {
    name = *(argv + each_word);
    substring = ih_core_string_substring(name, 0, 2);
    if (substring) {
      if (0 == strcmp("--", substring)) {
        name = strdup(name + 2);
        if (name) {
          if (!ih_container_dumpster_add(options->dumpster, name)) {
            ih_core_trace("ih_container_dumpster_add");
          }
          value = strdup(*(argv + each_word + 1));
          if (!ih_container_dumpster_add(options->dumpster, value)) {
            ih_core_trace("ih_container_dumpster_add");
          }
          if (!ih_container_map_add(options->options, name, value)) {
            ih_core_trace("ih_container_map_add");
            free(name);
          }
        } else {
          ih_core_trace("strdup");
        }
      }
      free(substring);
    } else {
      ih_core_trace("ih_core_string_substring");
    }
  }
}
