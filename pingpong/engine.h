#ifndef ih_pingpong_engine_h
#define ih_pingpong_engine_h

#include "ih/core/engine.h"
#include "ih/net/server.h"

struct ih_pingpong_engine_t;
typedef struct ih_pingpong_engine_t ih_pingpong_engine_t;

void *ih_pingpong_engine_create(ih_net_server_t *ih_net_server,
    void *custom_server_object);

void ih_pingpong_engine_destroy(void *engine_object);

ih_net_server_handle_message_f ih_pingpong_engine_get_handler_for_message
(void *engine_object, void *message_object);

void ih_pingpong_engine_maintain(void *engine_object);

void ih_pingpong_engine_run(void *engine_thread_object);

void ih_pingpong_engine_start(void *engine_thread_object);

void ih_pingpong_engine_stop(void *engine_thread_object);

#endif
