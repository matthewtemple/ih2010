#include "ih/container/list.h"
#include "ih/container/set.h"
#include "ih/core/content.h"
#include "ih/core/messagey.h"
#include "ih/core/nameobject.h"
#include "ih/core/string.h"
#include "ih/core/tools.h"
#include "ih/core/types.h"
#include "ih/external/external.h"
#include "ih/net/hypermessage.h"

struct ih_net_hypermessage_t {
  int client_socket;

  ih_net_hypermethod_t hypermethod;
  ih_net_hyperstatus_t hyperstatus;
  char *resource_path;
  ih_net_hyperversion_t hyperversion;
  ih_container_set_t *hyperheaders;
  char *body;
  unsigned long body_size;

  ih_core_content_t content_type;
  char *resource_name;
  ih_container_set_t *pri_parameters;
};

static ih_core_bool_t ih_net_hypermessage_create_pri
(ih_net_hypermessage_t *hypermessage);

static void ih_net_hypermessage_create_rollback
(ih_net_hypermessage_t *hypermessage);

/*
  TODO: simplify
*/
ih_net_hypermessage_t *ih_net_hypermessage_create(int client_socket,
    ih_net_hypermethod_t hypermethod, ih_net_hyperstatus_t hyperstatus,
    char *resource_path, ih_net_hyperversion_t hyperversion,
    ih_container_set_t *hyperheaders)
{
  ih_net_hypermessage_t *hypermessage;
  ih_core_bool_t so_far_so_good;

  hypermessage = malloc(sizeof *hypermessage);
  if (hypermessage) {
    hypermessage->client_socket = client_socket;
    hypermessage->hypermethod = hypermethod;
    hypermessage->hyperstatus = hyperstatus;
    hypermessage->hyperversion = hyperversion;
    hypermessage->body = NULL;
    hypermessage->body_size = 0;
    hypermessage->resource_name = NULL;
    hypermessage->pri_parameters = NULL;

    if (resource_path) {
      hypermessage->resource_path = strdup(resource_path);
      if (hypermessage->resource_path) {
        so_far_so_good = ih_core_bool_true;
      } else {
        ih_core_trace("strdup");
        so_far_so_good = ih_core_bool_false;
      }
    } else {
      hypermessage->resource_path = NULL;
      so_far_so_good = ih_core_bool_true;
    }

  } else {
    so_far_so_good = ih_core_bool_false;
    ih_core_trace("malloc");
  }

  if (so_far_so_good) {
    if (hyperheaders) {
      hypermessage->hyperheaders = ih_container_set_copy(hyperheaders);
    } else {
      hypermessage->hyperheaders
        = ih_container_set_create(ih_core_nameobject_compare,
            ih_core_nameobject_copy, ih_core_nameobject_destroy);
      if (!hypermessage->hyperheaders) {
        so_far_so_good = ih_core_bool_false;
        ih_core_trace("ih_container_set_create");
      }
    }
  }

  if (so_far_so_good) {
    if (hypermessage->resource_path) {
      if (!ih_net_hypermessage_create_pri(hypermessage)) {
        so_far_so_good = ih_core_bool_false;
      }
    }
  }

  if (!so_far_so_good && hypermessage) {
    ih_net_hypermessage_create_rollback(hypermessage);
    hypermessage = NULL;
  }

  assert(!hypermessage || hypermessage->hyperheaders);
  return hypermessage;
}

ih_core_bool_t ih_net_hypermessage_create_pri(ih_net_hypermessage_t *hypermessage)
{
  assert(hypermessage->resource_path);
  ih_core_bool_t success;
  char *parameters;
  char *parameter;
  char *name;
  char *value;
  char *parameter_context;
  char *nameobject_context;
  ih_core_nameobject_t *nameobject;
  char *resource_name;

  nameobject_context = NULL;

  resource_name = strtok(hypermessage->resource_path, "?");
  hypermessage->resource_name = strdup(resource_name);
  parameters = strtok(NULL, "?");
  if (parameters) {
    hypermessage->pri_parameters = ih_container_set_create
      (ih_core_nameobject_compare, ih_core_nameobject_copy,
          ih_core_nameobject_destroy);
    if (hypermessage->pri_parameters) {
      success = ih_core_bool_true;
      parameter = strtok_r(parameters, "&", &parameter_context);
      while (parameter) {
        name = strtok_r(parameter, "=", &nameobject_context);
        value = strtok_r(NULL, "=", &nameobject_context);
        if (name && value) {
          nameobject = ih_core_nameobject_create(name, value,
              ih_core_string_copy, ih_core_string_destroy,
              ih_core_string_get_as_string);
          if (nameobject) {
            if (!ih_container_set_add
                (hypermessage->pri_parameters, nameobject)) {
              ih_core_nameobject_destroy(nameobject);
            }
          } else {
            ih_core_trace("ih_core_nameobject_create");
            success = ih_core_bool_false;
          }
        }
        parameter = strtok_r(NULL, "&", &parameter_context);
      }
    } else {
      ih_core_trace("ih_container_set_create");
      success = ih_core_bool_false;
    }
  } else {
    hypermessage->pri_parameters = NULL;
    success = ih_core_bool_true;
  }

  return success;
}

void ih_net_hypermessage_create_rollback(ih_net_hypermessage_t *hypermessage)
{
  assert(hypermessage);

  if (hypermessage->resource_path) {
    free(hypermessage->resource_path);
  }
  if (hypermessage->hyperheaders) {
    ih_container_set_destroy(hypermessage->hyperheaders);
  }
  if (hypermessage->body) {
    free(hypermessage->body);
  }
  free(hypermessage);
}

void ih_net_hypermessage_destroy(void *hypermessage_object)
{
  assert(hypermessage_object);
  ih_net_hypermessage_t *hypermessage;

  hypermessage = hypermessage_object;

  if (hypermessage->body) {
    free(hypermessage->body);
  }

  if (hypermessage->resource_path) {
    free(hypermessage->resource_path);
  }

  if (hypermessage->resource_name) {
    free(hypermessage->resource_name);
  }

  if (hypermessage->pri_parameters) {
    ih_container_set_destroy(hypermessage->pri_parameters);
  }

  ih_container_set_destroy(hypermessage->hyperheaders);

  free(hypermessage);
}

char *ih_net_hypermessage_get_body(ih_net_hypermessage_t *hypermessage)
{
  return hypermessage->body;
}

unsigned long ih_net_hypermessage_get_body_size
(ih_net_hypermessage_t *hypermessage)
{
  return hypermessage->body_size;
}

int ih_net_hypermessage_get_client_socket(void *hypermessage_object)
{
  assert(hypermessage_object);
  ih_net_hypermessage_t *hypermessage;

  hypermessage = hypermessage_object;

  return hypermessage->client_socket;
}

ih_net_engine_id_t ih_net_hypermessage_get_engine_id
(void *message_object)
{
  return IH_NET_ENGINE_HYPER;
}

ih_core_nameobject_t *ih_net_hypermessage_get_hyperheader
(ih_net_hypermessage_t *hypermessage, char *header_name)
{
  assert(hypermessage);
  assert(header_name);
  ih_core_nameobject_t *decoy_header;
  ih_core_nameobject_t *found_header;

  decoy_header = ih_core_nameobject_create_decoy(header_name);
  found_header = ih_container_set_find
    (hypermessage->hyperheaders, decoy_header);

  return found_header;
}

ih_container_set_t *ih_net_hypermessage_get_hyperheaders
(ih_net_hypermessage_t *hypermessage)
{
  return hypermessage->hyperheaders;
}

ih_net_hypermethod_t ih_net_hypermessage_get_hypermethod
(ih_net_hypermessage_t *hypermessage)
{
  return hypermessage->hypermethod;
}

ih_net_hyperstatus_t ih_net_hypermessage_get_hyperstatus
(ih_net_hypermessage_t *hypermessage)
{
  return hypermessage->hyperstatus;
}

ih_net_hyperversion_t ih_net_hypermessage_get_hyperversion
(ih_net_hypermessage_t *hypermessage)
{
  return hypermessage->hyperversion;
}

char *ih_net_hypermessage_get_resource_name(ih_net_hypermessage_t *hypermessage)
{
  return hypermessage->resource_name;
}

char *ih_net_hypermessage_get_pri_parameter(ih_net_hypermessage_t *hypermessage,
    char *parameter_name)
{
  char *parameter_value;
  ih_core_nameobject_t *nameobject_decoy;
  ih_core_nameobject_t *nameobject;

  if (hypermessage->pri_parameters) {
    nameobject_decoy = ih_core_nameobject_create_decoy(parameter_name);
    if (nameobject_decoy) {
      nameobject
        = ih_container_set_find(hypermessage->pri_parameters, nameobject_decoy);
      if (nameobject) {
        parameter_value = ih_core_nameobject_get_object(nameobject);
      } else {
        parameter_value = NULL;
      }
      ih_core_nameobject_destroy_decoy(nameobject_decoy);
    } else {
      ih_core_trace("ih_core_nameobject_create_decoy");
      parameter_value = NULL;
    }
  } else {
    parameter_value = NULL;
  }

  return parameter_value;
}

double ih_net_hypermessage_get_pri_parameter_as_double
(ih_net_hypermessage_t *hypermessage, char *parameter_name)
{
  assert(hypermessage);
  assert(parameter_name);
  char *value_string;
  double value_double;

  value_double = 0.0;

  value_string
    = ih_net_hypermessage_get_pri_parameter(hypermessage, parameter_name);
  if (value_string) {
    value_double = atof(value_string);
    free(value_string);
  } else {
    ih_core_trace("ih_net_hypermessage_get_pri_parameter");
  }

  return value_double;
}

ih_core_bool_t ih_net_hypermessage_get_pri_parameter_as_unsigned_long
(ih_net_hypermessage_t *hypermessage, char *parameter_name,
    unsigned long *value)
{
  assert(hypermessage);
  assert(parameter_name);
  assert(value);
  char *value_string;
  ih_core_bool_t success;

  value_string
    = ih_net_hypermessage_get_pri_parameter(hypermessage, parameter_name);
  if (value_string) {
    success = ih_core_bool_true;
    *value = atol(value_string);
  } else {
    success = ih_core_bool_false;
    ih_core_trace("ih_net_hypermessage_get_pri_parameter");
  }

  return success;
}

ih_core_uuid_t *ih_net_hypermessage_get_pri_parameter_as_uuid
(ih_net_hypermessage_t *hypermessage, char *parameter_name)
{
  assert(hypermessage);
  assert(parameter_name);
  ih_core_uuid_t *uuid;
  char *uuid_string;

  uuid = NULL;

  uuid_string
    = ih_net_hypermessage_get_pri_parameter(hypermessage, parameter_name);
  if (uuid_string) {
    uuid = ih_core_uuid_create_from_string(uuid_string);
    if (!uuid) {
      ih_core_trace("ih_core_uuid_create_from_string");
    }
  } else {
    ih_core_trace("ih_net_hypermessage_get_pri_parameter");
  }

  return uuid;
}

ih_container_set_t *ih_net_hypermessage_get_pri_parameter_as_uuid_set
(ih_net_hypermessage_t *hypermessage, char *parameter_name)
{
  assert(hypermessage);
  assert(parameter_name);
  ih_core_uuid_t *uuid;
  char *uuid_string;
  char *parameter_string;
  ih_container_list_t *uuid_strings;
  ih_container_set_t *uuid_set;

  uuid_set = ih_container_set_create
    (ih_core_uuid_compare, ih_core_uuid_copy, ih_core_uuid_destroy);
  if (uuid_set) {
    parameter_string
      = ih_net_hypermessage_get_pri_parameter(hypermessage, parameter_name);
    if (parameter_string) {
      uuid_strings
        = ih_container_list_create_strings_from_string(parameter_string, ",");
      if (uuid_strings) {
        ih_container_list_iterate_start(uuid_strings);
        while ((uuid_string = ih_container_list_iterate_next(uuid_strings))) {
          uuid = ih_core_uuid_create_from_string(uuid_string);
          if (uuid) {
            if (!ih_container_set_find(uuid_set, uuid)) {
              if (!ih_container_set_add(uuid_set, uuid)) {
                ih_core_uuid_destroy(uuid);
                ih_core_trace("ih_container_set_add");
              }
            } else {
              ih_core_uuid_destroy(uuid);
            }
          } else {
            ih_core_trace("ih_core_uuid_create_from_string");
          }
        }
        ih_container_list_destroy(uuid_strings);
      } else {
        ih_core_trace("ih_container_list_create_string_from_strings");
      }
    } else {
      ih_core_trace("ih_net_hypermessage_get_pri_parameter");
    }
  } else {
    ih_core_trace("ih_container_set_create");
  }

  return uuid_set;
}

ih_container_set_t *ih_net_hypermessage_get_pri_parameters
(ih_net_hypermessage_t *hypermessage)
{
  return hypermessage->pri_parameters;
}

char *ih_net_hypermessage_get_resource_path(ih_net_hypermessage_t *hypermessage)
{
  return hypermessage->resource_path;
}

unsigned long ih_net_hypermessage_get_type(void *message_object)
{
  return IH_CORE_MESSAGEY_NO_TYPE;
}

ih_core_bool_t ih_net_hypermessage_set_body(ih_net_hypermessage_t *hypermessage,
    ih_core_content_t content_type, char *body, unsigned long body_size)
{
  assert(hypermessage);
  assert(body);
  assert(body_size > 0);
  ih_core_bool_t success;
  char *content_type_name;
  char *body_size_string;

  content_type_name = ih_core_content_get_name(content_type);
  success = ih_core_bool_true;

  if (!ih_net_hypermessage_set_hyperheader
      (hypermessage, "Content-Type", content_type_name)) {
    success = ih_core_bool_false;
  }

  if (asprintf(&body_size_string, "%lu", body_size) != -1) {
    if (!ih_net_hypermessage_set_hyperheader
        (hypermessage, "Content-Length", body_size_string)) {
      success = ih_core_bool_false;
    }
    free(body_size_string);
  } else {
    ih_core_trace("asprintf");
  }

  if (success) {
    hypermessage->body = malloc(body_size);
    if (hypermessage->body) {
      memcpy(hypermessage->body, body, body_size);
      hypermessage->body_size = body_size;
    } else {
      ih_core_trace("malloc");
      success = ih_core_bool_false;
    }
  }

  return success;
}

ih_core_bool_t ih_net_hypermessage_set_hyperheader
(ih_net_hypermessage_t *hypermessage, char *header_name, char *header_value)
{
  assert(hypermessage);
  assert(header_name);
  assert(header_value);
  ih_core_bool_t success;
  ih_core_nameobject_t *nameobject;

  nameobject = ih_core_nameobject_create(header_name, header_value,
      ih_core_string_copy, ih_core_string_destroy, ih_core_string_get_as_string);
  if (nameobject) {
    if (ih_container_set_add(hypermessage->hyperheaders, nameobject)) {
      success = ih_core_bool_true;
    } else {
      success = ih_core_bool_false;
    }
  } else {
    success = ih_core_bool_false;
  }

  return success;
}
