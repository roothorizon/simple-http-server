#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define TIMESTAMP_SIZE 50

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
} error_code_t;

typedef struct {
    char *base;
    size_t size;
    size_t used;
} memory_arena_t;

void handle_error(error_code_t error_code);
void arena_init(memory_arena_t *arena, size_t size);
void arena_free(memory_arena_t *arena);
void *arena_alloc(memory_arena_t *arena, size_t alloc_size);

void getDateTimeStamp(char *buffer, size_t size);

void logMessage(const char *message);
void logWarning(const char *message);
void logError(const char *message);

#endif