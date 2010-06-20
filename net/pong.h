#ifndef ih_net_pong_h
#define ih_net_pong_h

#include "ih/core/message.h"

struct ih_net_pong_t;
typedef struct ih_net_pong_t ih_net_pong_t;

ih_core_bool_t ih_net_pong_add_to_message(void *pong_object,
    ih_core_message_t *message);

ih_net_pong_t *ih_net_pong_create(char *string);

ih_net_pong_t *ih_net_pong_create_from_message(ih_core_message_t *message);

void ih_net_pong_destroy(ih_net_pong_t *pong);

#endif
