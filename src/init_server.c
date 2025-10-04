#include <stdint.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "init_server.h"

void init_server(server_context_t *p_server_ctx, uint16_t port) {

    // ** Initialize the server_addr struct
    p_server_ctx->server_addr.sin_family = IPV4_AF;
    p_server_ctx->server_addr.sin_addr.s_addr = LOOPBACK_ADDRESS;
    p_server_ctx->server_addr.sin_port = htons(port); // ** Converts the port number from host byte order to network byte order
    p_server_ctx->server_addr_len = sizeof(p_server_ctx->server_addr);

    // ** Create the socket
    create_socket(p_server_ctx);

    // ** Bind IP address and port to socket
    bind_socket(p_server_ctx);

    // ** Start listening for incoming connections
    listen_socket(p_server_ctx);

    printf("Listening for incoming connections on: %s, port: %d\n",
            inet_ntoa(p_server_ctx->server_addr.sin_addr), // ** Converts the ip address from network byte order to host byte order (not thread-safe)
            ntohs(p_server_ctx->server_addr.sin_port)); // ** Converts the port number from network byte order to host byte order
}

void create_socket(server_context_t *p_server_ctx) {
    p_server_ctx->server_fd = socket(IPV4_AF, PROTOCOL_TCP, TCP_PROTOCOL_NUMBER);
    if (p_server_ctx->server_fd == -1) {
        perror("create_socket");
        exit(EXIT_FAILURE);
    }
}

void bind_socket(server_context_t *p_server_ctx) {
    uint8_t socket_option = 1;
    // ** This socket option allows the server to bind to the same address and port even if the socket is in a TIME_WAIT state
    setsockopt(p_server_ctx->server_fd, SOL_SOCKET, SO_REUSEADDR, &socket_option, sizeof(socket_option));

    if (bind(p_server_ctx->server_fd, (struct sockaddr *)&p_server_ctx->server_addr, p_server_ctx->server_addr_len) == -1) {
        if (p_server_ctx->server_fd >= 0) {
            close(p_server_ctx->server_fd);
        }
        perror("bind_socket");
        exit(EXIT_FAILURE);
    }
}

void listen_socket(server_context_t *p_server_ctx) {
    if (listen(p_server_ctx->server_fd, MAX_CONNECTION_BACKLOG) == -1) {
        if (p_server_ctx->server_fd >= 0) {
            close(p_server_ctx->server_fd);
        }
        perror("listen_socket");
        exit(EXIT_FAILURE);
    }
}