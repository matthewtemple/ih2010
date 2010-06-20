#ifndef ih_core_buffer_h
#define ih_core_buffer_h

#include "ih/core/bool.h"

struct ih_core_buffer_t;
typedef struct ih_core_buffer_t ih_core_buffer_t;

ih_core_bool_t ih_core_buffer_append_char(ih_core_buffer_t *buffer, char c);

ih_core_bool_t ih_core_buffer_append_string(ih_core_buffer_t *buffer,
    char *string, unsigned long string_size);

void ih_core_buffer_clear(ih_core_buffer_t *buffer);

ih_core_buffer_t *ih_core_buffer_create(char *string, unsigned long string_size);

void ih_core_buffer_destroy(ih_core_buffer_t *buffer);

char *ih_core_buffer_get_string(ih_core_buffer_t *buffer,
    unsigned long *string_size);

char *ih_core_buffer_take_string(ih_core_buffer_t *buffer,
    unsigned long *string_size);

#endif
