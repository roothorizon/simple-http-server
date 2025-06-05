#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LOOPBACK_ADDRESS inet_addr("127.0.0.1");
#define PORT 8080
#define MAX_CONNECTION_BACKLOG 50

// ** Server struct
typedef struct {
    int32_t server_fd; // ** Server File Descriptor
    struct sockaddr_in server_addr; // ** Server Socket Address
    socklen_t server_addr_len; // ** Server Socket Address Length
} server_context_t;

void cleanup_server(server_context_t *p_server_ctx);

#endif