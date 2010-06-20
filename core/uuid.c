#include "ih/core/bool.h"
#include "ih/core/tools.h"
#include "ih/core/uuid.h"
#include "ih/external/external.h"

#define NULL_UUID "a8fffbb4-675c-4896-81f0-6eccf8bd8760"

struct ih_core_uuid_t {
  uuid_t uuid;
  char *string;
};

ih_core_bool_t ih_core_uuid_add_to_message(void *uuid_object,
    ih_core_message_t *message)
{
  assert(uuid_object);
  assert(message);
  ih_core_bool_t success;
  char *uuid_string;
  ih_core_uuid_t *uuid;

  uuid = uuid_object;

  uuid_string = ih_core_uuid_get_string(uuid);
  if (ih_core_message_add_string(message, uuid_string)) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
    ih_core_trace("ih_core_message_add_string");
  }

  return success;
}

int ih_core_uuid_compare(void *uuid_object_a,
    void *uuid_object_b)
{
  assert(uuid_object_a);
  assert(uuid_object_b);
  ih_core_uuid_t *uuid_a;
  ih_core_uuid_t *uuid_b;

  uuid_a = uuid_object_a;
  uuid_b = uuid_object_b;

  return uuid_compare(uuid_a->uuid, uuid_b->uuid);
}

void *ih_core_uuid_copy(void *uuid_object)
{
  assert(uuid_object);
  ih_core_uuid_t *original;
  ih_core_uuid_t *copy;

  original = uuid_object;

  copy = malloc(sizeof *copy);
  if (copy) {
    copy->string = NULL;
    uuid_copy(copy->uuid, original->uuid);
  } else {
    ih_core_trace("malloc");
  }

  return copy;
}

ih_core_uuid_t *ih_core_uuid_create()
{
  ih_core_uuid_t *uuid;

  uuid = malloc(sizeof *uuid);
  if (uuid) {
    uuid_generate(uuid->uuid);
    uuid->string = NULL;
  } else {
    ih_core_trace("malloc");
  }

  return uuid;
}

void *ih_core_uuid_create_from_message(ih_core_message_t *message)
{
  assert(message);
  ih_core_uuid_t *uuid;
  char *uuid_string;

  uuid_string = ih_core_message_take_string(message);
  if (uuid_string) {
    uuid = ih_core_uuid_create_from_string(uuid_string);
    if (!uuid) {
      ih_core_trace("ih_core_uuid_create_from_string");
    }
    free(uuid_string);
  } else {
    ih_core_trace("ih_core_message_take_string");
    uuid = NULL;
  }

  return uuid;
}

ih_core_uuid_t *ih_core_uuid_create_from_string(const char *uuid_string)
{
  ih_core_uuid_t *uuid;

  uuid = malloc(sizeof *uuid);
  if (uuid) {
    uuid->string = NULL;
    if (0 != uuid_parse(uuid_string, uuid->uuid)) {
      free(uuid);
      uuid = NULL;
    }
  } else {
    ih_core_trace("malloc");
  }

  return uuid;
}

ih_core_uuid_t *ih_core_uuid_create_null()
{
  return ih_core_uuid_create_from_string(NULL_UUID);
}

void ih_core_uuid_destroy(void *uuid_object)
{
  assert(uuid_object);
  ih_core_uuid_t *uuid;

  uuid = uuid_object;

  if (uuid->string) {
    if (strcmp(uuid->string, NULL_UUID) != 0) {
      free(uuid->string);
    }
  }
  free(uuid);
}

unsigned long ih_core_uuid_get_memory_size_bytes(ih_core_uuid_t *uuid)
{
  assert(uuid);
  unsigned long size;

  size = sizeof *uuid;
  size += sizeof(uuid_t);

  return size;
}

char *ih_core_uuid_get_string(void *uuid_object)
{
  assert(uuid_object);
  ih_core_uuid_t *uuid;

  uuid = uuid_object;

  if (!uuid->string) {
    uuid->string = malloc(37);
    if (uuid->string) {
      uuid_unparse(uuid->uuid, uuid->string);
    } else {
      uuid->string = NULL_UUID;
      ih_core_trace("malloc");
    }
  }

  assert(uuid->string);
  return uuid->string;
}

unsigned long ih_core_uuid_hash(ih_core_uuid_t *uuid)
{
  assert(uuid);
  unsigned long hash;

  ih_core_uuid_get_string(uuid);
  hash = ih_core_hasih_djb2_xor(uuid->string);

  return hash;
}

ih_core_bool_t ih_core_uuid_is_null(ih_core_uuid_t *uuid)
{
  assert(uuid);
  ih_core_bool_t is_null;
  ih_core_uuid_t *null_uuid;

  is_null = ih_core_bool_false;

  null_uuid = ih_core_uuid_create_null();
  if (null_uuid) {
    if (0 == ih_core_uuid_compare(uuid, null_uuid)) {
      is_null = ih_core_bool_true;
    }
    ih_core_uuid_destroy(null_uuid);
  } else {
    ih_core_trace("ih_core_uuid_create_null");
  }

  return is_null;
}
