#ifndef ih_config_environment_h
#define ih_config_environment_h

#include "ih/container/list.h"
#include "ih/external/external.h"

struct ih_config_environment_t;
typedef struct ih_config_environment_t ih_config_environment_t;

ih_config_environment_t *ih_config_environment_create();

void ih_config_environment_destroy(ih_config_environment_t *environment);

ih_core_bool_t ih_config_environment_find(ih_config_environment_t *environment,
    char *name);

ih_core_bool_t ih_config_environment_find_as_double
(ih_config_environment_t *environment, char *name, double *value,
    double default_value);

ih_core_bool_t ih_config_environment_find_as_string
(ih_config_environment_t *environment, char *name, char **value,
    char *default_value);

ih_core_bool_t ih_config_environment_find_as_unsigned_long
(ih_config_environment_t *environment, char *name, unsigned long *value,
    unsigned long default_value);

ih_core_bool_t ih_config_environment_find_as_unsigned_short
(ih_config_environment_t *environment, char *name, unsigned short *value,
    unsigned short default_value);

ih_core_bool_t ih_config_environment_find_list_as_strings
(ih_config_environment_t *environment, char *name, ih_container_list_t **list);

#endif
