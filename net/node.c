#include "ih/core/constants.h"
#include "ih/core/message.h"
#include "ih/core/messagey.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/net/client.h"
#include "ih/net/node.h"
#include "ih/net/post.h"
#include "ih/net/postey.h"
#include "ih/net/server.h"
#include "ih/net/starclient.h"

struct ih_net_node_t {
  ih_net_server_t *server;
  ih_core_messagey_t messagey;
  ih_net_postey_t postey;

  ih_net_starclient_t *starclient;

  pthread_t node_thread;

  ih_audit_log_t *log;
};

static void ih_net_node_create_rollback(ih_net_node_t *node);

static void *node_thread(void *node_object);

ih_net_node_t *ih_net_node_create(char *node_ip, unsigned short node_min_port,
    unsigned short node_max_port, unsigned short node_max_threads,
    ih_container_list_t *peer_node_ips, unsigned short peer_node_port_min,
    unsigned short peer_node_port_max, ih_audit_log_t *log)
{
  assert(peer_node_ips);
  ih_net_node_t *node;
  ih_core_bool_t so_far_so_good;

  node = malloc(sizeof *node);
  if (node) {
    ih_core_messagey_init(&node->messagey, ih_core_message_destroy,
        ih_core_message_get_client_socket,
        ih_core_message_get_engine_id, ih_core_message_get_type);
    ih_net_postey_init(&node->postey, ih_net_post_compare,
        ih_net_post_create, ih_net_post_create_decoy,
        ih_net_post_destroy, ih_net_post_destroy_decoy,
        ih_net_post_get_last_receive_activity_time,
        ih_net_post_get_socket, ih_net_post_get_stats,
        ih_net_post_receive_message, ih_net_post_receive_messages,
        ih_net_post_send_message, ih_net_post_send_messages,
        ih_net_post_is_socket_closed);
    node->log = log;
    so_far_so_good = ih_core_bool_true;
  } else {
    ih_core_trace("malloc");
    so_far_so_good = ih_core_bool_false;
  }

  if (so_far_so_good) {
    node->server = ih_net_server_create("standard", node_min_port,
        node_max_port, node_max_threads, &node->messagey, &node->postey,
        IH_NET_ENGINE_NO_GET_NAME_FUNCTION, IH_NET_SERVER_NO_CONFIG_SYSTEM, log);
    if (node->server) {
      so_far_so_good = ih_core_bool_true;
    } else {
      ih_core_trace_exit("ih_net_server_create failed()");
      so_far_so_good = ih_core_bool_false;
    }
  }

  if (so_far_so_good) {
    node->starclient = ih_net_starclient_create(peer_node_ips,
        peer_node_port_min, peer_node_port_max, node_ip, node_min_port,
        node_max_port, NULL, log);
    if (node->starclient) {
      so_far_so_good = ih_core_bool_true;
    } else {
      ih_core_trace_exit("ih_net_starclient_create() failed()");
      so_far_so_good = ih_core_bool_false;
    }
  }

  if (!so_far_so_good && node) {
    ih_net_node_create_rollback(node);
    node = NULL;
  }

  return node;
}

void ih_net_node_create_rollback(ih_net_node_t *node)
{
  assert(node);

  if (node->server) {
    ih_core_request_stop();
    ih_net_server_destroy(node->server);
  }
  if (node->starclient) {
    ih_net_starclient_destroy(node->starclient);
  }
  free(node);
}

void ih_net_node_destroy(ih_net_node_t *node)
{
  assert(node);

  /*
    TODO: take unprocessed and unsent messages from the server and reroute
    them to another node
  */

  if (0 != pthread_join(node->node_thread, NULL)) {
    ih_core_trace("pthread_join");
  }
  ih_net_server_destroy(node->server);
  ih_net_starclient_destroy(node->starclient);
  free(node);
}

ih_net_server_t *ih_net_node_get_server(ih_net_node_t *node)
{
  return node->server;
}

void ih_net_node_get_server_stats(ih_net_node_t *node,
    ih_net_server_stats_t *server_stats)
{
  ih_net_server_get_stats(node->server, server_stats);
}

ih_net_starclient_t *ih_net_node_get_starclient(ih_net_node_t *node)
{
  return node->starclient;
}

ih_core_bool_t ih_net_node_register_engine(ih_net_node_t *node,
    ih_net_engine_id_t engine_id, void *custom_server_context,
    ih_net_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, ih_net_maintain_t maintain_schedule,
    unsigned long message_type_count)
{
  return ih_net_server_register_engine(node->server, engine_id,
      custom_server_context, enginey, min_run_threads, max_run_threads,
      maintain_schedule, message_type_count);
}

void ih_net_node_register_message_handler(ih_net_node_t *node,
    ih_net_engine_id_t engine_id, unsigned long message_type,
    ih_net_server_handle_message_f message_handler)
{
  ih_net_server_register_message_handler(node->server, engine_id, message_type,
      message_handler);
}

ih_core_bool_t ih_net_node_send_message_to_all_peers(ih_net_node_t *node,
    ih_core_message_t *message)
{
  return ih_net_starclient_send_message_to_all_arms(node->starclient, message);
}

ih_core_bool_t ih_net_node_send_message_to_any_peer(ih_net_node_t *node,
    ih_core_message_t *message)
{
  return ih_net_starclient_send_message_to_any_arm(node->starclient, message);
}

ih_core_bool_t ih_net_node_send_message_to_client(ih_net_node_t *node,
    ih_core_message_t *message)
{
  return ih_net_server_send_message(node->server, message);
}

void ih_net_node_set_server_unresponsive_client_time_seconds(ih_net_node_t *node,
    unsigned long unresponsive_client_time_seconds)
{
  ih_net_server_set_unresponsive_client_time_seconds
    (node->server, unresponsive_client_time_seconds);
}

ih_core_bool_t ih_net_node_start(ih_net_node_t *node)
{
  assert(node);
  ih_core_bool_t success;

  if (ih_net_server_start(node->server)) {
    ih_net_starclient_connect(node->starclient);
    if (0 == pthread_create(&node->node_thread, NULL, node_thread, node)) {
      success = ih_core_bool_true;
    } else {
      ih_core_trace("pthread_create");
      success = ih_core_bool_false;
    }
  } else {
    success = ih_core_bool_false;
    ih_core_request_stop();
  }

  return success;
}

void *node_thread(void *node_object)
{
  assert(node_object);
  ih_net_node_t *node;

  node = node_object;

  while (!ih_core_stop_requested) {
    ih_net_starclient_process_messages(node->starclient);
    usleep(IH_CORE_STANDARD_SLEEP_MICROSECONDS);
  }

  return NULL;
}
