#ifndef ih_classify_classifyey_h
#define ih_classify_classifyey_h

#include "ih/audit/log.h"
#include "ih/container/array.h"
#include "ih/core/bitarray.h"
#include "ih/external/external.h"

typedef void *(*ih_classify_classifyey_create_f)
  (ih_container_array_t *classified_objects, ih_audit_log_t *log);

typedef void (*ih_classify_classifyey_destroy_f)(void *system_void);

typedef ih_core_bit_t (*ih_classify_classifyey_classify_object_f)
(void *system_void, ih_core_bitarray_t *object);

typedef ih_core_bool_t (*ih_classify_classifyey_learn_f)(void *system_void);

typedef ih_core_bool_t (*ih_classify_classifyey_observe_object_f)
(void *system_void, ih_core_bitarray_t *classified_object);

struct ih_classify_classifyey_t {
  ih_classify_classifyey_create_f create;
  ih_classify_classifyey_destroy_f destroy;
  ih_classify_classifyey_classify_object_f classify_object;
  ih_classify_classifyey_learn_f learn;
  ih_classify_classifyey_observe_object_f observe_object;
};
typedef struct ih_classify_classifyey_t ih_classify_classifyey_t;

void ih_classify_classifyey_free(ih_classify_classifyey_t *classifyey);

void ih_classify_classifyey_init(ih_classify_classifyey_t *classifyey,
    ih_classify_classifyey_create_f create,
    ih_classify_classifyey_destroy_f destroy,
    ih_classify_classifyey_classify_object_f classify_object,
    ih_classify_classifyey_learn_f learn,
    ih_classify_classifyey_observe_object_f observe_object);

#endif
