#ifndef ih_net_clientsocket_h
#define ih_net_clientsocket_h

int ih_net_clientsocket_create(char *server_ip_address,
    unsigned short server_port);

void ih_net_clientsocket_destroy(int clientsocket);

#endif
