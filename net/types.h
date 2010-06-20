#ifndef ih_net_types_h
#define ih_net_types_h

#include "ih/core/object.h"

enum ih_net_maintain_t {
  IH_NET_MAINTAIN_CONSTANTLY = 0,
  IH_NET_MAINTAIN_2_SECONDS = 2,
  IH_NET_MAINTAIN_4_SECONDS = 4,
  IH_NET_MAINTAIN_1_MINUTE = 60,
  IH_NET_MAINTAIN_1_HOUR = 60 * 60,
};
typedef enum ih_net_maintain_t ih_net_maintain_t;

enum ih_net_message_status_t {
  IH_NET_MESSAGE_STATUS_HANDLED,
  IH_NET_MESSAGE_STATUS_CANT_HANDLE,
  IH_NET_MESSAGE_STATUS_CANT_HANDLE_NOW,
};
typedef enum ih_net_message_status_t ih_net_message_status_t;

typedef ih_net_message_status_t (*ih_net_server_handle_message_f)
(void *engine_object, void *message_object);

struct ih_net_server_t;
typedef struct ih_net_server_t ih_net_server_t;

#endif
