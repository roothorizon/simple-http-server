#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>

#include "utils.h"
#include "init_server.h"

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>

void init_server(server_context_t *server_ctx, int port) {

    // Initialize the server_addr struct
    server_ctx->server_addr.sin_family = IPV4_AF;
    server_ctx->server_addr.sin_addr.s_addr = LOOPBACK_ADDRESS;
    server_ctx->server_addr.sin_port = htons(port);
    server_ctx->server_addr_len = sizeof(server_ctx->server_addr);

    // Create the socket
    create_socket(server_ctx);

    // Bind IP address and port to socket
    bind_socket(server_ctx);

    // Start listening for incoming connections
    listen_socket(server_ctx);

    printf("Listening for incoming connections on: %s, port: %d\n",
            inet_ntoa(server_ctx->server_addr.sin_addr), // Converts the ip address from network byte order to host byte order (not thread-safe)
            ntohs(server_ctx->server_addr.sin_port)); // Converts the port from network byte order to host byte order
}

void create_socket(server_context_t *server_ctx) {
    server_ctx->server_fd = socket(IPV4_AF, PROTOCOL_TCP, TCP_PROTOCOL_NUMBER);
    if (server_ctx->server_fd == -1) {
        perror("create_socket");
        exit(EXIT_FAILURE);
    }
}

void bind_socket(server_context_t *server_ctx) {
    int socket_option = 1;
    setsockopt(server_ctx->server_fd, SOL_SOCKET, SO_REUSEADDR, &socket_option, sizeof(socket_option));

    if (bind(server_ctx->server_fd, (struct sockaddr *)&server_ctx->server_addr, server_ctx->server_addr_len) == -1) {
        if (server_ctx->server_fd >= 0) {
            close(server_ctx->server_fd);
        }
        perror("bind_socket");
        exit(EXIT_FAILURE);
    }
}

void listen_socket(server_context_t *server_ctx) {
    if (listen(server_ctx->server_fd, MAX_CONNECTION_BACKLOG) == -1) {
        if (server_ctx->server_fd >= 0) {
            close(server_ctx->server_fd);
        }
        perror("listen_socket");
        exit(EXIT_FAILURE);
    }
}