#include "ih/archetype/system.h"
#include "ih/classify/classifyey.h"
#include "ih/classify/system.h"

struct ih_classify_system_t {
  ih_classify_classifyey_t classifyey;
  void *classify_object;
  ih_audit_log_t *log;
};

static void init_classifyey(ih_classify_classifyey_t *classifyey,
    ih_classify_algorithm_t algorithm);

ih_classify_system_t *ih_classify_system_create
(ih_container_array_t *classified_objects, ih_classify_algorithm_t algorithm,
    ih_audit_log_t *log)
{
  assert(classified_objects);
  assert(ih_container_array_get_size(classified_objects) > 0);
  assert(ih_core_bitarray_get_size(ih_container_array_find
          (classified_objects, 0)) >= 2);
  assert(log);
  ih_classify_system_t *system;

  system = malloc(sizeof *system);
  if (system) {
    system->log = log;
    init_classifyey(&system->classifyey, algorithm);
    system->classify_object = system->classifyey.create(classified_objects,
        log);
    if (!system->classify_object) {
      ih_audit_log_trace(log, "clss", "create");
      free(system);
      system = NULL;
    }
  } else {
    ih_core_trace("malloc");
  }

  return system;
}

void ih_classify_system_destroy(ih_classify_system_t *system)
{
  assert(system);

  system->classifyey.destroy(system->classify_object);
  free(system);
}

ih_core_bit_t ih_classify_system_classify_object(ih_classify_system_t *system,
    ih_core_bitarray_t *object)
{
  return system->classifyey.classify_object(system->classify_object, object);
}

ih_container_array_t *ih_classify_system_classify_objects
(ih_classify_system_t *system, ih_container_array_t *objects)
{
  assert(system);
  assert(objects);
  ih_core_bitarray_t *object;
  ih_container_array_t *classes;
  unsigned long object_count;
  ih_core_bit_t class;
  ih_core_bit_t *class_object;
  unsigned long index;

  object_count = ih_container_array_get_size(objects);
  classes = ih_container_array_create(object_count, ih_core_bit_compare,
      ih_core_bit_copy, ih_core_bit_destroy);
  if (classes) {
    ih_container_array_iterate_start(objects);
    index = 0;
    while ((object = ih_container_array_iterate_next(objects))) {
      class
        = system->classifyey.classify_object(system->classify_object, object);
      class_object = malloc(sizeof *class_object);
      if (class_object) {
        ih_container_array_add(classes, index, class_object);
      } else {
        ih_audit_log_trace(system->log, "clss", "malloc");
      }
      index++;
    }
  } else {
    ih_core_trace("ih_container_array_create");
  }

  return classes;
}

ih_core_bool_t ih_classify_system_learn(ih_classify_system_t *system,
    unsigned long max_wall_time_microseconds)
{
  assert(system);
  struct timeval start_time;
  ih_core_bool_t success;

  gettimeofday(&start_time, NULL);

  success = ih_core_bool_true;
  do {
    if (!system->classifyey.learn(system->classify_object)) {
      success = ih_core_bool_false;
      ih_audit_log_trace(system->log, "clss", "learn");
    }
  } while (ih_core_time_is_remaining_microseconds
      (&start_time, max_wall_time_microseconds));

  return success;
}

ih_core_bool_t ih_classify_system_observe_object(ih_classify_system_t *system,
    ih_core_bitarray_t *classified_object)
{
  return system->classifyey.observe_object(system->classify_object,
      classified_object);
}

ih_core_bool_t ih_classify_system_observe_objects(ih_classify_system_t *system,
    ih_container_array_t *classified_objects)
{
  assert(system);
  assert(classified_objects);
  ih_core_bool_t success;
  ih_core_bitarray_t *object;

  success = ih_core_bool_true;

  ih_container_array_iterate_start(classified_objects);
  while ((object = ih_container_array_iterate_next(classified_objects))) {
    if (!system->classifyey.observe_object(system->classify_object, object)) {
      success = ih_core_bool_false;
      ih_audit_log_trace(system->log, "clss", "observe_object");
    }
  }

  return success;
}

void init_classifyey(ih_classify_classifyey_t *classifyey,
    ih_classify_algorithm_t algorithm)
{
  assert(classifyey);

  switch (algorithm) {
    default:
    case IH_CLASSIFY_ALGORITHM_ARCHETYPE:
      ih_archetype_system_init_classifyey(classifyey);
      break;
  }
}
