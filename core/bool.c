#include "ih/core/bool.h"
#include "ih/core/message.h"

ih_core_bool_t ih_core_bool_add_to_message(void *bool_object,
    ih_core_message_t *message)
{
  return ih_core_message_add_bool(message, bool_object);
}

void *ih_core_bool_create_from_message(ih_core_message_t *message)
{
  return ih_core_message_take_bool(message);
}
