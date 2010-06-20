#ifndef ih_net_enginey_h
#define ih_net_enginey_h

#include "ih/core/object.h"
#include "ih/net/types.h"

typedef void *(*ih_net_enginey_create_f)
  (ih_net_server_t *server, void *custom_server_context_object);

typedef void (*ih_net_enginey_destroy_f)(void *engine_object);

typedef ih_net_server_handle_message_f(*ih_net_enginey_get_handler_for_message_f)
(void *engine_object, void *message_object);

typedef void (*ih_net_enginey_maintain_f)(void *engine_thread_context_object);

typedef void (*ih_net_enginey_run_f)(void *engine_thread_context_object);

typedef void (*ih_net_enginey_start_f)(void *engine_thread_context_object);

typedef void (*ih_net_enginey_stop_f)(void *engine_thread_context_object);

struct ih_net_enginey_t {
  ih_net_enginey_create_f create;
  ih_net_enginey_destroy_f destroy;
  ih_net_enginey_get_handler_for_message_f get_handler_for_message;
  ih_net_enginey_maintain_f maintain;
  ih_net_enginey_run_f run;
  ih_net_enginey_start_f start;
  ih_net_enginey_stop_f stop;
};
typedef struct ih_net_enginey_t ih_net_enginey_t;

void ih_net_enginey_init(ih_net_enginey_t *enginey,
    ih_net_enginey_create_f create, ih_net_enginey_destroy_f destroy,
    ih_net_enginey_get_handler_for_message_f get_handler_for_message,
    ih_net_enginey_maintain_f maintain, ih_net_enginey_run_f run,
    ih_net_enginey_start_f start, ih_net_enginey_stop_f stop);

#endif
