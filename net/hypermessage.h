#ifndef ih_net_hypermessage_h
#define ih_net_hypermessage_h

#include "ih/container/set.h"
#include "ih/core/content.h"
#include "ih/core/engine.h"
#include "ih/core/nameobject.h"
#include "ih/core/object.h"
#include "ih/core/uuid.h"
#include "ih/net/hyperversion.h"

#define IH_NET_HYPERMESSAGE_NO_HEADERS NULL
#define IH_NET_HYPERMESSAGE_NO_RESOURCE_PATH NULL

enum ih_net_hypermethod_t {
  IH_NET_HYPERMETHOD_UNKNOWN,
  IH_NET_HYPERMETHOD_GET,
  IH_NET_HYPERMETHOD_HEAD,
  IH_NET_HYPERMETHOD_POST,
};
typedef enum ih_net_hypermethod_t ih_net_hypermethod_t;

enum ih_net_hyperstatus_t {
  IH_NET_HYPERSTATUS_UNKNOWN,
  IH_NET_HYPERSTATUS_OK,
  IH_NET_HYPERSTATUS_NOT_FOUND,
};
typedef enum ih_net_hyperstatus_t ih_net_hyperstatus_t;

struct ih_net_hypermessage_t;
typedef struct ih_net_hypermessage_t ih_net_hypermessage_t;

ih_net_hypermessage_t *ih_net_hypermessage_create(int client_socket,
    ih_net_hypermethod_t hypermethod, ih_net_hyperstatus_t hyperstatus,
    char *resource_path, ih_net_hyperversion_t hyperversion,
    ih_container_set_t *hyperheaders);

void ih_net_hypermessage_destroy(void *hypermessage_object);

char *ih_net_hypermessage_get_body(ih_net_hypermessage_t *hypermessage);

unsigned long ih_net_hypermessage_get_body_size
(ih_net_hypermessage_t *hypermessage);

int ih_net_hypermessage_get_client_socket(void *hypermessage_object);

ih_net_engine_id_t ih_net_hypermessage_get_engine_id(void *message_object);

ih_core_nameobject_t *ih_net_hypermessage_get_hyperheader
(ih_net_hypermessage_t *hypermessage, char *header_name);

ih_container_set_t *ih_net_hypermessage_get_hyperheaders
(ih_net_hypermessage_t *hypermessage);

ih_net_hypermethod_t ih_net_hypermessage_get_hypermethod
(ih_net_hypermessage_t *hypermessage);

ih_net_hyperstatus_t ih_net_hypermessage_get_hyperstatus
(ih_net_hypermessage_t *hypermessage);

ih_net_hyperversion_t ih_net_hypermessage_get_hyperversion
(ih_net_hypermessage_t *hypermessage);

char *ih_net_hypermessage_get_resource_name(ih_net_hypermessage_t *hypermessage);

char *ih_net_hypermessage_get_pri_parameter(ih_net_hypermessage_t *hypermessage,
    char *parameter_name);

double ih_net_hypermessage_get_pri_parameter_as_double
(ih_net_hypermessage_t *hypermessage, char *parameter_name);

ih_core_bool_t ih_net_hypermessage_get_pri_parameter_as_unsigned_long
(ih_net_hypermessage_t *hypermessage, char *parameter_name,
    unsigned long *value);

ih_core_uuid_t *ih_net_hypermessage_get_pri_parameter_as_uuid
(ih_net_hypermessage_t *hypermessage, char *parameter_name);

ih_container_set_t *ih_net_hypermessage_get_pri_parameter_as_uuid_set
(ih_net_hypermessage_t *hypermessage, char *parameter_name);

ih_container_set_t *ih_net_hypermessage_get_pri_parameters
(ih_net_hypermessage_t *hypermessage);

char *ih_net_hypermessage_get_resource_path(ih_net_hypermessage_t *hypermessage);

unsigned long ih_net_hypermessage_get_type(void *message_object);

ih_core_bool_t ih_net_hypermessage_set_body(ih_net_hypermessage_t *hypermessage,
    ih_core_content_t content_type, char *body, unsigned long body_size);

ih_core_bool_t ih_net_hypermessage_set_hyperheader
(ih_net_hypermessage_t *hypermessage, char *header_name, char *header_value);

#endif
