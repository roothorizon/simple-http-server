#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "main.h"
#include "utils.h"

#define MEMORY_ARENA_SIZE 4096
#define MAX_MEMORY_ARENA_SIZE MEMORY_ARENA_SIZE * 1024

int accept_connections(http_server_t *p_http_server);
int handle_request(http_request_t *p_http_request);
int handle_response(http_request_t *p_http_request);
int http_request_init(memory_arena_t *p_arena, http_request_t **pp_request);
void cleanup_connection(http_request_t *p_http_request);

#endif