#include "utils.h"
#include "connection_handler.h"
#include "main.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void handle_client_connections(server_context_t *server_ctx) {
    client_context_t client_ctx;

    struct sockaddr_storage client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Accept incoming connection
    client_ctx.client_fd = accept(server_ctx->server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_ctx.client_fd == -1) {
        cleanup_server(server_ctx);
        perror("accept");
        exit(EXIT_FAILURE);
    }

    arena_init(&client_ctx.arena, MEMORY_ARENA_SIZE);
    if (client_ctx.arena.base == NULL) {
        close(client_ctx.client_fd);
        perror("arena_init");
        exit(-1);
    }

    handle_request(&client_ctx);
}

int handle_request(client_context_t *client_ctx) {
    ssize_t bytes_read;
    size_t total_bytes_read = 0;

    arena_init(&client_ctx->arena, MEMORY_ARENA_SIZE);
    if (client_ctx->arena.base == NULL) {
        close(client_ctx->client_fd);
        perror("arena_init");
        return -1;
    }

    client_ctx->p_request->request_size = MEMORY_ARENA_SIZE;
    client_ctx->p_request->p_request_buffer = (char *)arena_alloc(&client_ctx->arena, MEMORY_ARENA_SIZE);
    if (client_ctx->p_request->p_request_buffer == NULL) {
        arena_free(&client_ctx->arena);
        close(client_ctx->client_fd);
        perror("arena_alloc");
        return -1;
    }

    while ((bytes_read = read(client_ctx->client_fd, client_ctx->p_request->p_request_buffer + total_bytes_read,
    client_ctx->p_request->request_size - total_bytes_read)) > 0) {
        total_bytes_read += bytes_read;

        if (total_bytes_read == client_ctx->p_request->request_size) {
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

    if (total_bytes_read < client_ctx->p_request->request_size) {
        client_ctx->p_request->p_request_buffer[total_bytes_read] = '\0';
    } else {
        printf("Received request (not null-terminated):\n");
        fwrite(client_ctx->p_request->p_request_buffer, 1, total_bytes_read, stdout);
        printf("\n");
    }

    printf("Received request:\n%.*s\n", (int)total_bytes_read, client_ctx->p_request->p_request_buffer);

    char *p_method_end_space = strchr(client_ctx->p_request->p_request_buffer, ' ');
    if (p_method_end_space == NULL) {
        arena_free(&client_ctx->arena);
        close(client_ctx->client_fd);
        perror("Request parsing error");
        return -1;
    }
    client_ctx->p_request->method_len = p_method_end_space - client_ctx->p_request->p_request_buffer;
    client_ctx->p_request->p_method = client_ctx->p_request->p_request_buffer;

    printf("Received request line method: %.*s\n", (int)client_ctx->p_request->method_len, client_ctx->p_request->p_method);

    arena_free(&client_ctx->arena);
    close(client_ctx->client_fd);
    return 0;
}

int handle_response(client_context_t *client_ctx) {
    return SUCCESS;

}

int http_request_init(memory_arena_t *p_arena, http_req_t **pp_request) {
    http_req_t *p_request = (http_req_t *) arena_alloc(p_arena, MEMORY_ARENA_SIZE);

    if (p_request == NULL) {
        arena_free(p_arena);
        perror("arena_alloc");
        return -1;
    }

    memset(p_request, 0, MEMORY_ARENA_SIZE);
    *pp_request = p_request;

    p_request->p_request_buffer = (char *) p_arena->base + sizeof(http_req_t);
    p_request->request_size = p_arena->size - sizeof(http_req_t);

    return 0;
}

void cleanup_connection(client_context_t *p_client_ctx) {
    if (p_client_ctx->p_request->p_request_buffer) {
        free(p_client_ctx->p_request->p_request_buffer);
        p_client_ctx->p_request->p_request_buffer = NULL;
    }
    if (p_client_ctx->p_response->p_response_buffer) {
        free(p_client_ctx->p_response->p_response_buffer);
        p_client_ctx->p_request->p_request_buffer = NULL;
    }
    if (p_client_ctx->client_fd >= 0) {
        close(p_client_ctx->client_fd);
    }
}