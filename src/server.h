#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>

#define AF_IPV4 AF_INET
#define PROTOCOL_TCP SOCK_STREAM
#define PROTOCOL_NUMBER_TCP 6
#define DEFAULT_IP_ADDR "127.0.0.1"
#define MAX_CONNECTION_BACKLOG 50

typedef struct {
    int server_fd;
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;
} http_server_t;

http_server_t* http_server_init(const int port);
int create_socket(http_server_t *p_http_server);
int bind_socket(http_server_t *p_http_server);
int listen_socket(http_server_t *p_http_server);
int http_server_listen(http_server_t *p_http_server);
void cleanup_server(http_server_t *p_http_server);

#endif