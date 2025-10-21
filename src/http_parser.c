#include <string.h>
#include <stdio.h>

#include "http_parser.h"

int parse_http_request(http_request_t *p_http_request) {
    enum http_method_e http_method = parse_request_method(p_http_request);
    if (http_method == GET) {
        p_http_request->http_parser.p_http_path = parse_request_path(p_http_request);
        printf("The size of the http path is %zu\n", p_http_request->http_parser.http_path_len);
        parse_http_version(p_http_request);
        return 200;
    }
    return 404;
}

enum http_method_e parse_request_method(http_request_t *p_http_request) {
    size_t max_method_len = MAX_HTTP_METHOD_LENGTH;

    if (p_http_request == NULL ||
        p_http_request->p_request_buffer == NULL)
        return UNKNOWN_METHOD;

    if (max_method_len > p_http_request->request_size)
        return UNKNOWN_METHOD;

    char *p_method = p_http_request->p_request_buffer;
    size_t method_size = 0;

    struct http_method_pattern_s {
        char *p_pattern;
        size_t length;
        enum http_method_e http_method;
    };

    struct http_method_pattern_s a_patterns[] = {
        { "GET", 3, GET},
        { "HEAD", 4, HEAD},
        { "POST", 4, POST},
        { "PUT", 3, PUT},
        { "DELETE", 6, DELETE},
        { "CONNECT", 7, CONNECT},
        { "OPTIONS", 7, OPTIONS},
        { "TRACE", 5, TRACE},
        { "PATCH", 5, PATCH}
    };

    /*  
        Search the buffer until max length for the HTTP method is reached
        or until we find the first space after the HTTP method.
    */
    for (size_t i = 0; i < max_method_len; i++) {
        if (p_method[i] == SPACE) {
            method_size = i;

            /*
                Parse the HTTP method.                
            */
            for (size_t j = 0; j < sizeof(a_patterns)/sizeof(a_patterns[0]); j++) {

                /*
                    Find what HTTP method is requested and make sure it is formatted correctly.
                */
                if (method_size == a_patterns[j].length &&
                    memcmp(p_method, a_patterns[j].p_pattern, a_patterns[j].length) == 0) {

                    // ** Set the path pointer to the first character of the request line's path
                    p_http_request->http_parser.p_http_path = p_method + method_size;
                    
                    return a_patterns[j].http_method;
                }
            }

        }
    }

    p_http_request->http_parser.p_http_path = NULL;
    return UNKNOWN_METHOD;
}

char* parse_request_path(http_request_t *p_http_request) {
    size_t max_path_len = MAX_HTTP_PATH_LENGTH;
    http_path_t http_path;
    http_path.path_len = 0;

    if (p_http_request == NULL ||
        p_http_request->p_request_buffer == NULL) {
            p_http_request->http_parser.p_http_path = NULL;
            return "/index.html";
        }
        
    if (max_path_len > p_http_request->request_size) {
        max_path_len = p_http_request->request_size - (p_http_request->http_method_len + 1);
    }

    size_t path_size = 0;
    size_t http_method_len = p_http_request->http_method_len + 1;
    http_path.p_path = p_http_request->p_request_buffer + http_method_len;

    for (size_t i = 0; i < max_path_len; i++) {
        if (http_path.p_path[i] == SPACE) {
            path_size = i;

            if (path_size == 1 && http_path.p_path[0] == '/') {
                http_path.path_len = path_size;
                return http_path;
            }
        }
    }

    http_path.path_len = 0;
    http_path.p_path = NULL;
    return http_path;
}

http_version_t parse_http_version(http_req_t *p_http_request) {
    size_t max_version_len = MAX_HTTP_VERSION_LENGTH;
    
    if (p_http_request == NULL ||
        p_http_request->p_request_buffer == NULL)
        return UNKNOWN_VERSION;

    if (max_version_len > p_http_request->request_size)
        return UNKNOWN_VERSION;

    if (p_http_request->http_path.p_path == NULL)
        return UNKNOWN_VERSION;

    char *p_version = p_http_request->http_path.p_path + (p_http_request->http_path.path_len + 1);
    size_t version_size = 0;

    http_version_pattern_t a_patterns[] = {
        { "HTTP/1.0", 8, HTTP_1_0},
        { "HTTP/1.1", 8, HTTP_1_1}
    };

    /*  
        Search the buffer until max length for the HTTP method is reached
        or until we find the first space after the HTTP method.
    */
    for (size_t i = 0; i < max_version_len; i++) {
        if (p_version[i] == CR && p_version[i + 1] == LF) {
            version_size = i;

            /*
                Parse the HTTP method.                
            */
            for (size_t j = 0; j < sizeof(a_patterns)/sizeof(a_patterns[0]); j++) {

                /*
                    Find what HTTP method is requested and make sure it is formatted correctly.
                */
                if (version_size == a_patterns[j].length &&
                    memcmp(p_version, a_patterns[j].p_pattern, a_patterns[j].length) == 0) {
                    
                    printf("Parsed http version: %s", a_patterns[j].p_pattern);

                    return a_patterns[j].http_version;
                }
            }

        }
    }

    return UNKNOWN_VERSION;
}

void parse_req_headers(http_req_t *p_http_request) {

}