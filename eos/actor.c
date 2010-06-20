#include "ih/core/goal.h"
#include "ih/eos/actor.h"
#include "ih/eos/declarations.h"
#include "ih/eos/system.h"

#define ANARCHY_MODULUS 4
#define GENE_INDEX_DISPLAY 0
#define GENE_INDEX_MEET 1
#define GENE_INDEX_MOVE 2
#define GENE_INDEX_PERSONALITY 3
#define MUTATION 1
#define MUTATION_INCIDENCE_PER 32

struct display_gene_t {
  unsigned long red;
  unsigned long green;
  unsigned long blue;
};
typedef struct display_gene_t display_gene_t;

struct meet_gene_t {
  unsigned long address;
  unsigned long length;
};
typedef struct meet_gene_t meet_gene_t;

struct personality_gene_t {
  unsigned long extrovert;
  unsigned long racist;
};
typedef struct personality_gene_t personality_gene_t;

struct ih_eos_actor_t {
  ih_core_bitarray_t *solution;
  ih_eos_system_t *system;
  double score;
  ih_core_bool_t score_is_valid;
  void *box_cell;
};

static char display_color(ih_eos_actor_t *actor);

static ih_core_bool_t fitness_is_superior(double fitness_a, double fitness_b,
    ih_core_goal_t goal);

static double get_score(ih_eos_actor_t *actor);

static void meet_actor(ih_eos_actor_t *actor_a, ih_eos_actor_t *actor_b);

static void move_actor(ih_eos_actor_t *actor);

static void parse_display_gene(ih_eos_actor_t *actor,
    unsigned long gene_start_address, display_gene_t *display_gene);

static void parse_meet_gene(ih_eos_actor_t *actor,
    unsigned long gene_start_address, meet_gene_t *meet_gene);

static void parse_move_gene(ih_eos_actor_t *actor,
    unsigned long gene_start_address, ih_box_coordinate_t *move_gene);

static void parse_personality_gene(ih_eos_actor_t *actor,
    unsigned long gene_start_address, personality_gene_t *personality_gene);

char display_color(ih_eos_actor_t *actor)
{
  unsigned long display_gene_start_address;
  display_gene_t display_gene;
  char c;

  display_gene_start_address
    = ih_core_get_gene_start_address(actor->solution, GENE_INDEX_DISPLAY);
  parse_display_gene(actor, display_gene_start_address, &display_gene);

  if ((display_gene.red > display_gene.blue)
      && (display_gene.red > display_gene.green)) {
    c = 'r';
  } else if ((display_gene.green > display_gene.red)
      && (display_gene.green > display_gene.blue)) {
    c = 'g';
  } else if ((display_gene.blue > display_gene.green)
      && (display_gene.blue > display_gene.red)) {
    c = 'b';
  } else {
    c = 'w';
  }

  return c;
}

ih_core_bool_t fitness_is_superior(double fitness_a, double fitness_b,
    ih_core_goal_t goal)
{
  ih_core_bool_t superior;

  /*
    TODO: in general, what is the number optimal for society? :: is there a
    percentage, in general, of battles that should be arbitrarily decided, such
    that the optimal amount of diversion from the goal is kept in a culture?
    that diversion from the goal facilitiates diversity that aids in the goal
    :: is there some governing relationship to be found there?
  */
  if (0 == (rand() % ANARCHY_MODULUS)) {
    superior = ih_core_bool_true;
  } else {
    if (IH_CORE_GOAL_MAXIMIZE_SCORE == goal) {
      if (fitness_a > fitness_b) {
        superior = ih_core_bool_true;
      } else {
        superior = ih_core_bool_false;
      }
    } else {
      if (fitness_a < fitness_b) {
        superior = ih_core_bool_true;
      } else {
        superior = ih_core_bool_false;
      }
    }
  }

  return superior;
}

double get_score(ih_eos_actor_t *actor)
{
  assert(actor);
  void *context;
  ih_core_score_solution_f score_solution;
  ih_audit_log_t *log;

  context = ih_eos_system_get_context(actor->system);
  score_solution = ih_eos_system_get_score_solution(actor->system);

  if (!actor->score_is_valid) {
    if (score_solution(context, actor->solution, &actor->score)) {
      actor->score_is_valid = ih_core_bool_true;
    } else {
      log = ih_eos_system_get_log(actor->system);
      ih_audit_log_trace(log, "eos", "score_solution");
    }
  }

  return actor->score;
}

void ih_eos_actor_act(ih_eos_actor_t *actor)
{
  move_actor(actor);
}

int ih_eos_actor_compare_maximize(void *actor_a_void, void *actor_b_void)
{
  assert(actor_a_void);
  assert(actor_b_void);
  ih_eos_actor_t **actor_a;
  ih_eos_actor_t **actor_b;
  double actor_a_score;
  double actor_b_score;
  int compare_result;

  actor_a = (ih_eos_actor_t **) actor_a_void;
  actor_b = (ih_eos_actor_t **) actor_b_void;

  actor_a_score = get_score((ih_eos_actor_t *) *actor_a);
  actor_b_score = get_score((ih_eos_actor_t *) *actor_b);
  if (actor_a_score > actor_b_score) {
    compare_result = -1;
  } else if (actor_a_score < actor_b_score) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

int ih_eos_actor_compare_minimize(void *actor_a_void, void *actor_b_void)
{
  assert(actor_a_void);
  assert(actor_b_void);
  ih_eos_actor_t **actor_a;
  ih_eos_actor_t **actor_b;
  double actor_a_score;
  double actor_b_score;
  int compare_result;

  actor_a = (ih_eos_actor_t **) actor_a_void;
  actor_b = (ih_eos_actor_t **) actor_b_void;

  actor_a_score = get_score((ih_eos_actor_t *) *actor_a);
  actor_b_score = get_score((ih_eos_actor_t *) *actor_b);
  if (actor_a_score < actor_b_score) {
    compare_result = -1;
  } else if (actor_a_score > actor_b_score) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

void *ih_eos_actor_copy(void *actor_void)
{
  ih_core_trace_exit("TODO: implement");
  return NULL;
}

void *ih_eos_actor_create(void *system_void, ih_core_bitarray_t *solution)
{
  assert(system_void);
  assert(solution);
  ih_eos_actor_t *actor;
  ih_eos_system_t *system;
  ih_audit_log_t *log;

  system = system_void;

  actor = malloc(sizeof *actor);
  if (actor) {
    actor->system = system;
    actor->score_is_valid = ih_core_bool_false;
    actor->solution = ih_core_bitarray_copy(solution);
    if (!actor->solution) {
      log = ih_eos_system_get_log(system);
      ih_audit_log_trace(log, "eos", "ih_core_bitarray_copy");
      free(actor);
      actor = NULL;
    }
  } else {
    ih_core_trace_exit("malloc");
  }

  return actor;
}

void *ih_eos_actor_create_random(void *system_void)
{
  assert(system_void);
  ih_core_bitarray_t *bitarray;
  ih_eos_actor_t *actor;
  ih_audit_log_t *log;
  ih_eos_system_t *system;

  system = system_void;

  bitarray = ih_core_bitarray_create_random(IH_CORE_SOLUTION_BIT_COUNT);
  if (bitarray) {
    actor = ih_eos_actor_create(system, bitarray);
    if (!actor) {
      log = ih_eos_system_get_log(system);
      ih_audit_log_trace(log, "eos", "ih_eos_actor_create");
      ih_core_bitarray_destroy(bitarray);
    }
    ih_core_bitarray_destroy(bitarray);
  } else {
    actor = NULL;
    log = ih_eos_system_get_log(system);
    ih_audit_log_trace(log, "eos", "ih_core_bitarray_create_random");
  }

  return actor;
}

void ih_eos_actor_destroy(void *actor_void)
{
  assert(actor_void);
  ih_eos_actor_t *actor;

  actor = actor_void;

  ih_core_bitarray_destroy(actor->solution);
  free(actor);
}

void *ih_eos_actor_get_box_cell(void *actor_void)
{
  assert(actor_void);
  ih_eos_actor_t *actor;

  actor = actor_void;

  return actor->box_cell;
}

ih_core_bitarray_t *ih_eos_actor_get_solution(void *actor_void)
{
  assert(actor_void);
  ih_eos_actor_t *actor;

  actor = actor_void;

  return actor->solution;
}

void ih_eos_actor_init_actorey(ih_core_actorey_t *actorey)
{
  ih_core_actorey_init(actorey,
      ih_eos_actor_create, ih_eos_actor_create_random,
      ih_eos_actor_compare_minimize, ih_eos_actor_copy,
      ih_eos_actor_destroy, ih_eos_actor_get_box_cell,
      ih_eos_actor_set_box_cell);
}

void ih_eos_actor_set_box_cell(void *actor_void, void *box_cell)
{
  assert(actor_void);
  ih_eos_actor_t *actor;

  actor = actor_void;

  actor->box_cell = box_cell;
}

void meet_actor(ih_eos_actor_t *actor_a, ih_eos_actor_t *actor_b)
{
  meet_gene_t meet_gene;
  unsigned long meet_gene_start_address;
  unsigned long each_gene;
  ih_core_bit_t bit;
  ih_box_system_t *box;

  meet_gene_start_address
    = ih_core_get_gene_start_address(actor_a->solution, GENE_INDEX_MEET);
  parse_meet_gene(actor_a, meet_gene_start_address, &meet_gene);

  actor_b->score_is_valid = ih_core_bool_false;

  for (each_gene = meet_gene.address;
       each_gene < (meet_gene.address + meet_gene.length);
       each_gene++) {
    bit = ih_core_bitarray_get_bit(actor_a->solution, each_gene);
    ih_core_bitarray_set_bit(actor_b->solution, each_gene, bit);
    if (MUTATION) {
      if (0 == ih_core_random_unsigned_long(MUTATION_INCIDENCE_PER)) {
        ih_core_bitarray_set_bit(actor_b->solution, each_gene,
            ih_core_random_unsigned_long(2));
      }
    }
  }

  box = ih_eos_system_get_box(actor_a->system);
  ih_box_system_swap(box, actor_a, actor_b);
}

void move_actor(ih_eos_actor_t *actor)
{
  ih_box_coordinate_t move_gene;
  personality_gene_t personality_gene;
  unsigned long move_gene_start_address;
  unsigned long personality_gene_start_address;
  char color_of_possible_friend;
  ih_eos_actor_t *target_actor;
  double actor_score;
  double target_actor_score;
  ih_box_system_t *box;
  ih_box_coordinate_t coordinate;
  ih_core_goal_t goal;

  box = ih_eos_system_get_box(actor->system);

  move_gene_start_address
    = ih_core_get_gene_start_address(actor->solution, GENE_INDEX_MOVE);
  parse_move_gene(actor, move_gene_start_address, &move_gene);

  personality_gene_start_address = ih_core_get_gene_start_address
    (actor->solution, GENE_INDEX_PERSONALITY);
  parse_personality_gene(actor, personality_gene_start_address,
      &personality_gene);

  target_actor = ih_box_system_find_relative(box, actor, &move_gene);

  if (personality_gene.extrovert) {
    color_of_possible_friend = display_color(target_actor);
    if ((!personality_gene.racist)
        || (display_color(actor) == color_of_possible_friend)) {
      actor_score = get_score(actor);
      target_actor_score = get_score(target_actor);
      goal = ih_eos_system_get_goal(actor->system);
      if (fitness_is_superior(actor_score, target_actor_score, goal)) {
        meet_actor(actor, target_actor);
      }
    }
  } else {
    ih_box_coordinate_init_witih_xyz(&coordinate, 2 - move_gene.x,
        2 - move_gene.y, 2 - move_gene.z);
    target_actor = ih_box_system_find_relative(box, actor, &coordinate);
    if (!target_actor) {
      ih_box_system_move_relative(box, actor, &coordinate);
    }
  }
}

void parse_display_gene(ih_eos_actor_t *actor,
    unsigned long gene_start_address, display_gene_t *display_gene)
{
  display_gene->red = ih_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 0);
  display_gene->green = ih_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 8);
  display_gene->blue = ih_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 16);
}

void parse_meet_gene(ih_eos_actor_t *actor, unsigned long gene_start_address,
    meet_gene_t *meet_gene)
{
  meet_gene->address = ih_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 0);
  meet_gene->length = ih_core_bitarray_get_unsigned_char(actor->solution,
      gene_start_address + 8);
}

void parse_move_gene(ih_eos_actor_t *actor, unsigned long gene_start_address,
    ih_box_coordinate_t *move_gene)
{
  assert(actor);
  assert(move_gene);

  move_gene->x = ih_core_bitarray_get_unsigned_char
    (actor->solution, gene_start_address + 0) % 3;
  move_gene->y = ih_core_bitarray_get_unsigned_char
    (actor->solution, gene_start_address + 8) % 3;
  move_gene->z = 1;

}

void parse_personality_gene(ih_eos_actor_t *actor,
    unsigned long gene_start_address, personality_gene_t *personality_gene)
{
  personality_gene->extrovert = ih_core_bitarray_get_bit
    (actor->solution, gene_start_address + 0);
  personality_gene->racist = ih_core_bitarray_get_bit
    (actor->solution, gene_start_address + 1);
}
