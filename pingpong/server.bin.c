#include "ih/audit/log.h"
#include "ih/container/list.h"
#include "ih/core/messagey.h"
#include "ih/core/tools.h"
#include "ih/core/types.h"
#include "ih/external/external.h"
#include "ih/net/enginey.h"
#include "ih/net/post.h"
#include "ih/net/postey.h"
#include "ih/net/server.h"
#include "ih/pingpong/engine.h"
#include "ih/pingpong/message.h"

#define PING_SERVER_MAX_THREADS 32

struct ping_server_context_t {
  void *server_wide_shared_database;
};
typedef struct ping_server_context_t ping_server_context_t;

static void print_use_exit();

void print_use_exit()
{
  printf("\n    use: ping_server server_port\n\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  ih_net_server_t *server;
  ping_server_context_t custom_server_context;
  unsigned short server_port;
  ih_net_enginey_t enginey;
  ih_core_messagey_t messagey;
  ih_net_postey_t postey;
  ih_audit_log_t *log;

  if (argc < 2) {
    print_use_exit();
  }
  server_port = atoi(argv[1]);

  log = ih_audit_log_create(stdout);
  if (!log) {
    ih_core_trace_exit("ih_audit_log_create");
  }

  ih_net_enginey_init(&enginey, ih_pingpong_engine_create, ih_pingpong_engine_destroy,
      ih_pingpong_engine_get_handler_for_message, ih_pingpong_engine_maintain,
      ih_pingpong_engine_run, ih_pingpong_engine_start, ih_pingpong_engine_stop);

  ih_core_messagey_init(&messagey, ih_core_message_destroy,
      ih_core_message_get_client_socket, ih_core_message_get_engine_id,
      ih_core_message_get_type);

  ih_net_postey_init(&postey, ih_net_post_compare, ih_net_post_create,
      ih_net_post_create_decoy, ih_net_post_destroy, ih_net_post_destroy_decoy,
      ih_net_post_get_last_receive_activity_time, ih_net_post_get_socket,
      ih_net_post_get_stats, ih_net_post_receive_message,
      ih_net_post_receive_messages, ih_net_post_send_message,
      ih_net_post_send_messages, ih_net_post_is_socket_closed);

  server = ih_net_server_create("ping", server_port, server_port,
      PING_SERVER_MAX_THREADS, &messagey, &postey,
      IH_NET_ENGINE_NO_GET_NAME_FUNCTION, IH_NET_SERVER_NO_CONFIG_SYSTEM, log);
  if (!server) {
    ih_core_trace_exit("ih_net_server_create");
  }

  if (!ih_net_server_register_engine(server, IH_NET_ENGINE_PING,
          &custom_server_context, &enginey, 2, 2, IH_NET_MAINTAIN_1_MINUTE,
          IH_PINGPONG_MESSAGE_TYPE_COUNT)) {
    ih_core_trace_exit("ih_net_server_register_engine");
  }

  if (!ih_net_server_start(server)) {
    ih_core_trace_exit("ih_net_server_start");
  }

  ih_net_server_destroy(server);
  ih_audit_log_destroy(log);

  return 0;
}
