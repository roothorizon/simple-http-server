#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "main.h"

#define MEMORY_ARENA_SIZE 4096
#define MAX_MEMORE_ARENA_SIZE MEMORY_ARENA_SIZE * 1024

void handle_client_connections(server_context_t *server_ctx, client_context_t *client_ctx);
int handle_request(client_context_t *client_ctx);
int handle_response(client_context_t *client_ctx);

#endif