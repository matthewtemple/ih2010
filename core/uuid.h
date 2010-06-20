#ifndef ih_core_uuid_h
#define ih_core_uuid_h

#include "ih/core/bool.h"
#include "ih/core/message.h"
#include "ih/core/object.h"

struct ih_core_uuid_t;
typedef struct ih_core_uuid_t ih_core_uuid_t;

ih_core_bool_t ih_core_uuid_add_to_message(void *uuid_object,
    ih_core_message_t *message);

int ih_core_uuid_compare(void *uuid_object_a, void *uuid_object_b);

void *ih_core_uuid_copy(void *uuid_object);

ih_core_uuid_t *ih_core_uuid_create();

void *ih_core_uuid_create_from_message(ih_core_message_t *message);

ih_core_uuid_t *ih_core_uuid_create_from_string(const char *uuid_string);

ih_core_uuid_t *ih_core_uuid_create_null();

void ih_core_uuid_destroy(void *uuid_object);

unsigned long ih_core_uuid_get_memory_size_bytes(ih_core_uuid_t *uuid);

char *ih_core_uuid_get_string(void *uuid_object);

unsigned long ih_core_uuid_hash(ih_core_uuid_t *uuid);

ih_core_bool_t ih_core_uuid_is_null(ih_core_uuid_t *uuid);

#endif
