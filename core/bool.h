#ifndef ih_core_bool_h
#define ih_core_bool_h

#include "ih/core/declarations.h"

enum ih_core_bool_t {
  ih_core_bool_false = 0,
  ih_core_bool_true = 1,
};
typedef enum ih_core_bool_t ih_core_bool_t;

ih_core_bool_t ih_core_bool_add_to_message(void *bool_object,
    ih_core_message_t *message);

void *ih_core_bool_create_from_message(ih_core_message_t *message);

#endif
