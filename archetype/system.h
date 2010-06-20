#ifndef ih_archetype_system_h
#define ih_archetype_system_h

#include "ih/audit/log.h"
#include "ih/classify/classifyey.h"
#include "ih/container/array.h"
#include "ih/core/bitarray.h"
#include "ih/external/external.h"

struct ih_archetype_system_t;
typedef struct ih_archetype_system_t ih_archetype_system_t;

void *ih_archetype_system_create(ih_container_array_t *classified_objects,
    ih_audit_log_t *log);

void ih_archetype_system_destroy(void *system_void);

ih_core_bit_t ih_archetype_system_classify_object(void *system_void,
    ih_core_bitarray_t *object);

void ih_archetype_system_init_classifyey(ih_classify_classifyey_t *classifyey);

ih_core_bool_t ih_archetype_system_learn(void *system_void);

ih_core_bool_t ih_archetype_system_observe_object(void *system_void,
    ih_core_bitarray_t *classified_object);

#endif
