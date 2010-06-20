#include "ih/core/types.h"
#include "ih/external/external.h"
#include "ih/net/enginey.h"

void ih_net_enginey_init(ih_net_enginey_t *enginey,
    ih_net_enginey_create_f create, ih_net_enginey_destroy_f destroy,
    ih_net_enginey_get_handler_for_message_f get_handler_for_message,
    ih_net_enginey_maintain_f maintain, ih_net_enginey_run_f run,
    ih_net_enginey_start_f start, ih_net_enginey_stop_f stop)
{
  assert(enginey);

  enginey->create = create;
  enginey->destroy = destroy;
  enginey->get_handler_for_message = get_handler_for_message;
  enginey->maintain = maintain;
  enginey->run = run;
  enginey->start = start;
  enginey->stop = stop;
}
