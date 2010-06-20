#ifndef ih_ccsv_system_h
#define ih_ccsv_system_h

#include "ih/external/external.h"

struct ih_ccsv_system_t;
typedef struct ih_ccsv_system_t ih_ccsv_system_t;

ih_ccsv_system_t *ih_ccsv_system_create();

void ih_ccsv_system_destroy(void *system_void);

#endif
