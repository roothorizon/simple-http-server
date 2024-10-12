#include "connection_handler.h"

int handle_client_connections(server_context_t *server_ctx, client_context_t *client_ctx) {
    printf("PEER CONNECTION CALLED\n");
    // Accept incoming connection
    if ((client_ctx->client_fd = accept(server_ctx->server_fd,
    (struct sockaddr *)&server_ctx->server_addr,
    &server_ctx->server_addr_len)) < 0) {
        close(client_ctx->client_fd);
        printf("ERR_ACCEPT\n");
        return ERR_ACCEPT;
    }
    printf("PEER CONNECTION SUCCEEDED\n");
    return SUCCESS;
}

int handle_request(client_context_t *client_ctx) {
    printf("HANDLE REQUEST CALLED\n");
    ssize_t bytes_read;
    size_t total_bytes_read = 0;

    client_ctx->request_buffer = (char *)malloc(INITIAL_BUFFER_SIZE);
    if (client_ctx->request_buffer == NULL) {
        close(client_ctx->client_fd);
        printf("ERR_MALLOC\n");
        return ERR_MALLOC;
    }
    client_ctx->request_size = INITIAL_BUFFER_SIZE;
    printf("MALLOC SUCCEEDED\n");
    printf("Request buffer allocated with size: %zu\n", client_ctx->request_size);
    printf("Request buffer pointer: %p\n", (void *)client_ctx->request_buffer);
    printf("Contents of request buffer: \n%.*s\n", (int)strlen(client_ctx->request_buffer),client_ctx->request_buffer);

    while ((bytes_read = read(client_ctx->client_fd, client_ctx->request_buffer + total_bytes_read,
    client_ctx->request_size - total_bytes_read)) > 0) {
        total_bytes_read += bytes_read;
        printf("READ SUCCEEDED\n");

        if (total_bytes_read == client_ctx->request_size) {
            size_t new_size = client_ctx->request_size * 2;
            char *new_buffer = (char *)realloc(client_ctx->request_buffer, new_size);
            if (new_buffer == NULL) {
                free(client_ctx->request_buffer);
                client_ctx->request_buffer = NULL;
                close(client_ctx->client_fd);
                printf("ERR_REALLOC\n");
                return ERR_REALLOC;
            }
            client_ctx->request_buffer = new_buffer;
            client_ctx->request_size = new_size;
            printf("REALLOC SUCCEEDED\n");
        }
        break;
    }
    printf("WHILE LOOP SUCCEEDED\n");

    if (bytes_read < 0) {
        close(client_ctx->client_fd);
        printf("ERR_READ\n");
        return ERR_READ;
    }

    printf("Received request:\n%s\n", client_ctx->request_buffer);

    close(client_ctx->client_fd);
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

int handle_response(client_context_t *client_ctx) {
    return SUCCESS;

}