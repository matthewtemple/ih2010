#ifndef ih_config_file_h
#define ih_config_file_h

#include "ih/container/list.h"

struct ih_config_file_t;
typedef struct ih_config_file_t ih_config_file_t;

ih_config_file_t *ih_config_file_create(char *filename);

void ih_config_file_destroy(ih_config_file_t *conf);

ih_core_bool_t ih_config_file_find(ih_config_file_t *file, char *name);

ih_core_bool_t ih_config_file_find_as_double(ih_config_file_t *file,
    char *name, double *value, double default_value);

ih_core_bool_t ih_config_file_find_as_string(ih_config_file_t *file, char *name,
    char **value, char *default_value);

ih_core_bool_t ih_config_file_find_as_unsigned_long(ih_config_file_t *file,
    char *name, unsigned long *value, unsigned long default_value);

ih_core_bool_t ih_config_file_find_as_unsigned_short(ih_config_file_t *file,
    char *name, unsigned short *value, unsigned short default_value);

ih_core_bool_t ih_config_file_find_list_as_strings(ih_config_file_t *file,
    char *name, ih_container_list_t **list);

#endif
