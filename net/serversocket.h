#ifndef ih_net_serversocket_h
#define ih_net_serversocket_h

#include "ih/external/external.h"

int ih_net_serversocket_accept(int serversocket,
    struct sockaddr_in *client_address, socklen_t *client_address_size);

int ih_net_serversocket_create(unsigned short port);

void ih_net_serversocket_destroy(int serversocket);

#endif
