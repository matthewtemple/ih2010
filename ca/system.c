#include "ih/ca/system.h"
#include "ih/core/constants.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/file/basic.h"

struct ih_ca_system_t {
  ih_ca_systemey_t *systemey;

  ih_container_array_t *state_history;
  unsigned long current_time_step;
  unsigned long highest_cell_count;

  unsigned long random_seed;
  ih_core_bool_t random_seed_is_set;

  void *context_object;
};

static ih_core_bool_t create_new_current_state(ih_ca_system_t *system);

static ih_ca_state_t *get_current_state(ih_ca_system_t *system);

static GLOBAL(void) write_jpeg_file(char *filename, int quality,
    JSAMPLE *image_buffer, int image_height, int image_width);

ih_ca_system_t *ih_ca_system_create
(ih_container_array_t *initial_state_history,
    unsigned long initial_time_step_count, ih_ca_systemey_t *systemey)
{
  assert(initial_state_history);
  assert(ih_container_array_get_size(initial_state_history) > 0);
  assert(systemey);
  ih_ca_system_t *system;
  ih_ca_state_t *state;
  ih_ca_state_t *state_copy;
  unsigned long initial_state_history_size;
  unsigned long initial_state_history_array_size;
  unsigned long time_step;
  ih_core_bool_t so_far_so_good;
  unsigned long state_cell_count;

  system = malloc(sizeof *system);
  if (system) {
    so_far_so_good = ih_core_bool_true;

    system->systemey = systemey;
    system->random_seed_is_set = ih_core_bool_false;
    system->highest_cell_count = 0;

    initial_state_history_size
      = ih_container_array_get_size(initial_state_history);
    initial_state_history_array_size = initial_state_history_size
      + initial_time_step_count;
    system->state_history = ih_container_array_create
      (initial_state_history_array_size, ih_ca_state_compare,
          ih_ca_state_copy, ih_ca_state_destroy);
    if (system->state_history) {

      time_step = 0;
      ih_container_array_iterate_start(initial_state_history);
      while ((state = ih_container_array_iterate_next(initial_state_history))) {

        state_copy = ih_ca_state_copy(state);
        ih_container_array_add(system->state_history, time_step, state_copy);
        time_step++;

        state_cell_count = ih_ca_state_get_cell_count(state);
        ih_core_note_maximum_unsigned_long(&system->highest_cell_count,
            state_cell_count);

      }

      system->current_time_step = time_step - 1;
    } else {
      free(system);
      system = NULL;
    }
  } else {
    so_far_so_good = ih_core_bool_false;
    ih_core_trace("malloc");
  }

  if (so_far_so_good) {
    if (systemey->create_context) {
      system->context_object = systemey->create_context(systemey->name_object);
      if (!system->context_object) {
        so_far_so_good = ih_core_bool_false;
        ih_core_trace("create_context");
      }
    }
  }

  if (!so_far_so_good && system) {
    free(system);
    system = NULL;
  }

  assert(!system || system->state_history);
  return system;
}

void ih_ca_system_default_get_cell_color(ih_ca_t *cell,
    ih_core_color_t *color)
{
  assert(cell);
  assert(color);

  color->red = cell->value;
  color->green = cell->value;
  color->blue = cell->value;
}

void ih_ca_system_destroy(ih_ca_system_t *system)
{
  assert(system);

  if (system->systemey->destroy_context) {
    system->systemey->destroy_context(system->context_object);
  }
  ih_container_array_destroy(system->state_history);
  free(system);
}

unsigned long ih_ca_system_get_age_in_time_steps(ih_ca_system_t *system)
{
  return system->current_time_step;
}

unsigned long ih_ca_system_get_cell_count(ih_ca_system_t *system)
{
  return ih_ca_state_get_cell_count(get_current_state(system));
}

ih_ca_t *ih_ca_system_get_cell(ih_ca_system_t *system,
    unsigned long time_step, unsigned long cell_index)
{
  assert(system);
  ih_ca_state_t *state;
  ih_ca_t *cell;

  state = ih_container_array_find(system->state_history, time_step);
  cell = ih_ca_state_get_cell(state, cell_index);

  return cell;
}

void ih_ca_system_get_cell_color_binary(ih_ca_t *cell,
    ih_core_color_t *color)
{
  assert(cell);
  assert(color);

  if (0 == (cell->value % 2)) {
    color->red = IH_CORE_MAX_COLOR;
    color->green = IH_CORE_MAX_COLOR;
    color->blue = IH_CORE_MAX_COLOR;
  } else {
    color->red = 0;
    color->green = 0;
    color->blue = 0;
  }
}

void *ih_ca_system_get_context(ih_ca_system_t *system)
{
  return system->context_object;
}

ih_ca_t *ih_ca_system_get_current_cell(ih_ca_system_t *system,
    unsigned long cell_index)
{
  assert(system);
  ih_ca_state_t *state;
  ih_ca_t *cell;

  state = get_current_state(system);
  cell = ih_ca_state_get_cell(state, cell_index);

  return cell;
}

ih_ca_state_t *ih_ca_system_get_current_state(ih_ca_system_t *system)
{
  return get_current_state(system);
}

void *ih_ca_system_get_name(ih_ca_system_t *system)
{
  return system->systemey->name_object;
}

ih_ca_t *ih_ca_system_get_relative_cell(ih_ca_system_t *system,
    long relative_time_step, unsigned long cell_index)
{
  return ih_ca_system_get_cell(system,
      system->current_time_step + relative_time_step, cell_index);
}

ih_ca_state_t *ih_ca_system_get_state(ih_ca_system_t *system,
    unsigned long time_step)
{
  return ih_container_array_find(system->state_history, time_step);
}

void ih_ca_system_run(ih_ca_system_t *system, ih_core_run_t *run)
{
  assert(system);
  assert(run);
  unsigned long cell_index;
  unsigned long cell_count;
  ih_ca_state_t *current_state;
  unsigned long state_history_size;
  ih_ca_t new_cell_state;

  if (system->random_seed_is_set) {
    ih_core_seed_random(system->random_seed);
  }

  while (!ih_core_run_is_complete(run)) {
    current_state = get_current_state(system);
    cell_count = ih_ca_state_get_cell_count(current_state);

    state_history_size = ih_container_array_get_size(system->state_history);
    if (create_new_current_state(system)) {
      if (system->systemey->start_time_step) {
        system->systemey->start_time_step(system);
      }
      for (cell_index = 0; cell_index < cell_count; cell_index++) {
        new_cell_state = system->systemey->calculate_new_cell_state
          (system, cell_index);
        ih_ca_system_set_cell(system, system->current_time_step + 1,
            cell_index, &new_cell_state);
        if (ih_core_bool_false) {
          printf("%lu", new_cell_state.value);
        }
      }
      if (ih_core_bool_false) {
        printf("\n");
      }
      if (system->systemey->end_time_step) {
        system->systemey->end_time_step(system);
      }
      system->current_time_step++;
      ih_core_run_increment_iterations(run);
    } else {
      ih_core_trace("create_new_current_state");
      break;
    }
  }
}

ih_core_bool_t ih_ca_system_save_snapshot_jpeg(ih_ca_system_t *system,
    char *filename)
{
  assert(system);
  assert(filename);
  JSAMPLE *image_buffer;
  ih_core_bool_t success;
  unsigned long step_count;
  unsigned long cell_count;
  ih_core_color_t color;
  ih_ca_state_t *state;
  ih_ca_t *cell;
  unsigned long each_step;
  unsigned long each_cell;

  cell_count = system->highest_cell_count;
  step_count = ih_container_array_get_size(system->state_history);

  image_buffer = malloc(sizeof(JSAMPLE) * cell_count * step_count * 3);
  if (image_buffer) {
    success = ih_core_bool_true;

    each_step = 0;
    ih_container_array_iterate_start(system->state_history);
    while ((state = ih_container_array_iterate_next(system->state_history))) {
      for (each_cell = 0; each_cell < cell_count; each_cell++) {
        cell = ih_ca_state_get_cell(state, each_cell);

        system->systemey->get_cell_color(cell, &color);

        image_buffer[(each_step * cell_count * 3)
            + (each_cell * 3) + 0] = color.red;
        image_buffer[(each_step * cell_count * 3)
            + (each_cell * 3) + 1] = color.green;
        image_buffer[(each_step * cell_count * 3)
            + (each_cell * 3) + 2] = color.blue;

      }
      each_step++;
    }

    write_jpeg_file(filename, 100, image_buffer, step_count, cell_count);
    free(image_buffer);
  } else {
    success = ih_core_bool_false;
    ih_core_trace("malloc");
  }

  return success;
}

ih_core_bool_t ih_ca_system_save_snapshot_text(ih_ca_system_t *system,
    char *filename)
{
  assert(system);
  ih_ca_state_t *state;
  unsigned long cell_count;
  unsigned long each_cell;
  unsigned long cell_value;
  ih_file_basic_t *file;
  ih_core_bool_t success;
  int c;

  success = ih_core_bool_true;

  file = ih_file_basic_create
    (filename, IH_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE);
  if (file) {
    ih_container_array_iterate_start(system->state_history);
    while ((state = ih_container_array_iterate_next(system->state_history))) {
      cell_count = ih_ca_state_get_cell_count(state);
      for (each_cell = 0; each_cell < cell_count; each_cell++) {
        cell_value = ih_ca_state_get_cell_value(state, each_cell);

        /*
          TODO: make this into a named function
        */
        if (cell_value < 10) {
          c = 48 + cell_value;
        } else {
          c = 120;
        }

        if (!ih_file_basic_write_char(file, c)) {
          success = ih_core_bool_false;
          break;
        }
      }
      if (!ih_file_basic_write_char(file, '\n')) {
        success = ih_core_bool_false;
        break;
      }
    }
    ih_file_basic_destroy(file);
  } else {
    success = ih_core_bool_false;
    ih_core_trace("ih_file_basic_create");
  }

  return success;
}

void ih_ca_system_set_cell(ih_ca_system_t *system, unsigned long time_step,
    unsigned long cell_index, ih_ca_t *cell)
{
  assert(system);
  ih_ca_state_t *state;

  state = ih_container_array_find(system->state_history, time_step);
  ih_ca_state_set_cell(state, cell_index, cell);
}

void ih_ca_system_set_current_cell(ih_ca_system_t *system,
    unsigned long cell_index, ih_ca_t *cell)
{
  assert(system);
  ih_ca_state_t *state;

  state = get_current_state(system);
  ih_ca_state_set_cell(state, cell_index, cell);
}

void ih_ca_system_set_random_seed(ih_ca_system_t *system, unsigned long seed)
{
  assert(system);

  system->random_seed = seed;
  system->random_seed_is_set = ih_core_bool_true;
}

void ih_ca_system_set_relative_cell(ih_ca_system_t *system,
    long relative_time_step, unsigned long cell_index, ih_ca_t *cell)
{
  ih_ca_system_set_cell(system,
      system->current_time_step + relative_time_step, cell_index, cell);
}

ih_core_bool_t create_new_current_state(ih_ca_system_t *system)
{
  assert(system);
  ih_core_bool_t success;
  ih_ca_state_t *old_current_state;
  ih_ca_state_t *new_current_state;

  old_current_state = get_current_state(system);
  new_current_state = ih_ca_state_copy(old_current_state);
  if (new_current_state) {
    success = ih_core_bool_true;
    ih_container_array_add(system->state_history, system->current_time_step + 1,
        new_current_state);
  } else {
    success = ih_core_bool_false;
    ih_core_trace("ih_ca_state_copy");
  }

  return success;
}

ih_ca_state_t *get_current_state(ih_ca_system_t *system)
{
  return ih_container_array_find(system->state_history,
      system->current_time_step);
}

GLOBAL(void) write_jpeg_file(char *filename, int quality,
    JSAMPLE *image_buffer, int image_height, int image_width)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *outfile;
  JSAMPROW row_pointer[1];
  int row_stride;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);

  if ((outfile = fopen(filename, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }
  jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width = image_width;
  cinfo.image_height = image_height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE);

  jpeg_start_compress(&cinfo, TRUE);
  row_stride = image_width * 3;

  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  fclose(outfile);

  jpeg_destroy_compress(&cinfo);
}
