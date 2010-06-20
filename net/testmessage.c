#include "ih/core/engine.h"
#include "ih/core/message.h"
#include "ih/core/messagey.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/net/testengine.h"
#include "ih/net/testmessage.h"

ih_core_message_t *ih_net_testmessage_create(int client_socket,
    unsigned long message_type)
{
  return ih_core_message_create(client_socket, IH_NET_ENGINE_TEST, message_type,
      IH_CORE_MESSAGEY_NULL_DATA, IH_CORE_MESSAGEY_ZERO_DATA_SIZE);
}
