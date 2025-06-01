#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#define MAX_HTTP_METHOD_LENGTH 16
#define MAX_HTTP_PATH_LENGTH 4096
#define MAX_HTTP_VERSION_LENGTH 16
#define MAX_HTTP_END_LENGTH 8
#define SPACE ' '
#define CR '\r'
#define LF '\n'

#include <stdio.h>

// ** HTTP Request struct
typedef struct {
    char *p_request_buffer;
    size_t request_size;
    char *p_method, *p_path, *p_version;
    size_t method_len, path_len, version_len;
    char *p_host, *p_user_agent, *p_accept, *p_connection;
} http_req_t;

// ** HTTP Response struct
typedef struct {
    char *p_response_buffer;
    size_t response_size;
    char *p_method, *p_path, *p_version, *p_end;
    char *p_host, *p_user_agent, *p_accept, *p_connection;
    char *p_body;
} http_res_t;

typedef enum {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    UNKNOWN
} http_method_t;

typedef struct {
    char *p_pattern;
    size_t length;
    http_method_t http_method;
} http_method_pattern_t;

void parse_http_request(http_req_t *p_http_request);
void parse_req_line(http_req_t *p_http_request);
void parse_req_headers(http_req_t *p_http_request);

http_method_t parse_req_method(http_req_t *p_http_request);

#endif