#ifndef ih_file_basic_h
#define ih_file_basic_h

#include "ih/container/list.h"
#include "ih/core/types.h"
#include "ih/external/external.h"
#include "ih/file/mode.h"

struct ih_file_basic_t;
typedef struct ih_file_basic_t ih_file_basic_t;

ih_file_basic_t *ih_file_basic_create(const char *filename, ih_file_mode_t mode);

void ih_file_basic_destroy(ih_file_basic_t *file);

ih_core_bool_t ih_file_basic_exists(const char *filename);

ih_core_bool_t ih_file_basic_get_as_blob(ih_file_basic_t *file, char **blob,
    unsigned long *blob_size);

ih_container_list_t *ih_file_basic_get_as_line_list(ih_file_basic_t *file);

char *ih_file_basic_get_as_string(ih_file_basic_t *file);

ih_container_list_t *ih_file_basic_get_as_token_list(ih_file_basic_t *file,
    char *delimiters);

FILE *ih_file_basic_get_file(ih_file_basic_t *file);

ih_core_bool_t ih_file_basic_get_line_count(ih_file_basic_t *file,
    unsigned long *line_count);

ih_core_bool_t ih_file_basic_write_char(ih_file_basic_t *file, int c);

ih_core_bool_t ih_file_basic_write_string(ih_file_basic_t *file, char *string);

#endif
