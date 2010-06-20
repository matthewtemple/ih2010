#include "ih/core/content.h"
#include "ih/core/types.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/net/engine_thread.h"
#include "ih/net/hypermessage.h"
#include "ih/net/server.h"
#include "ih/net/testhyperengine.h"
#include "ih/net/testmessage.h"

struct ih_net_testhyperengine_t {
  void *things_the_engine_needs_to_share;
  ih_net_server_t *ih_net_server;
  void *custom_server_object;
};

static ih_net_message_status_t handle_about(void *engine_object,
    void *message_object);

static ih_net_message_status_t handle_not_found(void *engine_object,
    void *message_object);

ih_net_message_status_t handle_about(void *engine_object,
    void *message_object)
{
  assert(engine_object);
  assert(message_object);
  ih_net_testhyperengine_t *engine;
  ih_net_hypermessage_t *message;
  ih_net_hypermessage_t *response_message;
  int client_socket;
  ih_net_server_t *ih_net_server;
  char *body;
  unsigned long body_size;

  engine = engine_object;
  ih_net_server = engine->ih_net_server;
  message = message_object;
  client_socket = ih_core_message_get_client_socket(message);
  body = "about this site";
  body_size = strlen(body);

  response_message = ih_net_hypermessage_create(client_socket,
      IH_NET_HYPERMETHOD_UNKNOWN, IH_NET_HYPERSTATUS_OK,
      IH_NET_HYPERMESSAGE_NO_RESOURCE_PATH, IH_NET_HYPERVERSION_1_0,
      IH_NET_HYPERMESSAGE_NO_HEADERS);
  if (response_message) {
    ih_net_hypermessage_set_body(response_message,
        IH_CORE_CONTENT_MIME_TEXT_HTML, body, body_size);
  } else {
    ih_core_trace("ih_net_hypermessage_create");
  }

  if (!ih_net_server_send_message(ih_net_server, response_message)) {
    ih_core_trace("ih_net_server_send_message");
  }

  return IH_NET_MESSAGE_STATUS_HANDLED;
}

ih_net_message_status_t handle_not_found(void *engine_object,
    void *message_object)
{
  assert(engine_object);
  assert(message_object);
  ih_net_testhyperengine_t *engine;
  ih_net_hypermessage_t *message;
  ih_net_hypermessage_t *response_message;
  int client_socket;
  ih_net_server_t *ih_net_server;
  char *body;
  unsigned long body_size;

  engine = engine_object;
  ih_net_server = engine->ih_net_server;
  message = message_object;
  client_socket = ih_core_message_get_client_socket(message);
  body = "testhyperengine cannot handle this request";
  body_size = strlen(body);

  response_message = ih_net_hypermessage_create(client_socket,
      IH_NET_HYPERMETHOD_UNKNOWN, IH_NET_HYPERSTATUS_NOT_FOUND,
      IH_NET_HYPERMESSAGE_NO_RESOURCE_PATH, IH_NET_HYPERVERSION_1_0,
      IH_NET_HYPERMESSAGE_NO_HEADERS);
  if (response_message) {
    ih_net_hypermessage_set_body(response_message,
        IH_CORE_CONTENT_MIME_TEXT_HTML, body, body_size);
  } else {
    ih_core_trace("ih_net_hypermessage_create");
  }

  if (!ih_net_server_send_message(ih_net_server, response_message)) {
    ih_core_trace("ih_net_server_send_message");
  }

  return IH_NET_MESSAGE_STATUS_HANDLED;
}

void *ih_net_testhyperengine_create(ih_net_server_t *ih_net_server,
    void *custom_server_object)
{
  assert(ih_net_server);
  assert(custom_server_object);
  ih_net_testhyperengine_t *engine;

  printf("testhyperengine creating\n");

  engine = malloc(sizeof *engine);
  if (engine) {
    engine->ih_net_server = ih_net_server;
    engine->custom_server_object = custom_server_object;
  }

  return engine;
}

void ih_net_testhyperengine_destroy(void *engine_object)
{
  assert(engine_object);

  printf("testhyperengine destroying\n");
  free(engine_object);
}

ih_net_server_handle_message_f ih_net_testhyperengine_get_handler_for_message
(void *engine_object, void *hypermessage_object)
{
  assert(engine_object);
  assert(hypermessage_object);
  ih_net_testhyperengine_t *engine;
  ih_net_server_handle_message_f handler;
  ih_net_hypermessage_t *hypermessage;
  char *resource_name;

  engine = engine_object;
  hypermessage = hypermessage_object;

  resource_name = ih_net_hypermessage_get_resource_name(hypermessage);

  if (0 == strcmp("/", resource_name)) {
    handler = handle_about;

  } else if (0 == strcmp("/about", resource_name)) {
    handler = handle_about;

  } else {
    handler = handle_not_found;

  }

  return handler;
}

void ih_net_testhyperengine_maintain(void *engine_object)
{
  assert(engine_object);
  ih_net_testhyperengine_t *engine;

  engine = engine_object;
}

void ih_net_testhyperengine_run(void *engine_thread_object)
{
  assert(engine_thread_object);
  ih_net_engine_thread_t *engine_thread;
  ih_net_testhyperengine_t *engine;
  ih_net_server_t *ih_net_server;
  unsigned short thread_index;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;
  ih_net_server = engine->ih_net_server;
  thread_index = engine_thread->thread_index;

  ih_net_server_process_messages
    (ih_net_server, IH_NET_ENGINE_HYPER, thread_index);
}

void ih_net_testhyperengine_start(void *engine_thread_object)
{
  assert(engine_thread_object);
  ih_net_engine_thread_t *engine_thread;
  ih_net_testhyperengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("testhyperengine starting\n");
}

void ih_net_testhyperengine_stop(void *engine_thread_object)
{
  assert(engine_thread_object);
  ih_net_engine_thread_t *engine_thread;
  ih_net_testhyperengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("testhyperengine stopping\n");
}
