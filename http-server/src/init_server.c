#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>

#include "utils.h"
#include "init_server.h"

int init_server(server_context_t *server_ctx, int port) {
    printf("INIT SERVER CALLED\n");

    // Initialize the server_addr struct
    server_ctx->server_addr.sin_family = DOMAIN_IPV4;
    server_ctx->server_addr.sin_addr.s_addr = LOOPBACK_ADDRESS;
    server_ctx->server_addr.sin_port = htons(port);

    server_ctx->server_addr_len = sizeof(server_ctx->server_addr);

    // Create the socket
    server_ctx->server_fd = socket(DOMAIN_IPV4, PROTOCOL_TCP, PROTOCOL_NUMBER);
    if (server_ctx->server_fd < 0) {
        printf("ERR_SOCKET\n");
        return ERR_SOCKET;
    }

    // Bind IP address and port to socket
    if (bind(server_ctx->server_fd, (struct sockaddr *)&server_ctx->server_addr, server_ctx->server_addr_len) < 0) {
        close(server_ctx->server_fd);
        printf("ERR_BIND\n");
        return ERR_BIND;
    }

    // Start listening for incoming connections
    if (listen(server_ctx->server_fd, MAX_CONNECTION_BACKLOG) < 0) {
        close(server_ctx->server_fd);
        printf("ERR_LISTEN\n");
        return ERR_LISTEN;
    }

    printf("Listening for incoming connections on: %s, port: %d\n",
            inet_ntoa(server_ctx->server_addr.sin_addr), // Converts the ip address from network byte order to host byte order (not thread-safe)
            ntohs(server_ctx->server_addr.sin_port)); // Converts the port from network byte order to host byte order
    printf("INIT SERVER SUCCEEDED\n");
    return SUCCESS;
}

void cleanup_server(server_context_t *server_ctx, client_context_t *client_ctx) {
    printf("CLEANUP SERVER CALLED\n");
    if (server_ctx->server_fd > 0) {
        close(server_ctx->server_fd);
    }
    if (client_ctx->request_buffer) {
        free(client_ctx->request_buffer);
        client_ctx->request_buffer = NULL;
    }
    if (client_ctx->response_buffer) {
        free(client_ctx->response_buffer);
        client_ctx->request_buffer = NULL;
    }
    if (client_ctx->client_fd > 0) {
        close(client_ctx->client_fd);
    }
    printf("SERVER CLEANED UP\n");
}