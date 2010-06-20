#ifndef ih_net_engine_h
#define ih_net_engine_h

#define IH_NET_ENGINE_NO_GET_NAME_FUNCTION NULL

#define IH_NET_ENGINE_TYPE_COUNT 5
enum ih_net_engine_id_t {
  IH_NET_ENGINE_UNKNOWN = 0,
  IH_NET_ENGINE_TEST = 1,
  IH_NET_ENGINE_PING = 2,
  IH_NET_ENGINE_SHELL = 3,
  IH_NET_ENGINE_HYPER = 4,
  IH_NET_ENGINE_MAX = 31,
};
typedef enum ih_net_engine_id_t ih_net_engine_id_t;

typedef char *(*ih_net_engine_get_name_f)(unsigned long engine_id);

char *ih_net_engine_get_name(unsigned long engine_id,
    ih_net_engine_get_name_f get_engine_name);

#endif
