#include "main.h"

int main() {
    int server_file_descriptor = socket(DOMAIN_IPV4, PROTOCOL_TCP, PROTOCOL_NUMBER);
    if (server_file_descriptor < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    address.sin_family = DOMAIN_IPV4;
    address.sin_addr.s_addr = LOOPBACK_ADDRESS;
    address.sin_port = htons(PORT);

    if (bind(server_file_descriptor, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Failed to bind port");
        close(server_file_descriptor);
        exit(EXIT_FAILURE);
    }

    if (listen(server_file_descriptor, 5) < 0) {
        perror("Failed to listen on incoming connections");
        close(server_file_descriptor);
        exit(EXIT_FAILURE);
    }

    printf("Listening on port: %d\n", PORT);

    close(server_file_descriptor);
    return 0;
}