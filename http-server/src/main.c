#include "main.h"
#include "connection_handler.h"
#include "init_server.h"

#include <stdio.h>
#include <unistd.h>

int main() {
    server_context_t server_ctx;
    unsigned char running = 1;

    init_server(&server_ctx, PORT);
    printf("Server File Descriptor: %d\n", server_ctx.server_fd);

    while (running) {
        handle_client_connections(&server_ctx);
    }

    cleanup_server(&server_ctx);
    return 0;
}

void cleanup_server(server_context_t *server_ctx) {
    if (server_ctx->server_fd >= 0) {
        close(server_ctx->server_fd);
    }
    /*if (client_ctx->p_request->p_request_buffer) {
        free(client_ctx->p_request->p_request_buffer);
        client_ctx->p_request->p_request_buffer = NULL;
    }
    if (client_ctx->p_response->p_response_buffer) {
        free(client_ctx->p_response->p_response_buffer);
        client_ctx->p_request->p_request_buffer = NULL;
    }
    if (client_ctx->client_fd >= 0) {
        close(client_ctx->client_fd);
    }*/
    printf("Server cleanup done\n");
}