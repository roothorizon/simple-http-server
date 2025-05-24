#ifndef MAIN_H
#define MAIN_H

#include "utils.h"

#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

typedef struct {
    int server_fd; // Server File Descriptor
    struct sockaddr_in server_addr; // Server Socket Address
    socklen_t server_addr_len; // Server Socket Address Length
} server_context_t;

typedef struct {
    int client_fd;
    struct sockaddr_in client_addr;
    int client_addr_len;
    memory_arena_t arena;
    char *request_buffer;
    size_t request_size;
    char * response_buffer;
    size_t response_size;
} client_context_t;

void cleanup_server(server_context_t *server_ctx, client_context_t *client_ctx);

#endif