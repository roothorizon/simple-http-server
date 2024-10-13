#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "main.h"

#define INITIAL_BUFFER_SIZE 1024
#define MAX_BUFFER_SIZE 1024 * 1024

int handle_client_connections(server_context_t *server_ctx, client_context_t *client_ctx);
int handle_request(client_context_t *client_ctx);
int handle_response(client_context_t *client_ctx);

#endif