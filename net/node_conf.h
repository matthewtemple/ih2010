#ifndef ih_net_node_conf_h
#define ih_net_node_conf_h

#include "ih/config/file.h"

struct ih_net_node_conf_t {
  ih_config_file_t *conf;

  char *node_ip;
  unsigned short node_port;
  unsigned short node_max_threads;

  ih_container_list_t *peer_node_ips;
  unsigned short peer_node_port_min;
  unsigned short peer_node_port_max;
};
typedef struct ih_net_node_conf_t ih_net_node_conf_t;

ih_net_node_conf_t *ih_net_node_conf_create(char *conf_filename);

void ih_net_node_conf_destroy(ih_net_node_conf_t *node_conf);

#endif
