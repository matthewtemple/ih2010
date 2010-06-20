#include "ih/config/environment.h"
#include "ih/config/file.h"
#include "ih/config/options.h"
#include "ih/config/system.h"
#include "ih/core/tools.h"

struct ih_config_system_t {
  ih_config_file_t *file;
  ih_config_environment_t *environment;
  ih_config_options_t *options;
};

ih_config_system_t *ih_config_system_create(int argc, char *argv[],
    char *filename, ih_core_objects_t *objects)
{
  assert(argv);
  assert(objects);
  ih_config_system_t *system;
  ih_core_bool_t so_far_so_good;

  system = malloc(sizeof *system);
  if (system) {
    system->environment = NULL;
    system->options = NULL;
    if (filename) {
      system->file = ih_config_file_create(filename);
      if (system->file) {
        so_far_so_good = ih_core_bool_true;
      } else {
        so_far_so_good = ih_core_bool_false;
        ih_core_trace("ih_config_file_create");
      }
    } else {
      so_far_so_good = ih_core_bool_true;
      system->file = NULL;
    }
  } else {
    so_far_so_good = ih_core_bool_false;
    ih_core_trace("malloc");
  }

  if (so_far_so_good) {
    system->environment = ih_config_environment_create();
    if (!system->environment) {
      so_far_so_good = ih_core_bool_false;
      ih_core_trace("ih_config_environment_create");
    }
  }

  if (so_far_so_good) {
    system->options = ih_config_options_create(argc, argv, objects);
    if (!system->options) {
      so_far_so_good = ih_core_bool_false;
      ih_core_trace("ih_config_options_create");
    }
  }

  if (!so_far_so_good && system) {
    if (system->file) {
      ih_config_file_destroy(system->file);
    }
    if (system->environment) {
      ih_config_environment_destroy(system->environment);
    }
    if (system->options) {
      ih_config_options_destroy(system->options);
    }
    free(system);
    system = NULL;
  }

  return system;
}

void ih_config_system_destroy(ih_config_system_t *system)
{
  assert(system);

  if (system->file) {
    ih_config_file_destroy(system->file);
  }
  ih_config_environment_destroy(system->environment);
  ih_config_options_destroy(system->options);
  free(system);
}

ih_core_bool_t ih_config_system_find(ih_config_system_t *system, char *name)
{
  assert(system);
  assert(name);
  ih_core_bool_t found_it;

  found_it = ih_core_bool_false;

  if (ih_config_options_find(system->options, name)) {
    found_it = ih_core_bool_true;
  } else {
    if (ih_config_environment_find(system->environment, name)) {
      found_it = ih_core_bool_true;
    } else {
      if (system->file && ih_config_file_find(system->file, name)) {
        found_it = ih_core_bool_true;
      }
    }
  }

  return found_it;
}

ih_core_bool_t ih_config_system_find_as_double(ih_config_system_t *system,
    char *name, double *value, double default_value)
{
  assert(system);
  assert(name);
  assert(value);
  ih_core_bool_t found_it;

  found_it = ih_core_bool_false;

  if (ih_config_options_find_as_double
      (system->options, name, value, default_value)) {
    found_it = ih_core_bool_true;
  } else {
    if (ih_config_environment_find_as_double
        (system->environment, name, value, default_value)) {
      found_it = ih_core_bool_true;
    } else {
      if (system->file && ih_config_file_find_as_double
          (system->file, name, value, default_value)) {
        found_it = ih_core_bool_true;
      }
    }
  }

  return found_it;
}

ih_core_bool_t ih_config_system_find_as_string(ih_config_system_t *system,
    char *name, char **value, char *default_value)
{
  assert(system);
  assert(name);
  assert(value);
  assert(default_value);
  ih_core_bool_t found_it;

  found_it = ih_core_bool_false;

  if (ih_config_options_find_as_string
      (system->options, name, value, default_value)) {
    found_it = ih_core_bool_true;
  } else {
    if (ih_config_environment_find_as_string
        (system->environment, name, value, default_value)) {
      found_it = ih_core_bool_true;
    } else {
      if (system->file && ih_config_file_find_as_string
          (system->file, name, value, default_value)) {
        found_it = ih_core_bool_true;
      }
    }
  }

  return found_it;
}

ih_core_bool_t ih_config_system_find_as_unsigned_long(ih_config_system_t *system,
    char *name, unsigned long *value, unsigned long default_value)
{
  assert(system);
  assert(name);
  assert(value);
  ih_core_bool_t found_it;

  found_it = ih_core_bool_false;

  if (ih_config_options_find_as_unsigned_long
      (system->options, name, value, default_value)) {
    found_it = ih_core_bool_true;
  } else {
    if (ih_config_environment_find_as_unsigned_long
        (system->environment, name, value, default_value)) {
      found_it = ih_core_bool_true;
    } else {
      if (system->file && ih_config_file_find_as_unsigned_long
          (system->file, name, value, default_value)) {
        found_it = ih_core_bool_true;
      }
    }
  }

  return found_it;
}

ih_core_bool_t ih_config_system_find_as_unsigned_short(ih_config_system_t *system,
    char *name, unsigned short *value, unsigned short default_value)
{
  assert(system);
  assert(name);
  assert(value);
  ih_core_bool_t found_it;

  found_it = ih_core_bool_false;

  if (ih_config_options_find_as_unsigned_short
      (system->options, name, value, default_value)) {
    found_it = ih_core_bool_true;
  } else {
    if (ih_config_environment_find_as_unsigned_short
        (system->environment, name, value, default_value)) {
      found_it = ih_core_bool_true;
    } else {
      if (system->file && ih_config_file_find_as_unsigned_short
          (system->file, name, value, default_value)) {
        found_it = ih_core_bool_true;
      }
    }
  }

  return found_it;
}

ih_core_bool_t ih_config_system_find_list_as_strings(ih_config_system_t *system,
    char *name, ih_container_list_t **list)
{
  assert(system);
  assert(name);
  ih_core_bool_t found_it;

  found_it = ih_core_bool_false;

  if (ih_config_options_find_list_as_strings(system->options, name, list)) {
    found_it = ih_core_bool_true;
  } else {
    if (ih_config_environment_find_list_as_strings
        (system->environment, name, list)) {
      found_it = ih_core_bool_true;
    } else {
      if (system->file
          && ih_config_file_find_list_as_strings(system->file, name, list)) {
        found_it = ih_core_bool_true;
      }
    }
  }

  return found_it;
}
