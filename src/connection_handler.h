#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "main.h"
#include "utils.h"
#include "http_parser.h"

#define MEMORY_ARENA_SIZE 4096
#define MAX_MEMORY_ARENA_SIZE MEMORY_ARENA_SIZE * 1024

// ** Client struct
typedef struct {
    int32_t client_fd;
    struct sockaddr_in client_addr;
    uint32_t client_addr_len;
    memory_arena_t arena;
    http_req_t *p_request;
    http_res_t *p_response;
} client_context_t;

void handle_client_connections(server_context_t *p_server_ctx);
int handle_request(client_context_t *p_client_ctx);
int handle_response(client_context_t *p_client_ctx);
int http_request_init(memory_arena_t *p_arena, http_req_t **pp_request, http_res_t **pp_response);
void cleanup_connection(client_context_t *p_client_ctx);

#endif