#include "ih/net/hyperversion.h"

const char *ih_net_hyperversion_get_name(ih_net_hyperversion_t hyperversion)
{
  const char *name;

  switch (hyperversion) {

    case IH_NET_HYPERVERSION_1_0:
      name = "HTTP/1.0";
      break;

    case IH_NET_HYPERVERSION_1_1:
      name = "HTTP/1.1";
      break;

    case IH_NET_HYPERVERSION_UNKNOWN:
    default:
      name = "";
      break;
  }

  return name;
}
