#include "ih/cor3/actor.h"
#include "ih/cor3/system.h"
#include "ih/core/constants.h"
#include "ih/core/goal.h"

#define ANARCHY_MODULUS 4

struct ih_cor3_actor_t {
  void *box_cell;
  unsigned char elements[4][4][4];
  ih_core_bitarray_t *solution;
  ih_cor3_system_t *system;
  double score;
  ih_core_bool_t score_is_valid;
};

static void dance(ih_cor3_actor_t *destination, ih_cor3_actor_t *source_a,
    ih_cor3_actor_t *source_b);

static ih_cor3_actor_t *find_best_nearby_excluding_me(ih_cor3_actor_t *actor);

static ih_cor3_actor_t *find_worst_nearby_including_me(ih_cor3_actor_t *actor);

static unsigned char get_element(ih_cor3_actor_t *actor, unsigned short x,
    unsigned short y, unsigned short z);

static double get_score(ih_cor3_actor_t *actor);

static unsigned short get_solution_index_from_element_coordinates
(unsigned short x, unsigned short y, unsigned short z);

static ih_core_bool_t in_range(unsigned short index, unsigned short cut,
    unsigned short direction);

static void set_element(ih_cor3_actor_t *actor, unsigned short x,
    unsigned short y, unsigned short z, unsigned char value);

void dance(ih_cor3_actor_t *destination, ih_cor3_actor_t *source_a,
    ih_cor3_actor_t *source_b)
{
  assert(destination);
  assert(source_a);
  assert(source_b);
  unsigned short cut_x;
  unsigned short cut_y;
  unsigned short cut_z;
  unsigned short direction_x;
  unsigned short direction_y;
  unsigned short direction_z;
  unsigned short x;
  unsigned short y;
  unsigned short z;
  unsigned char element;

  cut_x = random() % 5;
  cut_y = random() % 5;
  cut_z = random() % 5;

  direction_x = random() % 2;
  direction_y = random() % 2;
  direction_z = random() % 2;

  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      for (z = 0; z < 4; z++) {
        if (in_range(x, cut_x, direction_x)
            && in_range(y, cut_y, direction_y)
            && in_range(z, cut_z, direction_z)) {
          element = get_element(source_a, x, y, z);
        } else {
          element = get_element(source_b, x, y, z);
        }
        /*  TODO: make this a #define  */
        if (0 == (random() % 64)) {
          element = random() % 256;
        }
        set_element(destination, x, y, z, element);
      }
    }
  }
}

ih_cor3_actor_t *find_best_nearby_excluding_me(ih_cor3_actor_t *actor)
{
  assert(actor);
  ih_box_coordinate_t relative_coordinate;
  ih_cor3_actor_t *neighbor_actor;
  ih_cor3_actor_t *best_actor;
  double best_score;
  double neighbor_score;
  ih_core_goal_t goal;
  ih_box_system_t *box;

  best_score = 0.0;
  best_actor = NULL;

  goal = ih_cor3_system_get_goal(actor->system);
  box = ih_cor3_system_get_box(actor->system);

  for (relative_coordinate.x = 0;
       relative_coordinate.x < 3;
       relative_coordinate.x++) {
    for (relative_coordinate.y = 0;
         relative_coordinate.y < 3;
         relative_coordinate.y++) {
      for (relative_coordinate.z = 0;
           relative_coordinate.z < 3;
           relative_coordinate.z++) {
        if ((1 == relative_coordinate.x)
            && (1 == relative_coordinate.y)
            && (1 == relative_coordinate.z)) {
          continue;
        }
        neighbor_actor = ih_box_system_find_relative(box, actor,
            &relative_coordinate);
        neighbor_score = get_score(neighbor_actor);
        if (!best_actor) {
          best_actor = neighbor_actor;
          best_score = neighbor_score;
        } else {
          if (IH_CORE_GOAL_MAXIMIZE_SCORE == goal) {
            if (neighbor_score > best_score) {
              best_actor = neighbor_actor;
              best_score = neighbor_score;
            }
          } else {
            if (neighbor_score < best_score) {
              best_actor = neighbor_actor;
              best_score = neighbor_score;
            }
          }
        }
      }
    }
  }

  assert(best_actor);
  return best_actor;
}

ih_cor3_actor_t *find_worst_nearby_including_me(ih_cor3_actor_t *actor)
{
  assert(actor);
  ih_box_coordinate_t relative_coordinate;
  ih_cor3_actor_t *neighbor_actor;
  ih_cor3_actor_t *worst_actor;
  double worst_score;
  double neighbor_score;
  ih_core_goal_t goal;
  ih_box_system_t *box;

  worst_actor = NULL;
  worst_score = 0.0;

  goal = ih_cor3_system_get_goal(actor->system);
  box = ih_cor3_system_get_box(actor->system);

  for (relative_coordinate.x = 0;
       relative_coordinate.x < 3;
       relative_coordinate.x++) {
    for (relative_coordinate.y = 0;
         relative_coordinate.y < 3;
         relative_coordinate.y++) {
      for (relative_coordinate.z = 0;
           relative_coordinate.z < 3;
           relative_coordinate.z++) {
        neighbor_actor = ih_box_system_find_relative(box, actor,
            &relative_coordinate);
        neighbor_score = get_score(neighbor_actor);
        if (!worst_actor) {
          worst_actor = neighbor_actor;
          worst_score = neighbor_score;
        } else {
          if (IH_CORE_GOAL_MAXIMIZE_SCORE == goal) {
            if (neighbor_score < worst_score) {
              worst_actor = neighbor_actor;
              worst_score = neighbor_score;
            }
          } else {
            if (neighbor_score > worst_score) {
              worst_actor = neighbor_actor;
              worst_score = neighbor_score;
            }
          }
        }
      }
    }
  }

  assert(worst_actor);
  return worst_actor;
}

unsigned char get_element(ih_cor3_actor_t *actor, unsigned short x,
    unsigned short y, unsigned short z)
{
  assert(actor);
  assert(x <= 3);
  assert(y <= 3);
  assert(z <= 3);

  return actor->elements[x][y][z];
}

double get_score(ih_cor3_actor_t *actor)
{
  assert(actor);
  void *context;
  ih_core_score_solution_f score_solution;
  ih_audit_log_t *log;

  if (!actor->score_is_valid) {
    context = ih_cor3_system_get_context(actor->system);
    score_solution = ih_cor3_system_get_score_solution(actor->system);
    if (score_solution(context, actor->solution, &actor->score)) {
      actor->score_is_valid = ih_core_bool_true;
    } else {
      log = ih_cor3_system_get_log(actor->system);
      ih_audit_log_trace(log, "cor3", "score_solution");
    }
  }

  return actor->score;
}

unsigned short get_solution_index_from_element_coordinates(unsigned short x,
    unsigned short y, unsigned short z)
{
  return (x * 16) + (y * 4) + z;
}

void ih_cor3_actor_act(ih_cor3_actor_t *actor)
{
  assert(actor);
  ih_cor3_actor_t *best_other_actor;
  ih_cor3_actor_t *worst_actor;
  ih_box_system_t *box;

  box = ih_cor3_system_get_box(actor->system);

  best_other_actor = find_best_nearby_excluding_me(actor);
  ih_box_system_swap(box, actor, best_other_actor);

  worst_actor = find_worst_nearby_including_me(actor);
  if (0 == (random() % ANARCHY_MODULUS)) {
    dance(actor, best_other_actor, worst_actor);
  } else {
    dance(worst_actor, actor, best_other_actor);
  }

}

int ih_cor3_actor_compare_maximize(void *actor_a_void, void *actor_b_void)
{
  assert(actor_a_void);
  assert(actor_b_void);
  ih_cor3_actor_t **actor_a;
  ih_cor3_actor_t **actor_b;
  double actor_a_score;
  double actor_b_score;
  int compare_result;

  actor_a = (ih_cor3_actor_t **) actor_a_void;
  actor_b = (ih_cor3_actor_t **) actor_b_void;

  actor_a_score = get_score((ih_cor3_actor_t *) *actor_a);
  actor_b_score = get_score((ih_cor3_actor_t *) *actor_b);
  if (actor_a_score > actor_b_score) {
    compare_result = -1;
  } else if (actor_a_score < actor_b_score) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

int ih_cor3_actor_compare_minimize(void *actor_a_void, void *actor_b_void)
{
  assert(actor_a_void);
  assert(actor_b_void);
  ih_cor3_actor_t **actor_a;
  ih_cor3_actor_t **actor_b;
  double actor_a_score;
  double actor_b_score;
  int compare_result;

  actor_a = (ih_cor3_actor_t **) actor_a_void;
  actor_b = (ih_cor3_actor_t **) actor_b_void;

  actor_a_score = get_score((ih_cor3_actor_t *) *actor_a);
  actor_b_score = get_score((ih_cor3_actor_t *) *actor_b);
  if (actor_a_score < actor_b_score) {
    compare_result = -1;
  } else if (actor_a_score > actor_b_score) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

void *ih_cor3_actor_copy(void *actor_void)
{
  ih_core_trace_exit("TODO: implement");
  return NULL;
}

void *ih_cor3_actor_create(void *system_void, ih_core_bitarray_t *solution)
{
  assert(system_void);
  assert(solution);
  ih_cor3_actor_t *actor;
  unsigned short x;
  unsigned short y;
  unsigned short z;
  unsigned short i;
  ih_audit_log_t *log;
  ih_cor3_system_t *system;

  system = system_void;

  log = ih_cor3_system_get_log(system);

  actor = malloc(sizeof *actor);
  if (actor) {
    actor->system = system;
    actor->score_is_valid = ih_core_bool_false;
    actor->box_cell = NULL;
    for (x = 0; x < 4; x++) {
      for (y = 0; y < 4; y++) {
        for (z = 0; z < 4; z++) {
          i = get_solution_index_from_element_coordinates(x, y, z);
          actor->elements[x][y][z] = ih_core_bitarray_get_bit(solution, i);
        }
      }
    }
    actor->solution = ih_core_bitarray_copy(solution);
    if (!actor->solution) {
      ih_audit_log_trace(log, "cor3", "ih_core_bitarray_copy");
      free(actor);
      actor = NULL;
    }
  } else {
    ih_audit_log_trace(log, "cor3", "malloc");
  }

  return actor;
}

void *ih_cor3_actor_create_random(void *system_void)
{
  assert(system_void);
  ih_core_bitarray_t *bitarray;
  ih_cor3_actor_t *actor;
  ih_audit_log_t *log;
  ih_cor3_system_t *system;

  system = system_void;

  bitarray = ih_core_bitarray_create_random(IH_CORE_SOLUTION_BIT_COUNT);
  if (bitarray) {
    actor = ih_cor3_actor_create(system, bitarray);
    if (!actor) {
      log = ih_cor3_system_get_log(system);
      ih_audit_log_trace(log, "cor3", "ih_cor3_actor_create");
    }
    ih_core_bitarray_destroy(bitarray);
  } else {
    actor = NULL;
    log = ih_cor3_system_get_log(system);
    ih_audit_log_trace(log, "cor3", "ih_core_bitarray_create_random");
  }

  return actor;
}

void ih_cor3_actor_destroy(void *actor_void)
{
  assert(actor_void);
  ih_cor3_actor_t *actor;

  actor = actor_void;

  ih_core_bitarray_destroy(actor->solution);
  free(actor);
}

void *ih_cor3_actor_get_box_cell(void *actor_void)
{
  assert(actor_void);
  ih_cor3_actor_t *actor;

  actor = actor_void;

  return actor->box_cell;
}

ih_core_bitarray_t *ih_cor3_actor_get_solution(void *actor_void)
{
  assert(actor_void);
  ih_cor3_actor_t *actor;

  actor = actor_void;

  return actor->solution;
}

void ih_cor3_actor_init_actorey(ih_core_actorey_t *actorey)
{
  ih_core_actorey_init(actorey, ih_cor3_actor_create,
      ih_cor3_actor_create_random, ih_cor3_actor_compare_minimize,
      ih_cor3_actor_copy, ih_cor3_actor_destroy, ih_cor3_actor_get_box_cell,
      ih_cor3_actor_set_box_cell);
}

void ih_cor3_actor_set_box_cell(void *actor_void, void *box_cell)
{
  assert(actor_void);
  ih_cor3_actor_t *actor;

  actor = actor_void;

  actor->box_cell = box_cell;
}

ih_core_bool_t in_range(unsigned short index, unsigned short cut,
    unsigned short direction)
{
  ih_core_bool_t in;

  if (direction) {
    if (index >= cut) {
      in = ih_core_bool_true;
    } else {
      in = ih_core_bool_false;
    }
  } else {
    if (index < cut) {
      in = ih_core_bool_true;
    } else {
      in = ih_core_bool_false;
    }
  }

  return in;
}

void set_element(ih_cor3_actor_t *actor, unsigned short x, unsigned short y,
    unsigned short z, unsigned char value)
{
  assert(actor);
  assert(x <= 3);
  assert(y <= 3);
  assert(z <= 3);
  unsigned short i;

  actor->elements[x][y][z] = value;

  i = get_solution_index_from_element_coordinates(x, y, z);
  ih_core_bitarray_set_bit(actor->solution, i, value);

  actor->score_is_valid = ih_core_bool_false;
}
