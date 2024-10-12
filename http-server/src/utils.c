#include "utils.h"

void handle_error(ErrorCode error_code) {
    switch (error_code) {
        case ERR_SOCKET:
            printf("Error: Unable to create socket.\n");
            break;
        
        case ERR_BIND:
            printf("Error: Unable to bind socket.\n");
            break;
        
        case ERR_LISTEN:
            printf("Error: Unable to start listening for incoming connections.\n");
            break;

        case ERR_ACCEPT:
            printf("Error: Unable to accept incoming connections.\n");
            break;

        case ERR_MALLOC:
            printf("Error: Malloc was unable to allocate memory.\n");
            break;

        case ERR_REALLOC:
            printf("Error: Realloc was unable to reallocate memory.\n");
            break;

        case ERR_READ:
            printf("Error: Unable to read request buffer.\n");
            break;

        default:
            printf("Error: An unknown error occurred.\n");
            break;
    }
}