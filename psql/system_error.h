#ifndef ih_psql_system_error_h
#define ih_psql_system_error_h

enum ih_psql_system_create_error_t {
  ih_psql_system_create_error_unknown,
  ih_psql_system_create_error_connect_failed,
};
typedef enum ih_psql_system_create_error_t ih_psql_system_create_error_t;

enum ih_psql_system_execute_error_t {
  ih_psql_system_execute_error_unknown,
  ih_psql_system_execute_error_bad_response,
  ih_psql_system_execute_error_empty_query,
  ih_psql_system_execute_error_fatal_error,
};
typedef enum ih_psql_system_execute_error_t
ih_psql_system_execute_error_t;

enum ih_psql_system_query_count_error_t {
  ih_psql_system_query_count_error_unknown,
  ih_psql_system_query_count_error_no_c_column,
};
typedef enum ih_psql_system_query_count_error_t
ih_psql_system_query_count_error_t;

enum ih_psql_system_query_error_t {
  ih_psql_system_query_error_unknown,
  ih_psql_system_query_error_bad_response,
  ih_psql_system_query_error_empty_query,
  ih_psql_system_query_error_fatal_error,
};
typedef enum ih_psql_system_query_error_t ih_psql_system_query_error_t;

enum ih_psql_system_vacuum_error_t {
  ih_psql_system_vacuum_error_unknown,
};
typedef enum ih_psql_system_vacuum_error_t ih_psql_system_vacuum_error_t;

#endif
