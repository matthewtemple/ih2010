#ifndef ih_net_webrequest_h
#define ih_net_webrequest_h

struct ih_net_webrequest_t;
typedef struct ih_net_webrequest_t ih_net_webrequest_t;

ih_net_webrequest_t *ih_net_webrequest_create();

void ih_net_webrequest_destroy(ih_net_webrequest_t *webrequest);

#endif
