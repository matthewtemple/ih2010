#include "ih/classify/classifyey.h"
#include "ih/core/tools.h"

void ih_classify_classifyey_free(ih_classify_classifyey_t *classifyey) {}

void ih_classify_classifyey_init(ih_classify_classifyey_t *classifyey,
    ih_classify_classifyey_create_f create,
    ih_classify_classifyey_destroy_f destroy,
    ih_classify_classifyey_classify_object_f classify_object,
    ih_classify_classifyey_learn_f learn,
    ih_classify_classifyey_observe_object_f observe_object)
{
  assert(classifyey);
  assert(create);
  assert(destroy);
  assert(classify_object);
  assert(learn);
  assert(observe_object);

  classifyey->create = create;
  classifyey->destroy = destroy;
  classifyey->classify_object = classify_object;
  classifyey->learn = learn;
  classifyey->observe_object = observe_object;
}
