#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>

#define AF_IPV4 AF_INET
#define PROTOCOL_TCP SOCK_STREAM
#define PROTOCOL_NUMBER_TCP 6
#define DEFAULT_IP_ADDR "127.0.0.1"
#define MAX_CONNECTION_BACKLOG 50

struct server_context_s {
    int server_fd;
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;
};

struct http_server_s {
    struct server_context_s server_ctx;
};

struct http_server_s* http_server_init(const int port);
int http_server_listen(struct http_server_s *p_http_server);
int create_socket(struct server_context_s *p_server_ctx);
int bind_socket(struct server_context_s *p_server_ctx);
int listen_socket(struct server_context_s *p_server_ctx);
void cleanup_server(struct http_server_s *p_server_ctx);

#endif