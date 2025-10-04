#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "server.h"

struct http_server_s* http_server_init(const int port) {

    struct http_server_s* http_server = malloc(sizeof(struct http_server_s));

    if (http_server == NULL) {
        perror("Error malloc");
        return NULL;
    }

    struct server_context_s *p_server_ctx = &http_server->server_ctx;
    struct sockaddr_in *p_server_addr = &p_server_ctx->server_addr;

    p_server_addr->sin_family = AF_IPV4;
    p_server_addr->sin_addr.s_addr = inet_addr(DEFAULT_IP_ADDR);
    p_server_addr->sin_port = htons(port);
    p_server_ctx->server_addr_len = sizeof(p_server_ctx->server_addr);

    return http_server;
}

int http_server_listen(struct http_server_s *p_http_server) {

    struct server_context_s *p_server_ctx = &p_http_server->server_ctx;

    int create_socket_status = create_socket(p_server_ctx);
    int bind_socket_status = bind_socket(p_server_ctx);
    int listen_socket_status = listen_socket(p_server_ctx);

    if (create_socket_status != 0 ||
        bind_socket_status != 0 ||
        listen_socket_status != 0) {
            cleanup_server(p_http_server);
            return 1;
        }

    printf("Listening for incoming connections on: %s, port: %d\n",
            inet_ntoa(p_server_ctx->server_addr.sin_addr),
            ntohs(p_server_ctx->server_addr.sin_port));

    return 0;
}

int create_socket(struct server_context_s *p_server_ctx) {

    p_server_ctx->server_fd = socket(AF_IPV4, PROTOCOL_TCP, PROTOCOL_NUMBER_TCP);
    if (p_server_ctx->server_fd == -1) {
        perror("Error creating socket");
        return 1;
    }

    return 0;
}

int bind_socket(struct server_context_s *p_server_ctx) {

    int bind_status = bind(p_server_ctx->server_fd, (struct sockaddr *)&p_server_ctx->server_addr, p_server_ctx->server_addr_len);

    if (bind_status == -1) {
        perror("Error binding socket");
        return 1;
    }

    return 0;
}

int listen_socket(struct server_context_s *p_server_ctx) {

    int listen_status = listen(p_server_ctx->server_fd, MAX_CONNECTION_BACKLOG);

    if (listen_status == -1) {
        perror("Error listening on socket");
        return 1;
    }

    return 0;
}

void cleanup_server(struct http_server_s *p_http_server) {

    const struct server_context_s *p_server_ctx = &p_http_server->server_ctx;

    if (p_server_ctx->server_fd >= 0) {
        close(p_server_ctx->server_fd);
    }

    free(p_http_server);
    p_http_server = NULL;

}