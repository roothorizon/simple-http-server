#include "main.h"
#include "init_server.h"
#include "connection_handler.h"
#include "utils.h"

int main() {
    server_context_t server_ctx;
    client_context_t client_ctx;
    unsigned char running = 1;

    int server_status = init_server(&server_ctx, PORT);
    printf("Server Status: %d\n", server_status);
    printf("Server FD: %d\n", server_ctx.server_fd);

    if (server_status != SUCCESS) {
        handle_error(server_status);
        cleanup_server(&server_ctx, &client_ctx);
        return server_status;
    }

    while (running) {
        int client_conn_status = handle_client_connections(&server_ctx, &client_ctx);
        printf("Peer Status: %d\n", client_conn_status);
        printf("Peer FD: %d\n", client_ctx.client_fd);
        if (client_conn_status != SUCCESS) {
            handle_error(client_conn_status);
            continue;
        }
        handle_request(&client_ctx);
        printf("Request Buffer Size: %zu\n", client_ctx.request_size);
        if (client_ctx.request_buffer != NULL) {
            printf("Request Buffer: %s\n", client_ctx.request_buffer);
        } else {
            printf("Request Buffer: NULL\n");
        }
    }

    cleanup_server(&server_ctx, &client_ctx);
    return SUCCESS;
}