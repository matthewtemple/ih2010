#include "ih/config/tools.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/psql/result.h"
#include "ih/psql/system.h"

int main(int argc, char *argv[])
{
  ih_config_disable_if_running_batcih_tests(argc, argv);

  ih_psql_system_t *ih_psql_system;
  ih_psql_system_create_error_t ih_psql_system_create_error;
  ih_psql_system_query_error_t ih_psql_system_query_error;
  char *ih_psql_system_query_error_text;
  ih_psql_result_t *ih_psql_result;
  char *sql;
  ih_container_map_t *row;
  ih_psql_system_execute_error_t ih_psql_system_execute_error;
  char *ih_psql_system_execute_error_text;
  ih_audit_log_t *log;
  unsigned long connection_count;

  connection_count = 1;

  log = ih_audit_log_create(stdout);
  if (!log) {
    ih_core_trace_exit("ih_audit_log_create");
  }

  ih_psql_system = ih_psql_system_create("localhost", "h", "h", "h",
      connection_count, log, &ih_psql_system_create_error);
  if (!ih_psql_system) {
    switch (ih_psql_system_create_error) {
      case ih_psql_system_create_error_connect_failed:
        ih_core_trace_exit("connect failed");
        break;
      case ih_psql_system_create_error_unknown:
      default:
        ih_core_trace_exit("ih_psql_system_create");
        break;
    }
  }

  sql = ih_core_string_append(NULL, "select * from thing where id > 1");
  if (!sql) {
    ih_core_trace_exit("ih_core_string_append");
  }
  ih_psql_result = ih_psql_system_query(ih_psql_system, sql,
      &ih_psql_system_query_error, &ih_psql_system_query_error_text);
  if (!ih_psql_result) {
    ih_core_trace_exit("%s", ih_psql_system_query_error_text);
  }

  ih_psql_result_iterate_start(ih_psql_result);
  while ((row = ih_psql_result_iterate_next(ih_psql_result))) {
    printf("id=%s name=%s\n", (char *) ih_container_map_find(row, "id"),
        (char *) ih_container_map_find(row, "name"));
  }

  ih_psql_result_destroy(ih_psql_result);
  free(sql);

  sql = ih_core_string_append(NULL,
      "insert into thing (id, name) values (4, 'd')");
  if (!ih_psql_system_execute(ih_psql_system, sql, &ih_psql_system_execute_error,
          &ih_psql_system_execute_error_text)) {
    ih_core_trace_exit("%s", ih_psql_system_execute_error_text);
  }
  free(sql);

  ih_psql_system_destroy(ih_psql_system);
  ih_audit_log_destroy(log);

  return 0;
}
