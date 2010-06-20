#include "ih/core/tools.h"
#include "ih/core/types.h"
#include "ih/external/external.h"
#include "ih/net/engine_thread.h"
#include "ih/net/pong.h"
#include "ih/net/server.h"
#include "ih/net/testengine.h"
#include "ih/net/testmessage.h"

struct ih_net_testengine_t {
  void *things_the_engine_needs_to_share;
  ih_net_server_t *ih_net_server;
  void *custom_server_object;
};

static ih_net_message_status_t handle_ping(void *engine_object,
    void *message_object);

ih_net_message_status_t handle_ping(void *engine_object,
    void *message_object)
{
  assert(engine_object);
  assert(message_object);
  ih_net_ping_t *ping;
  ih_core_message_t *pong_message;
  ih_net_testengine_t *engine;
  ih_net_server_t *ih_net_server;
  ih_net_message_status_t message_status;
  int client_socket;
  ih_core_message_t *message;
  ih_net_pong_t *pong;
  char *ping_string;

  message = message_object;
  engine = engine_object;
  ih_net_server = engine->ih_net_server;
  client_socket = ih_core_message_get_client_socket(message);

  ping = ih_net_ping_create_from_message(message);
  if (ping) {
    ping_string = ih_net_ping_get_string(ping);
    if (ping_string) {
      pong = ih_net_pong_create(ping_string);
      if (pong) {
        pong_message
          = ih_net_testmessage_create(client_socket, IH_NET_TESTMESSAGE_PONG);
        if (pong_message) {
          if (ih_net_pong_add_to_message(pong, pong_message)) {
            if (ih_net_server_send_message(ih_net_server, pong_message)) {
              message_status = IH_NET_MESSAGE_STATUS_HANDLED;
            } else {
              message_status = IH_NET_MESSAGE_STATUS_CANT_HANDLE;
              ih_core_message_destroy(pong_message);
            }
          } else {
            message_status = IH_NET_MESSAGE_STATUS_CANT_HANDLE;
            ih_core_trace("ih_net_pong_add_to_message");
            ih_core_message_destroy(pong_message);
          }
        } else {
          message_status = IH_NET_MESSAGE_STATUS_CANT_HANDLE;
          ih_core_trace("ih_net_testmessage_create");
        }
        ih_net_pong_destroy(pong);
      } else {
        message_status = IH_NET_MESSAGE_STATUS_CANT_HANDLE;
        ih_core_trace("ih_net_pong_create");
      }
    } else {
      message_status = IH_NET_MESSAGE_STATUS_CANT_HANDLE;
      ih_core_trace("ih_net_ping_get_string");
    }
    ih_net_ping_destroy(ping);
  } else {
    message_status = IH_NET_MESSAGE_STATUS_CANT_HANDLE;
    ih_core_trace("ih_net_ping_create_from_message");
  }

  return message_status;
}

void *ih_net_testengine_create(ih_net_server_t *ih_net_server,
    void *custom_server_object)
{
  assert(ih_net_server);
  assert(custom_server_object);
  ih_net_testengine_t *engine;

  printf("test engine creating context\n");

  engine = malloc(sizeof *engine);
  if (engine) {
    engine->ih_net_server = ih_net_server;
    engine->custom_server_object = custom_server_object;
  }

  ih_net_server_register_message_handler(ih_net_server, IH_NET_ENGINE_TEST,
      IH_NET_TESTMESSAGE_PING, handle_ping);
  return engine;
}

void ih_net_testengine_destroy(void *engine_object)
{
  assert(engine_object);

  printf("test engine destroying\n");
  free(engine_object);
}

ih_net_server_handle_message_f ih_net_testengine_get_handler_for_message
(void *engine_object, void *message_object)
{
  assert(engine_object);
  assert(message_object);
  ih_net_testengine_t *engine;
  ih_net_server_handle_message_f handler;
  ih_core_message_t *message;

  engine = engine_object;
  message = message_object;
  handler = ih_net_server_get_handler_for_message(engine->ih_net_server, message);

  return handler;
}

void ih_net_testengine_maintain(void *engine_object)
{
  assert(engine_object);
  ih_net_testengine_t *engine;

  engine = engine_object;
}

void ih_net_testengine_run(void *engine_thread_object)
{
  assert(engine_thread_object);
  ih_net_engine_thread_t *engine_thread;
  ih_net_testengine_t *engine;
  ih_net_server_t *server;
  unsigned short thread_index;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;
  server = engine->ih_net_server;
  thread_index = engine_thread->thread_index;

  ih_net_server_process_messages(server, IH_NET_ENGINE_TEST, thread_index);
}

void ih_net_testengine_start(void *engine_thread_object)
{
  assert(engine_thread_object);
  ih_net_engine_thread_t *engine_thread;
  ih_net_testengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("test engine starting\n");
}

void ih_net_testengine_stop(void *engine_thread_object)
{
  assert(engine_thread_object);
  ih_net_engine_thread_t *engine_thread;
  ih_net_testengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("test engine stopping\n");
}
