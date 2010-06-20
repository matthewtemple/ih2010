#ifndef ih_net_post_h
#define ih_net_post_h

#include "ih/container/list.h"
#include "ih/core/message.h"
#include "ih/core/types.h"
#include "ih/external/external.h"
#include "ih/net/post_stats.h"

struct ih_net_post_t;
typedef struct ih_net_post_t ih_net_post_t;

int ih_net_post_compare(void *post_object_a, void *post_object_b);

void *ih_net_post_create(int socket);

void *ih_net_post_create_decoy(int socket);

void ih_net_post_destroy(void *post_object);

void ih_net_post_destroy_decoy(void *post_object);

time_t ih_net_post_get_last_receive_activity_time(void *post_object);

int ih_net_post_get_socket(void *post_object);

void ih_net_post_get_stats(void *post_object, ih_net_post_stats_t *post_stats);

ih_core_bool_t ih_net_post_is_socket_closed(void *post_object);

void *ih_net_post_receive_message(void *post_object);

void ih_net_post_receive_messages(void *post_object);

ih_core_bool_t ih_net_post_send_message(void *post_object, void *message_object);

void ih_net_post_send_messages(void *post_object);

ih_container_list_t *ih_net_post_take_unsent_messages(ih_net_post_t *post);

#endif
