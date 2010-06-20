#include "ih/ca/ant.h"
#include "ih/ca/clear.h"
#include "ih/ca/crow.h"
#include "ih/ca/eca.h"
#include "ih/ca/fly.h"
#include "ih/ca/k3.h"
#include "ih/ca/malice.h"
#include "ih/ca/system.h"
#include "ih/ca/tools.h"
#include "ih/ca/water.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

static void randomize_ant_name(char *name);

static void randomize_malice_name(char *name);

static void randomize_water_name(char *name);

static void test_ant(char *name);

static void test_clear(unsigned short name);

static void test_crow(unsigned short name);

static void test_eca(unsigned short name);

/*
static void test_fly(char *name);
*/

static void test_k3(unsigned long name);

static void test_malice(char *name);

static void test_water(char *name);

void randomize_ant_name(char *name)
{
  assert(name);
  unsigned short each_char;

  for (each_char = 0; each_char < 64; each_char++) {
    if (ih_core_coin_toss()) {
      *(name + each_char) = '1';
    } else {
      *(name + each_char) = '0';
    }
  }
}

void randomize_malice_name(char *name)
{
  assert(name);
  unsigned short each_char;

  for (each_char = 0; each_char < 64; each_char++) {
    if (ih_core_coin_toss()) {
      *(name + each_char) = '1';
    } else {
      *(name + each_char) = '0';
    }
  }
}

void randomize_water_name(char *name)
{
  assert(name);
  unsigned short each_char;

  for (each_char = 0; each_char < 512; each_char++) {
    if (ih_core_coin_toss()) {
      *(name + each_char) = '1';
    } else {
      *(name + each_char) = '0';
    }
  }
}

void test_ant(char *name)
{
  ih_ca_system_t *cell_system;
  ih_container_array_t *initial_state_history;
  unsigned long initial_time_step_count;
  unsigned long cell_count;
  ih_core_run_t *run;
  char *filename;
  ih_ca_systemey_t cell_systemey;

  cell_count = 256;
  initial_time_step_count = 256;
  initial_state_history
    = ih_ca_create_initial_state_single_cell_binary(cell_count, 2);

  ih_ca_ant_init_systemey(&cell_systemey, name);

  cell_system = ih_ca_system_create(initial_state_history,
      initial_time_step_count, &cell_systemey);
  if (!cell_system) {
    ih_core_trace("ih_ca_system_create");
  }

  run = ih_core_run_create(IH_CORE_RUN_STYLE_ITERATIONS);
  if (!run) {
    ih_core_trace_exit("ih_core_run_create");
  }
  ih_core_run_set_max_iterations(run, initial_time_step_count);

  ih_ca_system_run(cell_system, run);

  filename = malloc(128);
  if (!filename) {
    ih_core_trace_exit("malloc");
  }

  sprintf(filename, "snapshots/ant/%s.jpg", name);

  if (!ih_ca_system_save_snapshot_jpeg(cell_system, filename)) {
    ih_core_trace("ih_ca_system_save_snapshot_jpeg");
  }

  free(filename);

  ih_ca_system_destroy(cell_system);
  ih_core_run_destroy(run);
  ih_container_array_destroy(initial_state_history);
}

void test_clear(unsigned short name)
{
  ih_ca_system_t *cell_system;
  ih_container_array_t *initial_state_history;
  unsigned long initial_time_step_count;
  unsigned long cell_count;
  ih_core_run_t *run;
  char *filename;
  ih_ca_systemey_t cell_systemey;

  cell_count = 256;
  initial_time_step_count = 256;
  initial_state_history
    = ih_ca_create_initial_state_single_cell_binary(cell_count, 1);

  ih_ca_clear_init_systemey(&cell_systemey, &name);

  cell_system = ih_ca_system_create(initial_state_history,
      initial_time_step_count, &cell_systemey);
  if (!cell_system) {
    ih_core_trace("ih_ca_system_create");
  }

  run = ih_core_run_create(IH_CORE_RUN_STYLE_ITERATIONS);
  if (!run) {
    ih_core_trace_exit("ih_core_run_create");
  }
  ih_core_run_set_max_iterations(run, initial_time_step_count);

  ih_ca_system_run(cell_system, run);

  filename = malloc(128);
  if (!filename) {
    ih_core_trace_exit("malloc");
  }

  sprintf(filename, "snapshots/clear/%03i.jpg", name);

  if (!ih_ca_system_save_snapshot_jpeg(cell_system, filename)) {
    ih_core_trace("ih_ca_system_save_snapshot_jpeg");
  }

  free(filename);

  ih_ca_system_destroy(cell_system);
  ih_core_run_destroy(run);
  ih_container_array_destroy(initial_state_history);
}

void test_crow(unsigned short name)
{
  ih_ca_system_t *cell_system;
  ih_container_array_t *initial_state_history;
  unsigned long initial_time_step_count;
  unsigned long cell_count;
  ih_core_run_t *run;
  char *filename;
  ih_ca_systemey_t cell_systemey;

  cell_count = 128;
  initial_time_step_count = 128;
  initial_state_history
    = ih_ca_create_initial_state_single_cell_binary(cell_count, 2);

  ih_ca_crow_init_systemey(&cell_systemey, &name);

  cell_system = ih_ca_system_create(initial_state_history,
      initial_time_step_count, &cell_systemey);
  if (!cell_system) {
    ih_core_trace("ih_ca_system_create");
  }

  run = ih_core_run_create(IH_CORE_RUN_STYLE_ITERATIONS);
  if (!run) {
    ih_core_trace_exit("ih_core_run_create");
  }
  ih_core_run_set_max_iterations(run, initial_time_step_count);

  ih_ca_system_run(cell_system, run);

  filename = malloc(128);
  if (!filename) {
    ih_core_trace_exit("malloc");
  }

  sprintf(filename, "snapshots/crow/%05i.jpg", name);

  if (!ih_ca_system_save_snapshot_jpeg(cell_system, filename)) {
    ih_core_trace("ih_ca_system_save_snapshot_jpeg");
  }

  free(filename);

  ih_ca_system_destroy(cell_system);
  ih_core_run_destroy(run);
  ih_container_array_destroy(initial_state_history);
}

void test_eca(unsigned short name)
{
  ih_ca_system_t *cell_system;
  ih_container_array_t *initial_state_history;
  unsigned long initial_time_step_count;
  unsigned long cell_count;
  ih_core_run_t *run;
  char *filename;
  ih_ca_systemey_t cell_systemey;

  cell_count = 79;
  initial_time_step_count = 25;
  initial_state_history
    = ih_ca_create_initial_state_single_cell_binary(cell_count, 1);

  ih_ca_eca_init_systemey(&cell_systemey, &name);

  cell_system = ih_ca_system_create(initial_state_history,
      initial_time_step_count, &cell_systemey);
  if (!cell_system) {
    ih_core_trace("ih_ca_system_create");
  }

  run = ih_core_run_create(IH_CORE_RUN_STYLE_ITERATIONS);
  if (!run) {
    ih_core_trace_exit("ih_core_run_create");
  }
  ih_core_run_set_max_iterations(run, initial_time_step_count);

  ih_ca_system_run(cell_system, run);

  filename = malloc(128);
  if (!filename) {
    ih_core_trace_exit("malloc");
  }

  sprintf(filename, "snapshots/eca/%03i.jpg", name);

  if (!ih_ca_system_save_snapshot_jpeg(cell_system, filename)) {
    ih_core_trace("ih_ca_system_save_snapshot_jpeg");
  }

  free(filename);

  ih_ca_system_destroy(cell_system);
  ih_core_run_destroy(run);
  ih_container_array_destroy(initial_state_history);
}

/*
void test_fly(char *name)
{
  ih_ca_system_t *cell_system;
  ih_container_array_t *initial_state_history;
  unsigned long initial_time_step_count;
  unsigned long cell_count;
  ih_core_run_t *run;
  char *filename;
  ih_ca_systemey_t cell_systemey;

  cell_count = 256;
  initial_time_step_count = 256;
  initial_state_history
    = ih_ca_create_initial_state_single_cell_binary(cell_count, 2);

  ih_ca_fly_init_systemey(&cell_systemey, name);

  cell_system = ih_ca_system_create(initial_state_history,
      initial_time_step_count, &cell_systemey);
  if (!cell_system) {
    ih_core_trace("ih_ca_system_create");
  }

  run = ih_core_run_create(IH_CORE_RUN_STYLE_ITERATIONS);
  if (!run) {
    ih_core_trace_exit("ih_core_run_create");
  }
  ih_core_run_set_max_iterations(run, initial_time_step_count);

  ih_ca_system_run(cell_system, run);

  filename = malloc(128);
  if (!filename) {
    ih_core_trace_exit("malloc");
  }

  sprintf(filename, "snapshots/fly/%s.jpg", name);

  if (!ih_ca_system_save_snapshot_jpeg(cell_system, filename)) {
    ih_core_trace("ih_ca_system_save_snapshot_jpeg");
  }

  free(filename);

  ih_ca_system_destroy(cell_system);
  ih_core_run_destroy(run);
  ih_container_array_destroy(initial_state_history);
}
*/

void test_k3(unsigned long name)
{
  ih_ca_system_t *cell_system;
  ih_container_array_t *initial_state_history;
  unsigned long initial_time_step_count;
  unsigned long cell_count;
  ih_core_run_t *run;
  char *filename;
  ih_ca_systemey_t cell_systemey;

  filename = malloc(128);
  if (!filename) {
    ih_core_trace_exit("malloc");
  }

  cell_count = 128;
  initial_time_step_count = 128;
  initial_state_history
    = ih_ca_create_initial_state_single_cell_k3(cell_count);

  ih_ca_k3_init_systemey(&cell_systemey, &name);

  cell_system = ih_ca_system_create(initial_state_history,
      initial_time_step_count, &cell_systemey);
  if (!cell_system) {
    ih_core_trace("ih_ca_system_create");
  }

  run = ih_core_run_create(IH_CORE_RUN_STYLE_ITERATIONS);
  if (!run) {
    ih_core_trace_exit("ih_core_run_create");
  }
  ih_core_run_set_max_iterations(run, initial_time_step_count);

  ih_ca_system_run(cell_system, run);

  sprintf(filename, "snapshots/k3/%08lu.jpg", name);

  if (!ih_ca_system_save_snapshot_jpeg(cell_system, filename)) {
    ih_core_trace("ih_ca_system_save_snapshot_jpeg");
  }

  ih_ca_system_destroy(cell_system);
  ih_core_run_destroy(run);
  ih_container_array_destroy(initial_state_history);
  free(filename);
}

void test_malice(char *name)
{
  ih_ca_system_t *cell_system;
  ih_container_array_t *initial_state_history;
  unsigned long initial_time_step_count;
  unsigned long cell_count;
  ih_core_run_t *run;
  char *filename;
  ih_ca_systemey_t cell_systemey;

  cell_count = 128;
  initial_time_step_count = 256;
  initial_state_history
    = ih_ca_create_initial_state_single_cell_binary(cell_count, 2);

  ih_ca_malice_init_systemey(&cell_systemey, name);

  cell_system = ih_ca_system_create(initial_state_history,
      initial_time_step_count, &cell_systemey);
  if (!cell_system) {
    ih_core_trace("ih_ca_system_create");
  }

  run = ih_core_run_create(IH_CORE_RUN_STYLE_ITERATIONS);
  if (!run) {
    ih_core_trace_exit("ih_core_run_create");
  }
  ih_core_run_set_max_iterations(run, initial_time_step_count);

  ih_ca_system_run(cell_system, run);

  filename = malloc(128);
  if (!filename) {
    ih_core_trace_exit("malloc");
  }

  sprintf(filename, "snapshots/malice/%s.jpg", name);

  if (!ih_ca_system_save_snapshot_jpeg(cell_system, filename)) {
    ih_core_trace("ih_ca_system_save_snapshot_jpeg");
  }

  free(filename);

  ih_ca_system_destroy(cell_system);
  ih_core_run_destroy(run);
  ih_container_array_destroy(initial_state_history);
}

void test_water(char *name)
{
  ih_ca_system_t *cell_system;
  ih_container_array_t *initial_state_history;
  unsigned long initial_time_step_count;
  unsigned long cell_count;
  ih_core_run_t *run;
  char *filename;
  ih_ca_systemey_t cell_systemey;

  cell_count = 256;
  initial_time_step_count = 512;
  initial_state_history
    = ih_ca_create_initial_state_single_cell_binary(cell_count, 3);

  ih_ca_water_init_systemey(&cell_systemey, name);

  cell_system = ih_ca_system_create(initial_state_history,
      initial_time_step_count, &cell_systemey);
  if (!cell_system) {
    ih_core_trace("ih_ca_system_create");
  }

  run = ih_core_run_create(IH_CORE_RUN_STYLE_ITERATIONS);
  if (!run) {
    ih_core_trace_exit("ih_core_run_create");
  }
  ih_core_run_set_max_iterations(run, initial_time_step_count);

  ih_ca_system_run(cell_system, run);

  filename = malloc(128);
  if (!filename) {
    ih_core_trace_exit("malloc");
  }

  ih_core_truncate_string(name, 64);
  sprintf(filename, "snapshots/water/%s.jpg", name);

  if (!ih_ca_system_save_snapshot_jpeg(cell_system, filename)) {
    ih_core_trace("ih_ca_system_save_snapshot_jpeg");
  }

  free(filename);

  ih_ca_system_destroy(cell_system);
  ih_core_run_destroy(run);
  ih_container_array_destroy(initial_state_history);
}

int main(int argc, char *argv[])
{
  unsigned long i;
  char ant_name[65];
  char malice_name[65];
  char water_name[513];

  ih_core_seed_random_witih_time();

  if (ih_core_bool_false) {

    ant_name[64] = '\0';
    for (i = 0; i < 32; i++) {
      randomize_ant_name(ant_name);
      test_ant(ant_name);
      /*  printf("%s\n", ant_name);  */
    }

    for (i = 0; i < 256; i++) {
      test_clear(i);
    }

    test_crow(26715);
    test_crow(28050);

    for (i = 0; i < 256; i++) {
      test_eca(i);
    }

    test_k3(0);
    test_k3(782563);
    test_k3(4438761);
    test_k3(6618712);
    test_k3(8388608);
    test_k3(9615746);
    test_k3(11145723);
    test_k3(11222444);
    test_k3(11416532);
    test_k3(11417530);
    test_k3(11417532);
    test_k3(12256311);
    test_k3(12410031);
    test_k3(14817253);
    test_k3(15736917);
    test_k3(16777215);
    if (ih_core_bool_false) {
      for (i = 0; i < 32; i++) {
        test_k3(random() % 16777216);
      }
    }

    malice_name[64] = '\0';
    for (i = 0; i < 32; i++) {
      randomize_malice_name(malice_name);
      test_malice(malice_name);
      /*  printf("%s\n", malice_name);  */
    }

    water_name[512] = '\0';
    for (i = 0; i < 32; i++) {
      randomize_water_name(water_name);
      test_water(water_name);
      /*  printf("%s\n", water_name);  */
    }

  }

  return 0;
}
