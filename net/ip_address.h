#ifndef ih_net_ip_address_h
#define ih_net_ip_address_h

struct ih_net_ip_address_t;
typedef struct ih_net_ip_address_t ih_net_ip_address_t;

ih_net_ip_address_t *ih_net_ip_address_create_from_string(char *ip_address);

void ih_net_ip_address_destroy();

#endif
