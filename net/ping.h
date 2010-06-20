#ifndef ih_net_ping_h
#define ih_net_ping_h

#include "ih/core/message.h"

struct ih_net_ping_t;
typedef struct ih_net_ping_t ih_net_ping_t;

ih_core_bool_t ih_net_ping_add_to_message(void *ping_object,
    ih_core_message_t *message);

ih_net_ping_t *ih_net_ping_create(const char *string);

ih_net_ping_t *ih_net_ping_create_from_message(ih_core_message_t *message);

void ih_net_ping_destroy(ih_net_ping_t *ping);

char *ih_net_ping_get_string(ih_net_ping_t *ping);

#endif
