#ifndef MAIN_H
#define MAIN_H

#include <netinet/in.h>

#define PORT 8080

typedef struct {
    int server_fd;
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;
} server_context_t;

typedef struct {
    int client_fd;
    struct sockaddr_in client_addr;
    int client_addr_len;
    char *request_buffer;
    size_t request_size;
    char * response_buffer;
    size_t response_size;
} client_context_t;

#endif