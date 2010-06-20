#include "ih/core/messagey.h"
#include "ih/core/types.h"
#include "ih/external/external.h"

void ih_core_messagey_init(ih_core_messagey_t *messagey,
    ih_core_messagey_destroy_f destroy,
    ih_core_messagey_get_client_socket_f get_client_socket,
    ih_core_messagey_get_engine_id_f get_engine_id,
    ih_core_messagey_get_type_f get_type)
{
  assert(messagey);

  messagey->destroy = destroy;
  messagey->get_client_socket = get_client_socket;
  messagey->get_engine_id = get_engine_id;
  messagey->get_type = get_type;
}
