#ifndef ih_core_shard_h
#define ih_core_shard_h

#include "ih/core/object.h"

struct ih_core_shard_t {
  unsigned long id;
  void *object;
};
typedef struct ih_core_shard_t ih_core_shard_t;

void ih_core_shard_init(ih_core_shard_t *shard, unsigned long id, void *object);

#endif
