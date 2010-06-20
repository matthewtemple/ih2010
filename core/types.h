#ifndef ih_core_types_h
#define ih_core_types_h

#include "ih/core/object.h"

enum ih_core_type_t {
  IH_CORE_TYPE_UNKNOWN = 0,
  IH_CORE_TYPE_BIT = 1,
  IH_CORE_TYPE_IH_CORE_BOOL_T = 2,
  IH_CORE_TYPE_CHAR = 3,
  IH_CORE_TYPE_DOUBLE = 4,
  IH_CORE_TYPE_ENUM = 5,
  IH_CORE_TYPE_LONG = 6,
  IH_CORE_TYPE_SHORT = 7,
  IH_CORE_TYPE_STRING = 8,
  IH_CORE_TYPE_UNSIGNED_CHAR = 9,
  IH_CORE_TYPE_UNSIGNED_LONG = 10,
  IH_CORE_TYPE_UNSIGNED_SHORT = 11,
};
typedef enum ih_core_type_t ih_core_type_t;

typedef void *(*thread_f)(void *thread_context);

#define IH_CORE_TIME_STRING_LENGTH 15
typedef char ih_core_time_string_t[IH_CORE_TIME_STRING_LENGTH + 1];

#endif
