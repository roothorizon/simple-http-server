#include <stdio.h>

#include "main.h"
#include "server.h"

int main() {

    http_server_t *http_server = http_server_init(8080);
    
    if (http_server == NULL) {
        return 1;
    }

    run_server_loop(http_server);

    return 0;
}