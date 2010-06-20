#ifndef ih_pingpong_ping_h
#define ih_pingpong_ping_h

#include "ih/core/bool.h"
#include "ih/core/message.h"

struct ih_pingpong_ping_t;
typedef struct ih_pingpong_ping_t ih_pingpong_ping_t;

ih_core_bool_t ih_pingpong_ping_add_to_message(void *ping_object,
    ih_core_message_t *message);

ih_pingpong_ping_t *ih_pingpong_ping_create(char *string);

ih_pingpong_ping_t *ih_pingpong_ping_create_from_message(ih_core_message_t *message);

void ih_pingpong_ping_destroy(ih_pingpong_ping_t *ping);

char *ih_pingpong_ping_get_string(ih_pingpong_ping_t *ping);

#endif
