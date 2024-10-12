#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DOMAIN_IPV4 AF_INET
#define PROTOCOL_TCP SOCK_STREAM
#define PROTOCOL_NUMBER 0
#define MAX_CONNECTION_BACKLOG 50
#define LOOPBACK_ADDRESS inet_addr("127.0.0.1");
#define PORT 8080
#define BUFFER_SIZE 1024
#define INITIAL_BUFFER_SIZE 1024
#define MAX_BUFFER_SIZE 1024 * 1024

typedef enum {
    ERR_UNKNOWN = -1,
    SUCCESS = 0,
    ERR_SOCKET = 1,
    ERR_BIND = 2,
    ERR_LISTEN = 3,
    ERR_ACCEPT = 4,
    ERR_MALLOC = 5,
    ERR_REALLOC = 6,
    ERR_READ = 7,
} ErrorCode;

typedef struct {
    int server_fd;
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;
} server_context_t;

typedef struct {
    int peer_fd;
    struct sockaddr_in peer_addr;
    int peer_addr_len;
    char *request_buffer;
    size_t request_size;
    char * response_buffer;
    size_t response_size;
} peer_context_t;

int initialize_server(server_context_t *server_ctx, int port);
int handle_peer_connections(server_context_t *server_ctx, peer_context_t *peer_ctx);
int handle_request(peer_context_t *peer_ctx);
int handle_response(peer_context_t *peer_ctx);
void cleanup_server(server_context_t *server_ctx, peer_context_t *peer_ctx);

static void handle_error(ErrorCode error_code) {
    switch (error_code) {
        case ERR_SOCKET:
            printf("Error: Unable to create socket.\n");
            break;
        
        case ERR_BIND:
            printf("Error: Unable to bind socket.\n");
            break;
        
        case ERR_LISTEN:
            printf("Error: Unable to start listening for incoming connections.\n");
            break;

        case ERR_ACCEPT:
            printf("Error: Unable to accept incoming connections.\n");
            break;

        case ERR_MALLOC:
            printf("Error: Malloc was unable to allocate memory.\n");
            break;

        case ERR_REALLOC:
            printf("Error: Realloc was unable to reallocate memory.\n");
            break;

        case ERR_READ:
            printf("Error: Unable to read request buffer.\n");
            break;

        default:
            printf("Error: An unknown error occurred.\n");
            break;
    }
}

#endif