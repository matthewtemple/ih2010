#include "ih/ih.h"

#define DEFAULT_SEARCH_ALGORITHM IH_SEARCH_ALGORITHM_BIOS
#define MAX_POINTS 64
#define MAX_OVERALL_TIME_MICROSECONDS 1000000 * 60 * 5
#define MAX_SEARCH_TIME_MICROSECONDS 1000000
#define POINT_RANGE 100
#define POINTS_FILENAME "stravel_points.csv"
#define SOLUTION_FILENAME "stravel_solution.csv"

#define RANDOM_POINT_COUNT ih_core_min(64, MAX_POINTS)

struct stravel_t {
  unsigned short point_count;
  double points[MAX_POINTS][2];
  double distances[MAX_POINTS][MAX_POINTS];
  unsigned short current_point;
  ih_audit_log_t *log;
};
typedef struct stravel_t stravel_t;

struct comparable_point_t {
  unsigned short index;
  stravel_t *stravel;
};
typedef struct comparable_point_t comparable_point_t;

static void cache_distances(stravel_t *stravel);

static double calculate_point_distance(stravel_t *stravel,
    unsigned short point_a, unsigned short point_b);

static int compare_points(const void *comparable_point_a_void,
    const void *comparable_point_b_void);

static void create_random_points(stravel_t *stravel);

static unsigned short get_fork_point(stravel_t *stravel,
    unsigned short current_point, unsigned char fork, ih_core_bool_t visited[]);

static double get_point_distance_from_cache(stravel_t *stravel,
    unsigned short point_a, unsigned short point_b);

static ih_core_bool_t load_points_from_file(stravel_t *stravel,
    char *points_filename);

static ih_core_bool_t save_points_to_file(stravel_t *stravel, char *filename);

static ih_core_bool_t save_solution_to_file(stravel_t *stravel,
    ih_core_bitarray_t *solution, char *filename);

static ih_core_bool_t score_solution(void *context, ih_core_bitarray_t *solution,
    double *score);

static void cache_distances(stravel_t *stravel)
{
  unsigned short i;
  unsigned short j;

  for (i = 0; i < stravel->point_count; i++) {
    for (j = 0; j < stravel->point_count; j++) {
      stravel->distances[i][j] = calculate_point_distance(stravel, i, j);
    }
  }
}

double calculate_point_distance(stravel_t *stravel, unsigned short point_a,
    unsigned short point_b)
{
  double d;
  double x1;
  double y1;
  double x2;
  double y2;

  x1 = stravel->points[point_a][0];
  y1 = stravel->points[point_a][1];
  x2 = stravel->points[point_b][0];
  y2 = stravel->points[point_b][1];

  d = ih_matih_tools_calculate_distance(x1, y1, x2, y2);

  return d;
}

int compare_points(const void *comparable_point_a_void,
    const void *comparable_point_b_void)
{
  assert(comparable_point_a_void);
  assert(comparable_point_b_void);
  const comparable_point_t *comparable_point_a;
  const comparable_point_t *comparable_point_b;
  unsigned short point_a;
  unsigned short point_b;
  int compare_result;
  double distance_to_a;
  double distance_to_b;
  stravel_t *stravel;

  comparable_point_a = (const comparable_point_t *) comparable_point_a_void;
  comparable_point_b = (const comparable_point_t *) comparable_point_b_void;

  point_a = comparable_point_a->index;
  point_b = comparable_point_b->index;
  stravel = comparable_point_a->stravel;

  distance_to_a
    = get_point_distance_from_cache(stravel, stravel->current_point, point_a);
  distance_to_b
    = get_point_distance_from_cache(stravel, stravel->current_point, point_b);

  if (distance_to_a < distance_to_b) {
    compare_result = -1;
  } else if (distance_to_b < distance_to_a) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

void create_random_points(stravel_t *stravel)
{
  unsigned short i;

  stravel->point_count = RANDOM_POINT_COUNT;

  ih_core_seed_random_witih_time();
  for (i = 0; i < stravel->point_count; i++) {
    stravel->points[i][0] = ((random() % 1000000) / 1000000.0) * POINT_RANGE;
    stravel->points[i][1] = ((random() % 1000000) / 1000000.0) * POINT_RANGE;
  }
}

unsigned short get_fork_point(stravel_t *stravel, unsigned short current_point,
    unsigned char fork, ih_core_bool_t visited[])
{
  comparable_point_t closest_points[stravel->point_count];
  unsigned short i;
  unsigned short j;
  unsigned short next_point;

  j = 0;
  for (i = 0; i < stravel->point_count; i++) {
    if (!visited[i]) {
      closest_points[j].index = i;
      closest_points[j].stravel = stravel;
      j++;
    }
  }

  stravel->current_point = current_point;
  qsort(closest_points, j, sizeof(comparable_point_t), compare_points);

  if (fork < j) {
    next_point = closest_points[fork].index;
  } else {
    next_point = closest_points[j - 1].index;
  }

  return next_point;
}

double get_point_distance_from_cache(stravel_t *stravel,
    unsigned short point_a, unsigned short point_b)
{
  return stravel->distances[point_a][point_b];
}

ih_core_bool_t load_points_from_file(stravel_t *stravel, char *points_filename)
{
  ih_file_csv_t *csv;
  ih_core_bool_t success;
  unsigned short each_point;
  double x;
  double y;

  csv = ih_file_csv_create_extended(points_filename, 0, MAX_POINTS - 1);
  if (csv) {
    success = ih_core_bool_true;
    stravel->point_count = ih_file_csv_get_object_count(csv);
    ih_audit_log_enter(stravel->log, "stvl", "loading %lu points from %s",
        stravel->point_count, points_filename);
    for (each_point = 0; each_point < stravel->point_count; each_point++) {
      x = ih_file_csv_get_value_by_name_as_double(csv, each_point, "x");
      y = ih_file_csv_get_value_by_name_as_double(csv, each_point, "y");
      stravel->points[each_point][0] = x;
      stravel->points[each_point][1] = y;
    }
    ih_file_csv_destroy(csv);
  } else {
    success = ih_core_bool_false;
    ih_audit_log_trace(stravel->log, "stvl", "ih_file_csv_create");
  }

  return success;
}

ih_core_bool_t save_points_to_file(stravel_t *stravel, char *filename)
{
  assert(stravel);
  ih_file_basic_t *file;
  ih_core_bool_t success;
  unsigned short each_point;
  double x;
  double y;
  char string[64];

  file
    = ih_file_basic_create(filename, IH_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE);
  if (file) {
    success = ih_core_bool_true;

    if (!ih_file_basic_write_string(file, "x,y\n")) {
      success = ih_core_bool_false;
      ih_audit_log_trace(stravel->log, "stvl", "ih_file_basic_write_string");
    }

    for (each_point = 0; each_point < stravel->point_count; each_point++) {
      x = stravel->points[each_point][0];
      y = stravel->points[each_point][1];
      if (sprintf(string, "%.5f,%.5f\n", x, y) >= 0) {  /*  TODO  */
        if (!ih_file_basic_write_string(file, string)) {
          success = ih_core_bool_false;
          ih_audit_log_trace(stravel->log, "stvl", "ih_file_basic_write_string");
          break;
        }
      } else {
        success = ih_core_bool_false;
        ih_audit_log_trace(stravel->log, "stvl", "sprintf");
        break;
      }
    }
    ih_file_basic_destroy(file);
  } else {
    success = ih_core_bool_false;
    ih_audit_log_trace(stravel->log, "stvl", "ih_file_basic_create");
  }

  return success;
}

ih_core_bool_t save_solution_to_file(stravel_t *stravel,
    ih_core_bitarray_t *solution, char *filename)
{
  assert(stravel);
  assert(filename);
  ih_core_bool_t visited[MAX_POINTS];
  unsigned short each_point;
  unsigned char bit_0;
  unsigned char bit_1;
  unsigned short fork;
  unsigned short current_point;
  unsigned short next_point;
  ih_file_basic_t *file;
  ih_core_bool_t success;
  double x;
  double y;
  char string[64];

  for (each_point = 0; each_point < stravel->point_count; each_point++) {
    visited[each_point] = ih_core_bool_false;
  }

  file
    = ih_file_basic_create(filename, IH_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE);
  if (file) {
    success = ih_core_bool_true;

    if (!ih_file_basic_write_string(file, "x,y\n")) {
      success = ih_core_bool_false;
      ih_audit_log_trace(stravel->log, "stvl", "ih_file_basic_write_string");
    }

    current_point = 0;
    x = stravel->points[current_point][0];
    y = stravel->points[current_point][1];
    if (sprintf(string, "%.5f,%.5f\n", x, y) >= 0) {  /*  TODO  */
      if (!ih_file_basic_write_string(file, string)) {
        success = ih_core_bool_false;
        ih_audit_log_trace(stravel->log, "stvl", "ih_file_basic_write_string");
      }
    } else {
      success = ih_core_bool_false;
      ih_audit_log_trace(stravel->log, "stvl", "sprintf");
    }
    visited[current_point] = ih_core_bool_true;

    for (each_point = 0; each_point < (stravel->point_count - 1);
         each_point++) {
      bit_0 = ih_core_bitarray_get_bit(solution, (current_point * 2) + 0);
      bit_1 = ih_core_bitarray_get_bit(solution, (current_point * 2) + 1);
      fork = (bit_0 * 1) + (bit_1 * 2);
      next_point = get_fork_point(stravel, current_point, fork, visited);
      x = stravel->points[next_point][0];
      y = stravel->points[next_point][1];
      if (sprintf(string, "%.5f,%.5f\n", x, y) >= 0) {  /*  TODO  */
        if (!ih_file_basic_write_string(file, string)) {
          success = ih_core_bool_false;
          ih_audit_log_trace(stravel->log, "stvl", "ih_file_basic_write_string");
          break;
        }
      } else {
        success = ih_core_bool_false;
        ih_audit_log_trace(stravel->log, "stvl", "sprintf");
        break;
      }

      visited[next_point] = ih_core_bool_true;
      current_point = next_point;
    }
    ih_file_basic_destroy(file);
  } else {
    success = ih_core_bool_false;
    ih_audit_log_trace(stravel->log, "stvl", "ih_file_basic_create");
  }

  return success;
}

ih_core_bool_t score_solution(void *context, ih_core_bitarray_t *solution,
    double *score)
{
  assert(score);
  assert(solution);
  assert(score);
  ih_core_bool_t visited[MAX_POINTS];
  unsigned short i;
  unsigned bit_0;
  unsigned bit_1;
  unsigned short fork;
  unsigned short current_point;
  unsigned short next_point;
  double distance;
  stravel_t *stravel;

  stravel = context;

  *score = 0.0;

  for (i = 0; i < stravel->point_count; i++) {
    visited[i] = ih_core_bool_false;
  }

  current_point = 0;
  visited[0] = ih_core_bool_true;
  for (i = 0; i < (stravel->point_count - 1); i++) {
    bit_0 = ih_core_bitarray_get_bit(solution, (current_point * 2) + 0);
    bit_1 = ih_core_bitarray_get_bit(solution, (current_point * 2) + 1);
    fork = (bit_0 * 1) + (bit_1 * 2);
    next_point = get_fork_point(stravel, current_point, fork, visited);
    visited[next_point] = ih_core_bool_true;
    distance
      = get_point_distance_from_cache(stravel, current_point, next_point);
    *score += distance;
    current_point = next_point;
  }

  return ih_core_bool_true;
}

int main(int argc, char *argv[])
{
  ih_config_options_t *options;
  ih_core_objects_t objects;
  ih_audit_log_t *log;
  char *points_filename;
  ih_search_system_t *system;
  ih_container_array_t *initial_solutions;
  unsigned short initial_solution_count;
  ih_container_array_t *solutions;
  unsigned short solution_count;
  ih_core_bool_t still_searching;
  unsigned short i;
  ih_core_bitarray_t *solution;
  double score;
  double best_score;
  stravel_t stravel;
  double goal_distance;
  double sqrt_point_count;
  struct timeval start_time;
  char *search_algorithm_string;
  ih_search_algorithm_t search_algorithm;

  initial_solutions = NULL;
  initial_solution_count = 0;

  ih_core_objects_init(&objects);

  log = ih_audit_log_create(stdout);
  if (!log) {
    ih_core_trace_exit("ih_audit_log_create");
  }
  stravel.log = log;

  options = ih_config_options_create(argc, argv, &objects);
  if (!options) {
    ih_audit_log_trace_exit(log, "stvl", "ih_config_options_create");
  }

  if (ih_config_options_find_as_string(options, "points_filename",
          &points_filename, "")) {
    if (!load_points_from_file(&stravel, points_filename)) {
      ih_audit_log_trace_exit(log, "stvl", "load_points_from_file");
    }
  } else {
    ih_audit_log_enter
      (log, "stvl", "creating %lu random points", RANDOM_POINT_COUNT);
    create_random_points(&stravel);
  }

  if (ih_config_options_find_as_string(options, "search_algorithm",
          &search_algorithm_string, "")) {
    if (!ih_search_algorithm_get_from_string
        (search_algorithm_string, &search_algorithm)) {
      search_algorithm = DEFAULT_SEARCH_ALGORITHM;
    }
  } else {
    search_algorithm = DEFAULT_SEARCH_ALGORITHM;
  }
  search_algorithm_string = ih_search_algorithm_get_string(search_algorithm);
  ih_audit_log_enter(log, "stvl", "search algorithm: %s",
      search_algorithm_string);

  ih_audit_log_enter(log, "stvl", "saving points to stravel_points.csv");
  if (!save_points_to_file(&stravel, POINTS_FILENAME)) {
    ih_audit_log_trace_exit(log, "stvl", "save_points_to_file");
  }

  sqrt_point_count = sqrt(stravel.point_count);
  goal_distance = ((POINT_RANGE * sqrt_point_count) + POINT_RANGE) * 1.0;

  ih_audit_log_enter(log, "stvl", "caching point distances");
  cache_distances(&stravel);

  ih_audit_log_enter(log, "stvl", "creating search system");
  system = ih_search_system_create(score_solution, IH_CORE_GOAL_MINIMIZE_SCORE,
      &stravel, initial_solutions, search_algorithm, log);
  if (system) {
    ih_audit_log_enter(log, "stvl", "goal distance: %.2f", goal_distance);
    gettimeofday(&start_time, NULL);
    still_searching = ih_core_bool_true;
    while (still_searching) {
      ih_search_system_search(system, MAX_SEARCH_TIME_MICROSECONDS);
      solutions = ih_search_system_get_solutions_copy(system, 1);
      if (solutions) {
        solution = ih_container_array_find(solutions, 0);
        if (!score_solution(&stravel, solution, &best_score)) {
          ih_audit_log_trace_exit(log, "stvl", "score_solution");
        }
        solution_count = ih_container_array_get_size(solutions);
        for (i = 0; i < solution_count; i++) {
          solution = ih_container_array_find(solutions, i);
          if (!score_solution(&stravel, solution, &score)) {
            ih_audit_log_trace_exit(log, "stvl", "score_solution");
          }
          if (ih_core_bool_true) {
            ih_audit_log_enter(log, "stvl", "distance: %.2f", score);
          }
          fflush(stdout);
          if (score <= goal_distance) {
            still_searching = ih_core_bool_false;
            ih_audit_log_enter
              (log, "stvl", "reached goal distance, stopping search");
          }
        }
        ih_container_array_destroy(solutions);
      } else {
        ih_audit_log_trace_exit
          (log, "stvl", "ih_search_system_get_solutions_copy");
        break;
      }
      if (!ih_core_time_is_remaining_microseconds
          (&start_time, MAX_OVERALL_TIME_MICROSECONDS)) {
        still_searching = ih_core_bool_false;
        ih_audit_log_enter
          (log, "stvl", "max search time elapsed, stopping search");
      }
    }

    solutions = ih_search_system_get_solutions_copy(system, 1);
    if (solutions) {
      solution = ih_container_array_find(solutions, 0);
      ih_audit_log_enter
        (log, "stvl", "saving solution to %s", SOLUTION_FILENAME);
      if (!save_solution_to_file(&stravel, solution, SOLUTION_FILENAME)) {
        ih_audit_log_trace_exit(log, "stvl", "save_solution_to_file");
      }
      ih_container_array_destroy(solutions);
    } else {
      ih_audit_log_trace_exit
        (log, "stvl", "ih_search_system_get_solutions_copy");
    }

    ih_search_system_destroy(system);
  } else {
    ih_audit_log_trace_exit(log, "stvl", "ih_search_system_create");
  }

  ih_config_options_destroy(options);
  ih_audit_log_destroy(log);

  return 0;
}
