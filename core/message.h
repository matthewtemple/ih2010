#ifndef ih_core_message_h
#define ih_core_message_h

#include "ih/core/engine.h"
#include "ih/core/object.h"
#include "ih/core/types.h"

enum ih_core_message_encoding_t {
  IH_CORE_MESSAGE_ENCODING_UNKNOWN = 0,
  IH_CORE_MESSAGE_ENCODING_PLAIN = 1,
};
typedef enum ih_core_message_encoding_t ih_core_message_encoding_t;

typedef ih_core_bool_t ih_core_message_add_to_message_f(void *object,
    ih_core_message_t *message);

typedef void *ih_core_message_create_from_message_f(ih_core_message_t *message);

ih_core_bool_t ih_core_message_add_bool(ih_core_message_t *message, void *object);

ih_core_bool_t ih_core_message_add_double(ih_core_message_t *message,
    void *object);

ih_core_bool_t ih_core_message_add_long(ih_core_message_t *message, void *object);

ih_core_bool_t ih_core_message_add_short(ih_core_message_t *message,
    void *object);

ih_core_bool_t ih_core_message_add_string(ih_core_message_t *message,
    void *object);

void *ih_core_message_copy(void *message_object);

ih_core_message_t *ih_core_message_create(int client_socket,
    ih_net_engine_id_t engine_id, unsigned long message_type, char *data,
    unsigned long data_size);

void ih_core_message_destroy(void *message_object);

int ih_core_message_get_client_socket(void *message_object);

char *ih_core_message_get_data(ih_core_message_t *message);

unsigned long ih_core_message_get_data_get_size(ih_core_message_t *message);

ih_core_message_encoding_t ih_core_message_get_encoding
(ih_core_message_t *message);

ih_net_engine_id_t ih_core_message_get_engine_id(void *message_object);

unsigned long ih_core_message_get_type(void *message_object);

void *ih_core_message_take_bool(ih_core_message_t *message);

ih_core_bool_t ih_core_message_take_bool_value(ih_core_message_t *message);

void *ih_core_message_take_double(ih_core_message_t *message);

double ih_core_message_take_double_value(ih_core_message_t *message);

void *ih_core_message_take_long(ih_core_message_t *message);

long ih_core_message_take_long_value(ih_core_message_t *message);

void *ih_core_message_take_short(ih_core_message_t *message);

short ih_core_message_take_short_value(ih_core_message_t *message);

void *ih_core_message_take_string(ih_core_message_t *message);

#endif
