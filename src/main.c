#include <stdio.h>

#include "main.h"
#include "server.h"

int main() {

    struct http_server_s *http_server = http_server_init(8080);
    
    if (http_server == NULL) {
        return 1;
    }

    http_server_listen(http_server);

    return 0;
}