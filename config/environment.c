#include "ih/core/tools.h"
#include "ih/config/environment.h"

struct ih_config_environment_t {
};

ih_config_environment_t *ih_config_environment_create()
{
  ih_config_environment_t *environment;

  environment = malloc(sizeof *environment);
  if (environment) {

  } else {
    ih_core_trace("malloc");
  }

  return environment;
}

void ih_config_environment_destroy(ih_config_environment_t *environment)
{
  assert(environment);
  free(environment);
}

ih_core_bool_t ih_config_environment_find(ih_config_environment_t *environment,
    char *name)
{
  assert(environment);
  assert(name);

  return ih_core_bool_false;
}

ih_core_bool_t ih_config_environment_find_as_double
(ih_config_environment_t *environment, char *name, double *value,
    double default_value)
{
  assert(environment);
  assert(name);
  assert(value);

  return ih_core_bool_false;
}

ih_core_bool_t ih_config_environment_find_as_string
(ih_config_environment_t *environment, char *name, char **value,
    char *default_value)
{
  assert(environment);
  assert(name);
  assert(value);
  assert(default_value);

  return ih_core_bool_false;
}

ih_core_bool_t ih_config_environment_find_as_unsigned_long
(ih_config_environment_t *environment, char *name, unsigned long *value,
    unsigned long default_value)
{
  assert(environment);
  assert(name);
  assert(value);

  return ih_core_bool_false;
}

ih_core_bool_t ih_config_environment_find_as_unsigned_short
(ih_config_environment_t *environment, char *name, unsigned short *value,
    unsigned short default_value)
{
  assert(environment);
  assert(name);
  assert(value);

  return ih_core_bool_false;
}

ih_core_bool_t ih_config_environment_find_list_as_strings
(ih_config_environment_t *environment, char *name, ih_container_list_t **list)
{
  assert(environment);
  assert(name);
  assert(list);

  return ih_core_bool_false;
}
