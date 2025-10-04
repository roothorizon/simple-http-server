#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"
#include "connection_handler.h"
#include "main.h"
#include "utils.h"

void handle_client_connections(server_context_t *p_server_ctx) {
    client_context_t client_ctx;
    client_ctx.client_addr_len = sizeof(client_ctx.client_addr);

    // ** Accept incoming connection
    client_ctx.client_fd = accept(p_server_ctx->server_fd, (struct sockaddr *)&client_ctx.client_addr, &client_ctx.client_addr_len);
    if (client_ctx.client_fd == -1) {
        cleanup_server(p_server_ctx);
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Received connection from: %s, port: %d\n",
            inet_ntoa(client_ctx.client_addr.sin_addr), // ** Converts the ip address from network byte order to host byte order (not thread-safe)
            ntohs(client_ctx.client_addr.sin_port)); // ** Converts the port number from network byte order to host byte order

    arena_init(&client_ctx.arena, MEMORY_ARENA_SIZE);
    if (client_ctx.arena.p_base == NULL) {
        close(client_ctx.client_fd);
        perror("arena_init");
        exit(-1);
    }
    memset(client_ctx.arena.p_base, 0, client_ctx.arena.size);

    handle_request(&client_ctx);
}

int handle_request(client_context_t *client_ctx) {
    ssize_t bytes_read;
    size_t total_bytes_read = 0;

    if (http_request_init(&client_ctx->arena, &client_ctx->p_request, &client_ctx->p_response) == -1) {
        printf("This is the problem\n");
        close(client_ctx->client_fd);
        return -1;
    }

    char *pp_request_buffer = client_ctx->p_request->p_request_buffer;
    size_t request_size = client_ctx->p_request->request_size;

    while (1) {
        /*
         *  Reads the data into the request buffer, saves the amount of bytes read in bytes_read.
         */
        bytes_read = read(client_ctx->client_fd, pp_request_buffer + total_bytes_read, request_size - total_bytes_read);

        if (bytes_read <= 0) {
            arena_free(&client_ctx->arena);
            close(client_ctx->client_fd);
            printf("Error: bytes_read <= 0 or connection closed\n");
            break;
        }

        total_bytes_read += bytes_read;
        pp_request_buffer[request_size] = '\0';

        char *headers_end = strstr(client_ctx->p_request->p_request_buffer, "\r\n\r\n");
        if (headers_end) {
            break;
        }

        if (total_bytes_read == client_ctx->p_request->request_size) {
            printf("Request too large\n");
            arena_free(&client_ctx->arena);
            close(client_ctx->client_fd);
            break;
        }
    }

    printf("Received request:\n%.*s\n", (uint32_t)total_bytes_read, client_ctx->p_request->p_request_buffer);

    uint16_t request_status = parse_http_request(client_ctx->p_request);

    if (request_status == 200) {
        const char *status_line = "HTTP/1.1 200 OK\r\n";
        const char *content_type = "Content-Type: text/html\r\n";
        const char *content_length = "Content-Length: 92\r\n";
        const char *empty_line = "\r\n";
        const char *body = "<html><body><h1>Hello, World!</h1><br><p>Received a successful GET request</p></body></html>";

        send(client_ctx->client_fd, status_line, strlen(status_line), 0);
        send(client_ctx->client_fd, content_type, strlen(content_type), 0);
        send(client_ctx->client_fd, content_length, strlen(content_length), 0);
        send(client_ctx->client_fd, empty_line, strlen(empty_line), 0);
        send(client_ctx->client_fd, body, strlen(body), 0);
    } else {
        const char *status_line = "HTTP/1.1 404 Not Found\r\n";
        const char *content_type = "Content-Type: text/html\r\n";
        const char *content_length = "Content-Length: 53\r\n";
        const char *empty_line = "\r\n";
        const char *body = "<html><body><h1>Not a GET request!</h1></body></html>";

        send(client_ctx->client_fd, status_line, strlen(status_line), 0);
        send(client_ctx->client_fd, content_type, strlen(content_type), 0);
        send(client_ctx->client_fd, content_length, strlen(content_length), 0);
        send(client_ctx->client_fd, empty_line, strlen(empty_line), 0);
        send(client_ctx->client_fd, body, strlen(body), 0);
    }

    arena_free(&client_ctx->arena);
    close(client_ctx->client_fd);
    return 0;
}

int handle_response(client_context_t *client_ctx) {
    return SUCCESS;

}

int http_request_init(memory_arena_t *p_arena, http_req_t **pp_request, http_res_t **pp_response) {
    http_req_t *p_request = (http_req_t *) arena_alloc(p_arena, sizeof(http_req_t));
    http_res_t *p_response = (http_res_t *) arena_alloc(p_arena, sizeof(http_res_t));

    if (p_request == NULL || p_response == NULL) {
        arena_free(p_arena);
        printf("arena_alloc for http request and response structs failed\n");
        return -1;
    }

    *pp_request = p_request;
    *pp_response = p_response;

    p_request->request_size = p_arena->size - (sizeof(http_req_t) + sizeof(http_res_t));
    p_request->p_request_buffer = (char *) arena_alloc(p_arena, p_request->request_size);

    if (p_request->p_request_buffer == NULL) {
        arena_free(p_arena);
        printf("arena_alloc for the http request buffer failed\n");
        return -1;
    }

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