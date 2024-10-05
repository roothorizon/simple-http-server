#include "main.h"

int main() {
    int server_file_descriptor, client_file_descriptor;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE] = {0};
    int server_address_length = sizeof(server_address);
    int running = 1;

    server_file_descriptor = socket(DOMAIN_IPV4, PROTOCOL_TCP, PROTOCOL_NUMBER);
    if (server_file_descriptor < 0) {
        perror("Server socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = DOMAIN_IPV4;
    server_address.sin_addr.s_addr = LOOPBACK_ADDRESS;
    server_address.sin_port = htons(PORT);

    if (bind(server_file_descriptor, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Failed to bind port");
        close(server_file_descriptor);
        exit(EXIT_FAILURE);
    }

    if (listen(server_file_descriptor, MAX_CONNECTION_BACKLOG) < 0) {
        perror("Failed to listen on incoming connections");
        close(server_file_descriptor);
        exit(EXIT_FAILURE);
    }

    printf("Listening on port: %d\n", PORT);
    while (running) {

        client_file_descriptor = accept(server_file_descriptor, 
        (struct sockaddr *)&server_address, 
        (socklen_t *)&server_address_length);
        if (client_file_descriptor < 0) {
            perror("Client socket creation failed");
            close(client_file_descriptor);
        }

        read(client_file_descriptor, buffer, BUFFER_SIZE);

        printf("%.*s", BUFFER_SIZE, buffer);

        close(client_file_descriptor);
        close(server_file_descriptor);
        running = 0;
    }

    close(server_file_descriptor);
    return 0;
}