#ifndef ih_core_messagey_h
#define ih_core_messagey_h

#include "ih/core/engine.h"
#include "ih/core/object.h"

#define IH_CORE_MESSAGEY_NO_CLIENT_SOCKET -1
#define IH_CORE_MESSAGEY_NO_TYPE 0
#define IH_CORE_MESSAGEY_NULL_DATA NULL
#define IH_CORE_MESSAGEY_ZERO_DATA_SIZE 0

typedef void (*ih_core_messagey_destroy_f)(void *message_object);

typedef int (*ih_core_messagey_get_client_socket_f)(void *message_object);

typedef ih_net_engine_id_t (*ih_core_messagey_get_engine_id_f)
(void *message_object);

typedef unsigned long (*ih_core_messagey_get_type_f)(void *message_object);

struct ih_core_messagey_t {
  ih_core_messagey_destroy_f destroy;
  ih_core_messagey_get_client_socket_f get_client_socket;
  ih_core_messagey_get_engine_id_f get_engine_id;
  ih_core_messagey_get_type_f get_type;
};
typedef struct ih_core_messagey_t ih_core_messagey_t;

void ih_core_messagey_init(ih_core_messagey_t *messagey,
    ih_core_messagey_destroy_f destroy,
    ih_core_messagey_get_client_socket_f get_client_socket,
    ih_core_messagey_get_engine_id_f get_engine_id,
    ih_core_messagey_get_type_f get_type);

#endif
