#ifndef ih_net_exchange_h
#define ih_net_exchange_h

#include "ih/net/postey.h"

struct ih_net_exchange_t;
typedef struct ih_net_exchange_t ih_net_exchange_t;

ih_net_exchange_t *ih_net_exchange_create(ih_net_postey_t *postey);

void ih_net_exchange_destroy(ih_net_exchange_t *exchange);

unsigned long ih_net_exchange_get_post_count(ih_net_exchange_t *exchange);

ih_core_bool_t ih_net_exchange_register_post(ih_net_exchange_t *exchange,
    void *post_object);

void ih_net_exchange_send_and_receive_messages(ih_net_exchange_t *exchange);

ih_core_bool_t ih_net_exchange_unregister_post(ih_net_exchange_t *exchange,
    int socket);

#endif
