#include "ih/core/shard.h"
#include "ih/core/types.h"
#include "ih/external/external.h"

void ih_core_shard_init(ih_core_shard_t *shard, unsigned long id,
    void *object)
{
  assert(shard);
  shard->id = id;
  shard->object = object;
}
