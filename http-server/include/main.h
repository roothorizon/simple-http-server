#ifndef MAIN_H
#define MAIN_H

#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

// ** Server struct
typedef struct {
    int server_fd; // Server File Descriptor
    struct sockaddr_in server_addr; // Server Socket Address
    socklen_t server_addr_len; // Server Socket Address Length
} server_context_t;

void cleanup_server(server_context_t *p_server_ctx);

#endif