#ifndef MAIN_H
#define MAIN_H

#include <arpa/inet.h>

typedef struct {
    int server_fd;
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;
} http_server_t;

enum http_method_e {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    UNKNOWN_METHOD
};

enum http_version_e {
    HTTP_0_9,
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2,
    HTTP_3,
    UNKNOWN_VERSION
};

struct http_parser_s {
    char *p_http_path;
    size_t http_path_len;
    enum http_method_e http_method;
    enum http_version_e http_version;
};

typedef struct {
    int request_fd;
    struct sockaddr_in request_addr;
    socklen_t request_addr_len;
    http_server_t *http_server;
    char *p_request_buffer;
    size_t request_size;
    struct http_parser_s http_parser;
} http_request_t;

#endif