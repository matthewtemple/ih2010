#ifndef ih_psql_system_h
#define ih_psql_system_h

#include "ih/audit/log.h"
#include "ih/psql/result.h"
#include "ih/psql/system_error.h"

struct ih_psql_system_t;
typedef struct ih_psql_system_t ih_psql_system_t;

ih_psql_system_t *ih_psql_system_create(const char *db_host, const char *db_name,
    const char *db_user, const char *db_password,
    unsigned short connection_count, ih_audit_log_t *log,
    ih_psql_system_create_error_t *error);

void ih_psql_system_destroy(ih_psql_system_t *system);

ih_core_bool_t ih_psql_system_execute(ih_psql_system_t *system, char *sql,
    ih_psql_system_execute_error_t *error, char **error_text);

ih_psql_result_t *ih_psql_system_query(ih_psql_system_t *system,
    char *sql, ih_psql_system_query_error_t *error, char **error_text);

ih_core_bool_t ih_psql_system_query_count(ih_psql_system_t *system,
    char *sql, unsigned long *count, ih_psql_system_query_count_error_t *error,
    char **error_text);

ih_core_bool_t ih_psql_system_vacuum(ih_psql_system_t *system,
    ih_psql_system_vacuum_error_t *error);

#endif
