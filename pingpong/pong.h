#ifndef ih_pingpong_pong_h
#define ih_pingpong_pong_h

#include "ih/core/bool.h"
#include "ih/core/message.h"

struct ih_pingpong_pong_t;
typedef struct ih_pingpong_pong_t ih_pingpong_pong_t;

ih_core_bool_t ih_pingpong_pong_add_to_message(void *pong_object,
    ih_core_message_t *message);

ih_pingpong_pong_t *ih_pingpong_pong_create(char *string);

ih_pingpong_pong_t *ih_pingpong_pong_create_from_message(ih_core_message_t *message);

void ih_pingpong_pong_destroy(ih_pingpong_pong_t *pong);

#endif
