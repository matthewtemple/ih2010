#ifndef ih_net_client_h
#define ih_net_client_h

#include "ih/audit/log.h"
#include "ih/container/list.h"
#include "ih/core/message.h"
#include "ih/core/messagey.h"
#include "ih/net/postey.h"

struct ih_net_client_t;
typedef struct ih_net_client_t ih_net_client_t;

typedef void (*ih_net_client_handle_message_f)(void *custom_client_context,
    ih_core_message_t *message);

void *ih_net_client_copy(void *client_object);

ih_net_client_t *ih_net_client_create(const char *server_ip_address,
    unsigned short server_min_port, unsigned short server_max_port,
    ih_net_engine_get_name_f get_engine_name, void *custom_client_context,
    ih_audit_log_t *log);

void ih_net_client_destroy(void *client_object);

char *ih_net_client_get_as_string(void *client_object);

char *ih_net_client_get_server_ip_address(ih_net_client_t *client);

unsigned short ih_net_client_get_server_port(ih_net_client_t *client);

int ih_net_client_get_socket(ih_net_client_t *client);

ih_core_bool_t ih_net_client_is_connected_to_server(ih_net_client_t *client);

void ih_net_client_print(void *client_object);

void ih_net_client_process_messages(ih_net_client_t *client);

ih_core_bool_t ih_net_client_register_engine(ih_net_client_t *client,
    ih_net_engine_id_t engine_id, unsigned long message_type_count);

void ih_net_client_register_message_handler(ih_net_client_t *client,
    ih_net_engine_id_t engine_id, unsigned long message_type,
    ih_net_client_handle_message_f message_handler);

ih_core_bool_t ih_net_client_send_message(ih_net_client_t *client,
    ih_core_message_t *message);

ih_container_list_t *ih_net_client_take_unsent_messages(ih_net_client_t *client);

#endif
