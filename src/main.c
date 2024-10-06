#include "main.h"

int main() {
    int server_fd, client_fd; // Server File Descriptor & Client File Descriptor
    struct sockaddr_in server_addr; // Creates an instance of the sockaddr_in struct
    socklen_t server_addr_size = sizeof(server_addr); // Size in bytes of the sockaddr_in struct
    socklen_t client_addr_size;
    char buffer[BUFFER_SIZE] = {0}; // Creates a buffer to read incoming data
    unsigned char running = 1; // Controls when to exit out of the program loop

    // Initialize the server_addr struct
    server_addr.sin_family = DOMAIN_IPV4;
    server_addr.sin_addr.s_addr = LOOPBACK_ADDRESS;
    server_addr.sin_port = PORT;

    // Create the socket
    server_fd = socket(DOMAIN_IPV4, PROTOCOL_TCP, PROTOCOL_NUMBER);
    if (server_fd < 0)
        handle_error("Server socket creation failed");

    // Bind IP address and port to socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, server_addr_size) < 0)
        handle_error("Server socket bind failed");

    // Start listening for incoming connections
    if (listen(server_fd, MAX_CONNECTION_BACKLOG) < 0)
        handle_error("Server socket listening failed");

    printf("Listening for incoming connections on: %s, port: %d\n",
            inet_ntoa(server_addr.sin_addr), // Converts the ip address from network byte order to host byte order (not thread-safe)
            ntohs(server_addr.sin_port)); // Converts the port from network byte order to host byte order
    while (running) {

        // Accept a connection request
        client_fd = accept(server_fd, 
        (struct sockaddr *)&server_addr, 
        &server_addr_size);

        if (client_fd < 0) {
            close(client_fd);
            handle_error("Client connection failed");
        }

        read(client_fd, buffer, BUFFER_SIZE);
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
                send(client_fd, response, strlen(response), 0);
                printf("%.*s", BUFFER_SIZE, response);

                char file_buffer[BUFFER_SIZE];
                while (fgets(file_buffer, BUFFER_SIZE, file)) {
                    send(client_fd, file_buffer, strlen(file_buffer), 0);
                }
                fclose(file);
            } else {
                char *not_found = "HTTP/1.1 404 Not Found\r\n\r\n";
                send(client_fd, not_found, strlen(not_found), 0);
                printf("%.*s", strlen(not_found), not_found);
            }
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}