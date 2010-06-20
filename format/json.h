#ifndef ih_format_json_h
#define ih_format_json_h

#include "ih/external/external.h"

struct ih_format_json_t;
typedef struct ih_format_json_t ih_format_json_t;

ih_format_json_t *ih_format_json_create();

void ih_format_json_destroy(ih_format_json_t *json);

#endif
