#ifndef ih_net_testmessage_h
#define ih_net_testmessage_h

#include "ih/net/ping.h"

#define IH_NET_TESTMESSAGE_TYPE_COUNT 3
enum ih_net_testmessage_t {
  IH_NET_TESTMESSAGE_UNKNOWN = 0,
  IH_NET_TESTMESSAGE_PING = 1,
  IH_NET_TESTMESSAGE_PONG = 2
};
typedef enum ih_net_testmessage_t ih_net_testmessage_t;

ih_core_message_t *ih_net_testmessage_create(int client_socket,
    unsigned long message_type);

#endif
