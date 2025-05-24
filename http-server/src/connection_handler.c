#include "utils.h"
#include "connection_handler.h"
#include "main.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_client_connections(server_context_t *server_ctx, client_context_t *client_ctx) {
    struct sockaddr_storage client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Accept incoming connection
    client_ctx->client_fd = accept(server_ctx->server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_ctx->client_fd == -1) {
        cleanup_server(server_ctx, client_ctx);
        perror("accept");
        exit(EXIT_FAILURE);
    }
}

int handle_request(client_context_t *client_ctx) {
    ssize_t bytes_read;
    size_t total_bytes_read = 0;

    arena_init(&client_ctx->arena, MEMORY_ARENA_SIZE);
    if (!client_ctx->arena.base) {
        close(client_ctx->client_fd);
        perror("arena_init");
        return -1;
    }

    client_ctx->request_buffer = (char *)arena_alloc(&client_ctx->arena, MEMORY_ARENA_SIZE);
    if (!client_ctx->request_buffer) {
        arena_free(&client_ctx->arena);
        close(client_ctx->client_fd);
        perror("arena_alloc");
        return -1;
    }

    client_ctx->request_size = MEMORY_ARENA_SIZE;

    while ((bytes_read = read(client_ctx->client_fd, client_ctx->request_buffer + total_bytes_read,
    client_ctx->request_size - total_bytes_read)) > 0) {
        total_bytes_read += bytes_read;

        if (total_bytes_read == client_ctx->request_size) {
            printf("Request too large\n");
            arena_free(&client_ctx->arena);
            close(client_ctx->client_fd);
            return -1;
        }
    }

    if (bytes_read < 0) {
        arena_free(&client_ctx->arena);
        close(client_ctx->client_fd);
        perror("Empty request");
        return -1;
    }

    if (total_bytes_read < client_ctx->request_size) {
        client_ctx->request_buffer[total_bytes_read] = '\0';
    } else {
        printf("Received request (not null-terminated):\n");
        fwrite(client_ctx->request_buffer, 1, total_bytes_read, stdout);
        printf("\n");
    }

    printf("Received request:\n%.*s\n", (int)total_bytes_read, client_ctx->request_buffer);

    arena_free(&client_ctx->arena);
    close(client_ctx->client_fd);
    return 0;
    //Call handle response
    /*
    read(pper_ctx->peer_fd, &buffer, BUFFER_SIZE);
        printf("%.*s", BUFFER_SIZE, buffer);
        char method[10], path[100];
        sscanf(buffer, "%s %s", method, path);

        if (strcmp(method, "GET") == 0) {
            if (strcmp(path, "/") == 0) {
                strcpy(path, "/index.html");
            }
            char full_path[200] = "../www";
            strcat(full_path, path);

            FILE *file = fopen(full_path, "r");
            if (file != NULL) {
                char response[BUFFER_SIZE] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
                send(peer_ctx->peer_fd, response, strlen(response), 0);
                printf("%.*s", BUFFER_SIZE, response);

                char file_buffer[BUFFER_SIZE];
                while (fgets(file_buffer, BUFFER_SIZE, file)) {
                    send(pper_ctx->peer_fd, file_buffer, strlen(file_buffer), 0);
                }
                fclose(file);
            } else {
                char *not_found = "HTTP/1.1 404 Not Found\r\n\r\n";
                send(peer_ctx->peer_fd, not_found, strlen(not_found), 0);
                printf("%.*s", strlen(not_found), not_found);
            }
        }

    close(peer_ctx->peer_fd);
    return SUCCESS;*/
}

int handle_response(client_context_t *client_ctx) {
    return SUCCESS;

}