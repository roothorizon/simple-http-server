#ifndef INIT_SERVER_H
#define INIT_SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>

#include "main.h"

#define IPV4_AF AF_INET
#define PROTOCOL_TCP SOCK_STREAM
#define TCP_PROTOCOL_NUMBER 6

void init_server(server_context_t *p_server_ctx, uint16_t port);
void create_socket(server_context_t *p_server_ctx);
void bind_socket(server_context_t *p_server_ctx);
void listen_socket(server_context_t *p_server_ctx);

#endif