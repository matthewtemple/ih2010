#ifndef ih_audit_log_h
#define ih_audit_log_h

#include "ih/core/bool.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

#define ih_audit_log_trace(log, system, entry, ...) _ih_audit_log_trace \
  (log, system, entry, __FILE__, __LINE__, ##__VA_ARGS__);

#define ih_audit_log_trace_exit(log, system, entry, ...) _ih_audit_log_trace \
  (log, system, entry, __FILE__, __LINE__, ##__VA_ARGS__); exit(44);

struct ih_audit_log_t;
typedef struct ih_audit_log_t ih_audit_log_t;

ih_core_bool_t ih_audit_log_add_file(ih_audit_log_t *log, FILE *file);

ih_audit_log_t *ih_audit_log_create(FILE *file);

void ih_audit_log_destroy(ih_audit_log_t *log);

ih_core_bool_t ih_audit_log_enter(ih_audit_log_t *log, const char *system,
    const char *entry, ...);

ih_core_bool_t ih_audit_log_remove_file(ih_audit_log_t *log, FILE *file);

ih_core_bool_t _ih_audit_log_trace(ih_audit_log_t *log, const char *system,
    const char *entry, const char *file, unsigned long line, ...);

#endif
