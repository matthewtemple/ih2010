#ifndef ih_net_starclient_h
#define ih_net_starclient_h

#include "ih/audit/log.h"
#include "ih/core/engine.h"
#include "ih/net/client.h"
#include "ih/net/starclient_stats.h"

struct ih_net_starclient_t;
typedef struct ih_net_starclient_t ih_net_starclient_t;

ih_core_bool_t ih_net_starclient_connect(ih_net_starclient_t *starclient);

ih_net_starclient_t *ih_net_starclient_create(ih_container_list_t *star_arm_ips,
    unsigned short star_arm_port_min, unsigned short star_arm_port_max,
    char *node_server_exclude_ip, unsigned short node_server_exclude_min_port,
    unsigned short node_server_exclude_max_port, void *custom_client_context,
    ih_audit_log_t *log);

void ih_net_starclient_destroy(ih_net_starclient_t *starclient);

ih_net_client_t *ih_net_starclient_get_client(ih_net_starclient_t *starclient,
    int socket);

void ih_net_starclient_get_stats(ih_net_starclient_t *starclient,
    ih_net_starclient_stats_t *starclient_stats);

void ih_net_starclient_process_messages(ih_net_starclient_t *starclient);

ih_core_bool_t ih_net_starclient_register_engine(ih_net_starclient_t *starclient,
    ih_net_engine_id_t engine_id, unsigned long message_type_count);

void ih_net_starclient_register_message_handler(ih_net_starclient_t *starclient,
    ih_net_engine_id_t engine_id, unsigned long message_type,
    ih_net_client_handle_message_f message_handler);

ih_core_bool_t ih_net_starclient_send_message_to_any_arm
(ih_net_starclient_t *starclient, ih_core_message_t *message);

ih_core_bool_t ih_net_starclient_send_message_to_all_arms
(ih_net_starclient_t *starclient, ih_core_message_t *message);

void ih_net_starclient_set_unsent_messages_queue_size
(ih_net_starclient_t *starclient, unsigned long queue_size);

ih_core_bool_t ih_net_starclient_star_available(ih_net_starclient_t *starclient);

#endif
