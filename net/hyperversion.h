#ifndef ih_net_hyperversion_h
#define ih_net_hyperversion_h

enum ih_net_hyperversion_t {
  IH_NET_HYPERVERSION_UNKNOWN,
  IH_NET_HYPERVERSION_1_0,
  IH_NET_HYPERVERSION_1_1,
};
typedef enum ih_net_hyperversion_t ih_net_hyperversion_t;

const char *ih_net_hyperversion_get_name(ih_net_hyperversion_t hyperversion);

#endif
