#ifndef ih_net_testengine_h
#define ih_net_testengine_h

#include "ih/core/engine.h"
#include "ih/net/server.h"

struct ih_net_testengine_t;
typedef struct ih_net_testengine_t ih_net_testengine_t;

void *ih_net_testengine_create(ih_net_server_t *ih_net_server,
    void *custom_server_object);

void ih_net_testengine_destroy(void *engine_object);

ih_net_server_handle_message_f ih_net_testengine_get_handler_for_message
(void *engine_object, void *message_object);

void ih_net_testengine_maintain(void *engine_object);

void ih_net_testengine_run(void *engine_thread_object);

void ih_net_testengine_start(void *engine_thread_object);

void ih_net_testengine_stop(void *engine_thread_object);

#endif
