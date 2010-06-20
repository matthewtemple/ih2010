#include "ih/audit/log.h"
#include "ih/config/tools.h"
#include "ih/container/list.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/net/enginey.h"
#include "ih/net/hypermessage.h"
#include "ih/net/hyperpost.h"
#include "ih/net/post.h"
#include "ih/net/server.h"
#include "ih/net/testengine.h"
#include "ih/net/testhyperengine.h"
#include "ih/net/testmessage.h"

#define TEST_SERVER_MAX_THREADS 32
#define TEST_HYPERSERVER_MAX_THREADS 32

struct ih_core_test_server_context_t {
  void *server_wide_shared_database;
};
typedef struct ih_core_test_server_context_t ih_core_test_server_context_t;

int main(int argc, char *argv[])
{
  ih_config_disable_if_running_batcih_tests(argc, argv);

  ih_core_test_server_context_t custom_server_context;

  ih_net_server_t *server;
  unsigned short port;
  ih_net_enginey_t enginey;
  ih_core_messagey_t messagey;
  ih_net_postey_t postey;

  ih_net_server_t *hyperserver;
  unsigned short hyperport;
  ih_net_enginey_t hyperenginey;
  ih_core_messagey_t hypermessagey;
  ih_net_postey_t hyperpostey;

  ih_audit_log_t *log;

  log = ih_audit_log_create(stdout);
  if (!log) {
    ih_core_trace_exit("ih_audit_log_create");
  }

  port = 20000;

  ih_net_enginey_init(&enginey, ih_net_testengine_create,
      ih_net_testengine_destroy, ih_net_testengine_get_handler_for_message,
      ih_net_testengine_maintain, ih_net_testengine_run,
      ih_net_testengine_start, ih_net_testengine_stop);

  ih_core_messagey_init(&messagey, ih_core_message_destroy,
      ih_core_message_get_client_socket, ih_core_message_get_engine_id,
      ih_core_message_get_type);

  ih_net_postey_init(&postey, ih_net_post_compare, ih_net_post_create,
      ih_net_post_create_decoy, ih_net_post_destroy, ih_net_post_destroy_decoy,
      ih_net_post_get_last_receive_activity_time, ih_net_post_get_socket,
      ih_net_post_get_stats, ih_net_post_receive_message,
      ih_net_post_receive_messages, ih_net_post_send_message,
      ih_net_post_send_messages, ih_net_post_is_socket_closed);

  server = ih_net_server_create("standard", port, port, TEST_SERVER_MAX_THREADS,
      &messagey, &postey, IH_NET_ENGINE_NO_GET_NAME_FUNCTION,
      IH_NET_SERVER_NO_CONFIG_SYSTEM, log);
  assert(server);

  if (!ih_net_server_register_engine(server, IH_NET_ENGINE_TEST,
          &custom_server_context, &enginey, 2, 2, IH_NET_MAINTAIN_CONSTANTLY,
          IH_NET_TESTMESSAGE_TYPE_COUNT)) {
    ih_core_trace_exit("ih_net_server_register_engine");
  }

  hyperport = 50000;

  ih_net_enginey_init(&hyperenginey, ih_net_testhyperengine_create,
      ih_net_testhyperengine_destroy,
      ih_net_testhyperengine_get_handler_for_message,
      ih_net_testhyperengine_maintain, ih_net_testhyperengine_run,
      ih_net_testhyperengine_start, ih_net_testhyperengine_stop);

  ih_core_messagey_init(&hypermessagey, ih_net_hypermessage_destroy,
      ih_net_hypermessage_get_client_socket, ih_net_hypermessage_get_engine_id,
      ih_net_hypermessage_get_type);

  ih_net_postey_init(&hyperpostey, ih_net_hyperpost_compare,
      ih_net_hyperpost_create, ih_net_hyperpost_create_decoy,
      ih_net_hyperpost_destroy, ih_net_hyperpost_destroy_decoy,
      ih_net_hyperpost_get_last_receive_activity_time,
      ih_net_hyperpost_get_socket, ih_net_hyperpost_get_stats,
      ih_net_hyperpost_receive_message, ih_net_hyperpost_receive_messages,
      ih_net_hyperpost_send_message, ih_net_hyperpost_send_messages,
      ih_net_hyperpost_is_socket_closed);

  hyperserver = ih_net_server_create("http", hyperport, hyperport,
      TEST_HYPERSERVER_MAX_THREADS, &hypermessagey, &hyperpostey,
      IH_NET_ENGINE_NO_GET_NAME_FUNCTION, IH_NET_SERVER_NO_CONFIG_SYSTEM, log);
  assert(hyperserver);

  if (!ih_net_server_start(server)) {
    ih_core_trace_exit("ih_net_server_start");
  }

  if (!ih_net_server_start(hyperserver)) {
    ih_core_trace_exit("ih_net_server_start");
  }

  ih_net_server_destroy(server);
  ih_net_server_destroy(hyperserver);
  ih_audit_log_destroy(log);

  return 0;
}
