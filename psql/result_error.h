#ifndef ih_psql_result_error_h
#define ih_psql_result_error_h

enum ih_psql_result_create_error_t {
  ih_psql_result_create_error_unknown,
  ih_psql_result_create_error_ih_container_array_create_failed,
  ih_psql_result_create_error_ih_container_map_add_failed,
  ih_psql_result_create_error_ih_container_map_create_failed,
  ih_psql_result_create_error_malloc_failed,
};
typedef enum ih_psql_result_create_error_t ih_psql_result_create_error_t;

#endif
