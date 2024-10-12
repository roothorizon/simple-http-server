#include "main.h"

int initialize_server(server_context_t *server_ctx, int port) {
    printf("INIT SERVER CALLED\n");

    // Initialize the server_addr struct
    server_ctx->server_addr.sin_family = DOMAIN_IPV4;
    server_ctx->server_addr.sin_addr.s_addr = LOOPBACK_ADDRESS;
    server_ctx->server_addr.sin_port = htons(port);

    server_ctx->server_addr_len = sizeof(server_ctx->server_addr);

    // Create the socket
    server_ctx->server_fd = socket(DOMAIN_IPV4, PROTOCOL_TCP, PROTOCOL_NUMBER);
    if (server_ctx->server_fd < 0) {
        printf("ERR_SOCKET\n");
        return ERR_SOCKET;
    }

    // Bind IP address and port to socket
    if (bind(server_ctx->server_fd, (struct sockaddr *)&server_ctx->server_addr, server_ctx->server_addr_len) < 0) {
        close(server_ctx->server_fd);
        printf("ERR_BIND\n");
        return ERR_BIND;
    }

    // Start listening for incoming connections
    if (listen(server_ctx->server_fd, MAX_CONNECTION_BACKLOG) < 0) {
        close(server_ctx->server_fd);
        printf("ERR_LISTEN\n");
        return ERR_LISTEN;
    }

    printf("Listening for incoming connections on: %s, port: %d\n",
            inet_ntoa(server_ctx->server_addr.sin_addr), // Converts the ip address from network byte order to host byte order (not thread-safe)
            ntohs(server_ctx->server_addr.sin_port)); // Converts the port from network byte order to host byte order
    printf("INIT SERVER SUCCEEDED\n");
    return SUCCESS;
}

int handle_peer_connections(server_context_t *server_ctx, peer_context_t *peer_ctx) {
    printf("PEER CONNECTION CALLED\n");
    // Accept incoming connection
    if ((peer_ctx->peer_fd = accept(server_ctx->server_fd,
    (struct sockaddr *)&server_ctx->server_addr,
    &server_ctx->server_addr_len)) < 0) {
        close(peer_ctx->peer_fd);
        printf("ERR_ACCEPT\n");
        return ERR_ACCEPT;
    }
    printf("PEER CONNECTION SUCCEEDED\n");
    return SUCCESS;
}

int handle_request(peer_context_t *peer_ctx) {
    printf("HANDLE REQUEST CALLED\n");
    ssize_t bytes_read;
    size_t total_bytes_read = 0;

    peer_ctx->request_buffer = (char *)malloc(INITIAL_BUFFER_SIZE);
    if (peer_ctx->request_buffer == NULL) {
        close(peer_ctx->peer_fd);
        printf("ERR_MALLOC\n");
        return ERR_MALLOC;
    }
    peer_ctx->request_size = INITIAL_BUFFER_SIZE;
    printf("MALLOC SUCCEEDED\n");
    printf("Request buffer allocated with size: %zu\n", peer_ctx->request_size);
    printf("Request buffer pointer: %p\n", (void *)peer_ctx->request_buffer);
    printf("Contents of request buffer: \n%.*s\n", (int)strlen(peer_ctx->request_buffer),peer_ctx->request_buffer);

    while ((bytes_read = read(peer_ctx->peer_fd, peer_ctx->request_buffer + total_bytes_read,
    peer_ctx->request_size - total_bytes_read)) > 0) {
        total_bytes_read += bytes_read;
        printf("READ SUCCEEDED\n");

        if (total_bytes_read == peer_ctx->request_size) {
            size_t new_size = peer_ctx->request_size * 2;
            char *new_buffer = (char *)realloc(peer_ctx->request_buffer, new_size);
            if (new_buffer == NULL) {
                free(peer_ctx->request_buffer);
                peer_ctx->request_buffer = NULL;
                close(peer_ctx->peer_fd);
                printf("ERR_REALLOC\n");
                return ERR_REALLOC;
            }
            peer_ctx->request_buffer = new_buffer;
            peer_ctx->request_size = new_size;
            printf("REALLOC SUCCEEDED\n");
        }
        break;
    }
    printf("WHILE LOOP SUCCEEDED\n");

    if (bytes_read < 0) {
        close(peer_ctx->peer_fd);
        printf("ERR_READ\n");
        return ERR_READ;
    }

    printf("Received request:\n%s\n", peer_ctx->request_buffer);

    close(peer_ctx->peer_fd);
    return SUCCESS;
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
        }*/

    //close(peer_ctx->peer_fd);
    //return SUCCESS;
}

int handle_response(peer_context_t *peer_ctx) {
    return SUCCESS;

}

void cleanup_server(server_context_t *server_ctx, peer_context_t *peer_ctx) {
    printf("CLEANUP SERVER CALLED\n");
    if (server_ctx->server_fd > 0) {
        close(server_ctx->server_fd);
    }
    if (peer_ctx->request_buffer) {
        free(peer_ctx->request_buffer);
        peer_ctx->request_buffer = NULL;
    }
    if (peer_ctx->response_buffer) {
        free(peer_ctx->response_buffer);
        peer_ctx->request_buffer = NULL;
    }
    if (peer_ctx->peer_fd > 0) {
        close(peer_ctx->peer_fd);
    }
    printf("SERVER CLEANED UP\n");
}

int main() {
    server_context_t server_ctx;
    peer_context_t peer_ctx;
    unsigned char running = 1;

    int server_status = initialize_server(&server_ctx, PORT);
    printf("Server Status: %d\n", server_status);
    printf("Server FD: %d\n", server_ctx.server_fd);

    if (server_status != SUCCESS) {
        handle_error(server_status);
        cleanup_server(&server_ctx, &peer_ctx);
        return server_status;
    }

    while (running) {
        int peer_conn_status = handle_peer_connections(&server_ctx, &peer_ctx);
        printf("Peer Status: %d\n", peer_conn_status);
        printf("Peer FD: %d\n", peer_ctx.peer_fd);
        if (peer_conn_status != SUCCESS) {
            handle_error(peer_conn_status);
            continue;
        }
        handle_request(&peer_ctx);
        printf("Request Buffer Size: %zu\n", peer_ctx.request_size);
        if (peer_ctx.request_buffer != NULL) {
            printf("Request Buffer: %s\n", peer_ctx.request_buffer);
        } else {
            printf("Request Buffer: NULL\n");
        }
    }

    cleanup_server(&server_ctx, &peer_ctx);
    return SUCCESS;
}