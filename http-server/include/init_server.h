#ifndef INIT_SERVER_H
#define INIT_SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>

#include "main.h"

#include <arpa/inet.h>

#define IPV4_AF AF_INET
#define PROTOCOL_TCP SOCK_STREAM
#define TCP_PROTOCOL_NUMBER 6
#define LOOPBACK_ADDRESS inet_addr("127.0.0.1");
#define MAX_CONNECTION_BACKLOG 50

void init_server(server_context_t *server_ctx, int port);
void create_socket(server_context_t *server_ctx);
void bind_socket(server_context_t *server_ctx);
void listen_socket(server_context_t *server_ctx);

#endif