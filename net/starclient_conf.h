#ifndef ih_net_starclient_conf_h
#define ih_net_starclient_conf_h

#include "ih/config/file.h"

struct ih_net_starclient_conf_t {
  ih_config_file_t *conf;

  ih_container_list_t *star_arm_ips;
  unsigned short star_arm_port_min;
  unsigned short star_arm_port_max;

  char *node_server_exclude_ip;
  unsigned short node_server_exclude_port;
};
typedef struct ih_net_starclient_conf_t ih_net_starclient_conf_t;

ih_net_starclient_conf_t *ih_net_starclient_conf_create(char *conf_filename);

void ih_net_starclient_conf_destroy(ih_net_starclient_conf_t *starclient_conf);

#endif
