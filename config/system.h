#ifndef ih_config_system_h
#define ih_config_system_h

#include "ih/core/bool.h"
#include "ih/core/objects.h"
#include "ih/container/list.h"
#include "ih/external/external.h"

struct ih_config_system_t;
typedef struct ih_config_system_t ih_config_system_t;

ih_config_system_t *ih_config_system_create(int argc, char *argv[],
    char *filename, ih_core_objects_t *objects);

void ih_config_system_destroy(ih_config_system_t *system);

ih_core_bool_t ih_config_system_find(ih_config_system_t *system, char *name);

ih_core_bool_t ih_config_system_find_as_double(ih_config_system_t *system,
    char *name, double *value, double default_value);

ih_core_bool_t ih_config_system_find_as_string(ih_config_system_t *system,
    char *name, char **value, char *default_value);

ih_core_bool_t ih_config_system_find_as_unsigned_long(ih_config_system_t *system,
    char *name, unsigned long *value, unsigned long default_value);

ih_core_bool_t ih_config_system_find_as_unsigned_short(ih_config_system_t *system,
    char *name, unsigned short *value, unsigned short default_value);

ih_core_bool_t ih_config_system_find_list_as_strings(ih_config_system_t *system,
    char *name, ih_container_list_t **list);

#endif
