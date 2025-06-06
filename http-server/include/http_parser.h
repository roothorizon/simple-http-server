#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <stdint.h>
#define MAX_HTTP_METHOD_LENGTH 16
#define MAX_HTTP_PATH_LENGTH 4096
#define MAX_HTTP_VERSION_LENGTH 16
#define MAX_HTTP_END_LENGTH 8
#define SPACE ' '
#define CR '\r'
#define LF '\n'

#include <stdio.h>

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
    UNKNOWN_METHOD
} http_method_t;

typedef struct {
    char *p_path;
    size_t path_len;
} http_path_t;

typedef enum {
    HTTP_0_9,
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2,
    HTTP_3,
    UNKNOWN_VERSION
} http_version_t;

// ** HTTP Request struct
typedef struct {
    char *p_request_buffer;
    size_t request_size;
    http_method_t http_method;
    size_t http_method_len;
    http_path_t http_path;
    http_version_t http_version;
    size_t content_length;
} http_req_t;

// ** HTTP Response struct
typedef struct {
    char *p_response_buffer;
    size_t response_size;
    char *p_method, *p_path, *p_version, *p_end;
    char *p_host, *p_user_agent, *p_accept, *p_connection;
    char *p_body;
} http_res_t;

typedef struct {
    char *p_pattern;
    size_t length;
    http_method_t http_method;
} http_method_pattern_t;

typedef struct {
    char *p_pattern;
    size_t length;
    http_version_t http_version;
} http_version_pattern_t;

uint16_t parse_http_request(http_req_t *p_http_request);

http_method_t parse_req_method(http_req_t *p_http_request);
http_path_t parse_req_path(http_req_t *p_http_request);
http_version_t parse_http_version(http_req_t *p_http_request);
void parse_req_headers(http_req_t *p_http_request);

#endif