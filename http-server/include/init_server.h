#ifndef INIT_SERVER_H
#define INIT_SERVER_H

#include "main.h"
#include "utils.h"

#define DOMAIN_IPV4 AF_INET
#define PROTOCOL_TCP SOCK_STREAM
#define PROTOCOL_NUMBER 0
#define LOOPBACK_ADDRESS inet_addr("127.0.0.1");
#define MAX_CONNECTION_BACKLOG 50

int init_server(server_context_t *server_ctx, int port);
void cleanup_server(server_context_t *server_ctx, client_context_t *client_ctx);

#endif