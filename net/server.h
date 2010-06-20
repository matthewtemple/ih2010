#ifndef ih_net_server_h
#define ih_net_server_h

#include "ih/audit/log.h"
#include "ih/config/system.h"
#include "ih/container/list.h"
#include "ih/core/engine.h"
#include "ih/core/message.h"
#include "ih/core/messagey.h"
#include "ih/core/types.h"
#include "ih/net/enginey.h"
#include "ih/net/ip_address.h"
#include "ih/net/postey.h"
#include "ih/net/server_stats.h"

#define IH_NET_SERVER_MAX_ENGINES 64
#define IH_NET_SERVER_NO_MAINTAIN_FUNCTION NULL
#define IH_NET_SERVER_NO_CONFIG_SYSTEM NULL
#define IH_NET_SERVER_DONT_CLOSE_UNRESPONSIVE_CLIENTS 0
#define IH_NET_MESSAGE_TYPE_COUNT_NONE 0

ih_net_server_t *ih_net_server_create(const char *name, unsigned short min_port,
    unsigned short max_port, unsigned short max_threads,
    ih_core_messagey_t *messagey, ih_net_postey_t *postey,
    ih_net_engine_get_name_f get_engine_name, ih_config_system_t *config_system,
    ih_audit_log_t *log);

void ih_net_server_destroy(ih_net_server_t *server);

ih_core_bool_t ih_net_server_engine_run_thread_quiesce_requested
(ih_net_server_t *server, ih_net_engine_id_t engine_id,
    unsigned short thread_index);

ih_net_server_handle_message_f ih_net_server_get_handler_for_message
(ih_net_server_t *server, void *message_object);

ih_config_system_t *ih_net_server_get_config_system(ih_net_server_t *server);

void ih_net_server_get_stats(ih_net_server_t *server,
    ih_net_server_stats_t *server_stats);

void ih_net_server_print_stats(ih_net_server_t *server);

void ih_net_server_process_messages(ih_net_server_t *server,
    ih_net_engine_id_t engine_id, unsigned short thread_index);

ih_core_bool_t ih_net_server_register_engine(ih_net_server_t *server,
    ih_net_engine_id_t engine_id, void *custom_server_context,
    ih_net_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, ih_net_maintain_t maintain_schedule,
    unsigned long message_type_count);

void ih_net_server_register_message_handler(ih_net_server_t *server,
    ih_net_engine_id_t engine_id, unsigned long message_type,
    ih_net_server_handle_message_f message_handler);

ih_core_bool_t ih_net_server_send_message(ih_net_server_t *server,
    void *message_object);

void ih_net_server_set_unresponsive_client_time_seconds
(ih_net_server_t *server, unsigned long unresponsive_client_time_seconds);

ih_core_bool_t ih_net_server_start(ih_net_server_t *server);

ih_container_list_t *ih_net_server_take_unprocessed_messages
(ih_net_server_t *server);

ih_container_list_t *ih_net_server_take_unsent_messages(ih_net_server_t *server);

#endif
