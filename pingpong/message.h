#ifndef ih_pingpong_message_h
#define ih_pingpong_message_h

#define IH_PINGPONG_MESSAGE_TYPE_COUNT 3
enum ih_pingpong_message_t {
  IH_PINGPONG_MESSAGE_UNKNOWN = 0,
  IH_PINGPONG_MESSAGE_PING = 1,
  IH_PINGPONG_MESSAGE_PONG = 2
};
typedef enum ih_pingpong_message_t ih_pingpong_message_t;

ih_core_message_t *ih_pingpong_message_create(int client_socket,
    unsigned long message_type);

#endif
