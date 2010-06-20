#ifndef ih_net_hyperpost_h
#define ih_net_hyperpost_h

#include "ih/core/object.h"
#include "ih/core/types.h"
#include "ih/net/post_stats.h"

struct ih_net_hyperpost_t;
typedef struct ih_net_hyperpost_t ih_net_hyperpost_t;

int ih_net_hyperpost_compare(void *hyperpost_object_a,
    void *hyperpost_object_b);

void *ih_net_hyperpost_create(int socket);

void *ih_net_hyperpost_create_decoy(int socket);

void ih_net_hyperpost_destroy(void *hyperpost_object);

void ih_net_hyperpost_destroy_decoy(void *hyperpost_object);

time_t ih_net_hyperpost_get_last_receive_activity_time(void *hyperpost_object);

int ih_net_hyperpost_get_socket(void *hyperpost_object);

void ih_net_hyperpost_get_stats(void *hyperpost_object,
    ih_net_post_stats_t *post_stats);

ih_core_bool_t ih_net_hyperpost_is_socket_closed(void *hyperpost_object);

void *ih_net_hyperpost_receive_message(void *hyperpost_object);

void ih_net_hyperpost_receive_messages(void *hyperpost_object);

ih_core_bool_t ih_net_hyperpost_send_message(void *hyperpost_object,
    void *hypermessage_object);

void ih_net_hyperpost_send_messages(void *hyperpost_object);

#endif
