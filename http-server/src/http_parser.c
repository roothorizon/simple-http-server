#include "http_parser.h"

#include <string.h>

uint16_t parse_http_request(http_req_t *p_http_request) {
    http_method_t http_method = parse_req_method(p_http_request);
    if (http_method == GET) {
        return 200;
    }
    return 404;
}

void parse_req_line(http_req_t *p_http_request) {

}

void parse_req_headers(http_req_t *p_http_request) {

}

http_method_t parse_req_method(http_req_t *p_http_request) {
    size_t max_method_length = MAX_HTTP_METHOD_LENGTH;

    if (p_http_request == NULL ||
        p_http_request->p_request_buffer == NULL)
        return UNKNOWN;

    if (max_method_length > p_http_request->request_size)
        return UNKNOWN;

    char *p_method = p_http_request->p_request_buffer;
    size_t method_size = 0;

    http_method_pattern_t a_patterns[] = {
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
    for (size_t i = 0; i < max_method_length; i++) {
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
                    p_method += method_size + 1;
                    p_http_request->p_path = p_method;

                    // ** Set the request line's method pointer back to the first character of the method
                    p_http_request->p_method = p_http_request->p_request_buffer;
                    p_http_request->method_len = method_size;
                    
                    return a_patterns[j].http_method;
                }
            }

        }
    }

    p_http_request->p_method = NULL;
    p_http_request->method_len = 0;
    return UNKNOWN;
}