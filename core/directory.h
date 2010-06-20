#ifndef ih_core_directory_h
#define ih_core_directory_h

#include "ih/core/bool.h"
#include "ih/external/external.h"

struct ih_core_directory_t;
typedef struct ih_core_directory_t ih_core_directory_t;

ih_core_directory_t *ih_core_directory_create();

void ih_core_directory_destroy(ih_core_directory_t *ih_core_directory);

ih_core_bool_t ih_core_directory_exists(char *directory_name);

ih_core_bool_t ih_core_directory_make(char *directory_name, mode_t mode);

#endif
