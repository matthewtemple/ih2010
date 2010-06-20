#include "ih/container/array.h"
#include "ih/container/map.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/psql/result.h"

struct ih_psql_result_t {
  ih_container_array_t *rows;
  PGresult *pg_result;
};

ih_psql_result_t *ih_psql_result_create(PGresult *pg_result,
    ih_core_objectey_t *string_objectey, ih_psql_result_create_error_t *error)
{
  assert(pg_result);
  ih_psql_result_t *result;
  int row_count;
  int field_count;
  int each_row;
  int each_column;
  char *field_name;
  char *field_value;
  ih_container_map_t *map;
  ih_core_bool_t so_far_so_good;

  result = malloc(sizeof *result);
  if (result) {
    result->pg_result = pg_result;
    row_count = PQntuples(pg_result);
    result->rows = ih_container_array_create(row_count, ih_container_map_compare,
        ih_container_map_copy, ih_container_map_destroy);
    if (result->rows) {
      field_count = PQnfields(pg_result);
      so_far_so_good = ih_core_bool_true;
      for (each_row = 0; each_row < row_count; each_row++) {
        map = ih_container_map_create(string_objectey, string_objectey,
            IH_CONTAINER_MAP_DONT_DESTROY);
        if (map) {
          for (each_column = 0; each_column < field_count; each_column++) {
            field_name = PQfname(pg_result, each_column);
            field_value = PQgetvalue(pg_result, each_row, each_column);
            if (ih_container_map_add(map, field_name, field_value)) {
              ih_container_array_add(result->rows, each_row, map);
            } else {
              *error = ih_psql_result_create_error_ih_container_map_add_failed;
              so_far_so_good = ih_core_bool_false;
              ih_container_map_destroy(map);
              break;
            }
          }
        } else {
          *error = ih_psql_result_create_error_ih_container_map_create_failed;
          so_far_so_good = ih_core_bool_false;
          break;
        }
      }
      if (!so_far_so_good) {
        free(result);
        result = NULL;
      }
    } else {
      *error = ih_psql_result_create_error_ih_container_array_create_failed;
    }
  } else {
    *error = ih_psql_result_create_error_malloc_failed;
  }

  return result;
}

void ih_psql_result_destroy(ih_psql_result_t *result)
{
  ih_container_array_destroy(result->rows);
  PQclear(result->pg_result);
  free(result);
}

ih_container_map_t *ih_psql_result_get_row(ih_psql_result_t *result,
    unsigned long row_index)
{
  return ih_container_array_find(result->rows, row_index);
}

unsigned long ih_psql_result_get_row_count(ih_psql_result_t *result)
{
  return ih_container_array_get_size(result->rows);
}

ih_container_map_t *ih_psql_result_iterate_next(ih_psql_result_t *result)
{
  return ih_container_array_iterate_next(result->rows);
}

void ih_psql_result_iterate_start(ih_psql_result_t *result)
{
  ih_container_array_iterate_start(result->rows);
}
