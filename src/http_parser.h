#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "main.h"

#define MAX_HTTP_METHOD_LENGTH 16
#define MAX_HTTP_PATH_LENGTH 4096
#define MAX_HTTP_VERSION_LENGTH 16
#define MAX_HTTP_END_LENGTH 8
#define SPACE ' '
#define CR '\r'
#define LF '\n'

int parse_http_request(http_request_t *p_http_request);
enum http_method_e parse_request_method(http_request_t *p_http_request);
char* parse_request_path(http_request_t *p_http_request);
enum http_version_e parse_http_version(http_request_t *p_http_request);
void parse_request_headers(http_request_t *p_http_request);

#endif