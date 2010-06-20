#include "ih/core/message.h"
#include "ih/core/messagey.h"
#include "ih/external/external.h"
#include "ih/pingpong/engine.h"
#include "ih/pingpong/message.h"

ih_core_message_t *ih_pingpong_message_create(int client_socket,
    unsigned long message_type)
{
  ih_core_message_t *message;
  message = ih_core_message_create(client_socket, IH_NET_ENGINE_PING,
      message_type, IH_CORE_MESSAGEY_NULL_DATA, IH_CORE_MESSAGEY_ZERO_DATA_SIZE);
  return message;
}
