#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server.h"

http_server_t* http_server_init(const int port) {

    http_server_t* http_server = malloc(sizeof(http_server_t));

    if (http_server == NULL) {
        perror("Error malloc");
        return NULL;
    }

    struct sockaddr_in *p_server_addr = &http_server->server_addr;

    memset(p_server_addr, 0, sizeof(*p_server_addr));
    p_server_addr->sin_family = AF_IPV4;
    p_server_addr->sin_addr.s_addr = inet_addr(DEFAULT_IP_ADDR);
    p_server_addr->sin_port = htons(port);
    http_server->server_addr_len = sizeof(*p_server_addr);

    return http_server;
}

int create_socket(http_server_t *p_http_server) {

    p_http_server->server_fd = socket(AF_IPV4, PROTOCOL_TCP, PROTOCOL_NUMBER_TCP);
    if (p_http_server->server_fd == -1) {
        perror("Error creating socket");
        return 1;
    }

    return 0;
}

int bind_socket(http_server_t *p_http_server) {

    int bind_status = bind(p_http_server->server_fd, (struct sockaddr *)&p_http_server->server_addr, p_http_server->server_addr_len);

    if (bind_status == -1) {
        perror("Error binding socket");
        return 1;
    }

    return 0;
}

int listen_socket(http_server_t *p_http_server) {

    int listen_status = listen(p_http_server->server_fd, MAX_CONNECTION_BACKLOG);

    if (listen_status == -1) {
        perror("Error listening on socket");
        return 1;
    }

    return 0;
}

int http_server_listen(http_server_t *p_http_server) {

    int create_socket_status = create_socket(p_http_server);
    int bind_socket_status = bind_socket(p_http_server);
    int listen_socket_status = listen_socket(p_http_server);

    if (create_socket_status != 0 ||
        bind_socket_status != 0 ||
        listen_socket_status != 0) {
            cleanup_server(p_http_server);
            return 1;
        }

    printf("Listening for incoming connections on: %s, port: %d\n",
            inet_ntoa(p_http_server->server_addr.sin_addr),
            ntohs(p_http_server->server_addr.sin_port));

    return 0;
}

void cleanup_server(http_server_t *p_http_server) {

    if (p_http_server->server_fd >= 0) {
        close(p_http_server->server_fd);
    }

    free(p_http_server);
    p_http_server = NULL;

}