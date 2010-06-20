#ifndef ih_net_postey_h
#define ih_net_postey_h

#include "ih/core/object.h"
#include "ih/core/types.h"
#include "ih/external/external.h"
#include "ih/net/post_stats.h"

typedef int (*ih_net_postey_compare_f)(void *a_object, void *b_object);

typedef void *(*ih_net_postey_create_f)(int socket);

typedef void *(*ih_net_postey_create_decoy_f)(int socket);

typedef void (*ih_net_postey_destroy_f)(void *post_object);

typedef void (*ih_net_postey_destroy_decoy_f)(void *post_object);

typedef time_t (*ih_net_postey_get_last_receive_activity_time_f)
(void *post_object);

typedef int (*ih_net_postey_get_socket_f)(void *post_object);

typedef void (*ih_net_postey_get_stats_f)(void *post_object,
    ih_net_post_stats_t *post_stats);

typedef void *(*ih_net_postey_receive_message_f)(void *post_object);

typedef void (*ih_net_postey_receive_messages_f)(void *post_object);

typedef ih_core_bool_t (*ih_net_postey_send_message_f)(void *post_object,
    void *message_object);

typedef void (*ih_net_postey_send_messages_f)(void *post_object);

typedef ih_core_bool_t (*ih_net_postey_socket_closed_f)(void *post_object);

struct ih_net_postey_t {
  ih_net_postey_compare_f compare;
  ih_net_postey_create_f create;
  ih_net_postey_create_decoy_f create_decoy;
  ih_net_postey_destroy_f destroy;
  ih_net_postey_destroy_decoy_f destroy_decoy;

  ih_net_postey_get_last_receive_activity_time_f
  get_last_receive_activity_time;

  ih_net_postey_get_socket_f get_socket;
  ih_net_postey_get_stats_f get_stats;
  ih_net_postey_receive_message_f receive_message;
  ih_net_postey_receive_messages_f receive_messages;
  ih_net_postey_send_message_f send_message;
  ih_net_postey_send_messages_f send_messages;
  ih_net_postey_socket_closed_f socket_closed;
};
typedef struct ih_net_postey_t ih_net_postey_t;

void ih_net_postey_init(ih_net_postey_t *postey, ih_net_postey_compare_f compare,
    ih_net_postey_create_f create, ih_net_postey_create_decoy_f create_decoy,
    ih_net_postey_destroy_f destroy, ih_net_postey_destroy_decoy_f destroy_decoy,

    ih_net_postey_get_last_receive_activity_time_f
    get_last_receive_activity_time,

    ih_net_postey_get_socket_f get_socket, ih_net_postey_get_stats_f get_stats,
    ih_net_postey_receive_message_f receive_message,
    ih_net_postey_receive_messages_f receive_messages,
    ih_net_postey_send_message_f send_message,
    ih_net_postey_send_messages_f send_messages,
    ih_net_postey_socket_closed_f socket_closed);

#endif
