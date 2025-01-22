#ifndef UTILS_H
#define UTILS_H

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
} ErrorCode;

void handle_error(ErrorCode error_code);

void getDateTimeStamp(char *buffer, size_t size);

void logMessage(const char *message);
void logWarning(const char *message);
void logError(const char *message);

#endif