#ifndef ih_net_testhyperengine_h
#define ih_net_testhyperengine_h

#include "ih/core/engine.h"

struct ih_net_testhyperengine_t;
typedef struct ih_net_testhyperengine_t ih_net_testhyperengine_t;

void *ih_net_testhyperengine_create(ih_net_server_t *ih_net_server,
    void *custom_server_object);

ih_net_server_handle_message_f ih_net_testhyperengine_get_handler_for_message
(void *engine_object, void *hypermessage_object);

void ih_net_testhyperengine_destroy(void *engine_object);

void ih_net_testhyperengine_maintain(void *engine_object);

void ih_net_testhyperengine_run(void *engine_thread_object);

void ih_net_testhyperengine_start(void *engine_thread_object);

void ih_net_testhyperengine_stop(void *engine_thread_object);

#endif
