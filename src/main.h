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
#define PORT htons(8080);
#define BUFFER_SIZE 1024

#define handle_error(message) do { perror(message); exit(EXIT_FAILURE); } while(0)

void initialize_server();
void handle_peer_connections();

#endif