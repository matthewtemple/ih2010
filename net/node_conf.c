#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/net/node_conf.h"

#define DEFAULT_NODE_IP "127.0.0.1"
#define DEFAULT_MAX_THREADS 2
#define DEFAULT_NODE_PORT 67
#define DEFAULT_PEER_NODE_PORT_MIN 8000
#define DEFAULT_PEER_NODE_PORT_MAX 9000

static ih_core_bool_t ih_net_node_conf_create_node_ip
(ih_net_node_conf_t *node_conf);

static ih_core_bool_t ih_net_node_conf_create_node_max_threads
(ih_net_node_conf_t *node_conf);

static ih_core_bool_t ih_net_node_conf_create_node_port
(ih_net_node_conf_t *node_conf);

static ih_core_bool_t ih_net_node_conf_create_peer_node_ips
(ih_net_node_conf_t *node_conf);

static ih_core_bool_t ih_net_node_conf_create_peer_node_port_range
(ih_net_node_conf_t *node_conf);

ih_net_node_conf_t *ih_net_node_conf_create(char *conf_filename)
{
  assert(conf_filename);
  ih_net_node_conf_t *node_conf;
  ih_core_bool_t so_far_so_good;

  node_conf = malloc(sizeof *node_conf);
  if (node_conf) {
    node_conf->conf = ih_config_file_create(conf_filename);
    if (node_conf->conf) {
      so_far_so_good = ih_core_bool_true;
    } else {
      so_far_so_good = ih_core_bool_false;
      ih_core_trace("new");
    }
  } else {
    ih_core_trace("malloc");
    so_far_so_good = ih_core_bool_false;
  }

  if (so_far_so_good) {
    so_far_so_good = ih_net_node_conf_create_node_ip(node_conf);
  }

  if (so_far_so_good) {
    so_far_so_good = ih_net_node_conf_create_node_max_threads(node_conf);
  }

  if (so_far_so_good) {
    so_far_so_good = ih_net_node_conf_create_node_port(node_conf);
  }

  if (so_far_so_good) {
    so_far_so_good = ih_net_node_conf_create_peer_node_ips(node_conf);
  }

  if (so_far_so_good) {
    so_far_so_good = ih_net_node_conf_create_peer_node_port_range(node_conf);
  }

  if (!so_far_so_good && node_conf) {
    if (node_conf->conf) {
      ih_config_file_destroy(node_conf->conf);
    }
    free(node_conf);
    node_conf = NULL;
  }

  return node_conf;
}

ih_core_bool_t ih_net_node_conf_create_node_ip(ih_net_node_conf_t *node_conf)
{
  assert(node_conf);
  ih_core_bool_t success;

  if (ih_config_file_find_as_string
      (node_conf->conf, "node-ip", &node_conf->node_ip, DEFAULT_NODE_IP)) {
    success = ih_core_bool_true;
  } else {
    printf("node conf doesn't specify node-ip\n");
    success = ih_core_bool_false;
  }

  return success;
}

ih_core_bool_t ih_net_node_conf_create_node_max_threads
(ih_net_node_conf_t *node_conf)
{
  assert(node_conf);
  ih_core_bool_t success;

  success = ih_config_file_find_as_unsigned_short(node_conf->conf,
      "node-max-threads", &node_conf->node_max_threads, DEFAULT_MAX_THREADS);
  if (success) {
    success = ih_core_bool_true;
  } else {
    printf("node conf doesn't specify node-max-threads\n");
    success = ih_core_bool_false;
  }

  return success;
}

ih_core_bool_t ih_net_node_conf_create_node_port(ih_net_node_conf_t *node_conf)
{
  assert(node_conf);
  ih_core_bool_t success;

  if (ih_config_file_find_as_unsigned_short(node_conf->conf, "node-port",
          &node_conf->node_port, DEFAULT_NODE_PORT)) {
    success = ih_core_bool_true;
  } else {
    printf("node conf doesn't specify node-port\n");
    success = ih_core_bool_false;
  }

  return success;
}

ih_core_bool_t ih_net_node_conf_create_peer_node_ips
(ih_net_node_conf_t *node_conf)
{
  assert(node_conf);
  ih_core_bool_t success;

  if (ih_config_file_find_list_as_strings
      (node_conf->conf, "peer-node-ips[list]", &node_conf->peer_node_ips)) {
    success = ih_core_bool_true;
  } else {
    printf("node conf doesn't specify peer-node-ip[list]\n");
    success = ih_core_bool_false;
  }

  return success;
}

ih_core_bool_t ih_net_node_conf_create_peer_node_port_range
(ih_net_node_conf_t *node_conf)
{
  assert(node_conf);
  ih_core_bool_t success;
  ih_core_bool_t found_peer_node_port_min;
  ih_core_bool_t found_peer_node_port_max;

  found_peer_node_port_min = ih_config_file_find_as_unsigned_short
    (node_conf->conf, "peer-node-port-min", &node_conf->peer_node_port_min,
        DEFAULT_PEER_NODE_PORT_MIN);
  found_peer_node_port_max = ih_config_file_find_as_unsigned_short
    (node_conf->conf, "peer-node-port-max", &node_conf->peer_node_port_max,
        DEFAULT_PEER_NODE_PORT_MAX);
  if (found_peer_node_port_min && found_peer_node_port_max) {
    success = ih_core_bool_true;
  } else {
    printf("node conf doesn't specify both peer-node-port-min and "
        "peer-node-port-max\n");
    success = ih_core_bool_false;
  }

  return success;
}

void ih_net_node_conf_destroy(ih_net_node_conf_t *node_conf)
{
  assert(node_conf);
  ih_config_file_destroy(node_conf->conf);
  free(node_conf);
}
