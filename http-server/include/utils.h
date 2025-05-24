#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

typedef enum {
    ERR_UNKNOWN = -1,
    SUCCESS = 0,
    ERR_SOCKET = 1,
    ERR_BIND = 2,
    ERR_LISTEN = 3,
    ERR_ACCEPT = 4,
    ERR_MALLOC = 5,
    ERR_REALLOC = 6,
    ERR_READ = 7,
} ErrorCode;

typedef struct {
    char *base;
    size_t size;
    size_t used;
} memory_arena_t;

void handle_error(ErrorCode error_code);
void arena_init(memory_arena_t *arena, size_t size);
void arena_free(memory_arena_t *arena);
void *arena_alloc(memory_arena_t *arena, size_t alloc_size);

#endif