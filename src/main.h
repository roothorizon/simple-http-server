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
#define PORT 8080;
#define BUFFER_SIZE 1024
#define INITIAL_BUFFER_SIZE 1024
#define MAX_BUFFER_SIZE 1024 * 1024

#define handle_error(message) do { perror(message); exit(EXIT_FAILURE); } while(0)

enum {
    SUCCESS = 0,
    ERR_SOCKET = 1,
    ERR_BIND = 2,
    ERR_LISTEN = 3,
    ERR_ACCEPT = 4,
    ERR_MALLOC = 5,
    ERR_REALLOC = 6,
    ERR_READ = 7,
};

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

int initialize_server();
int handle_peer_connections();
int handle_request();
int handle_response();

#endif