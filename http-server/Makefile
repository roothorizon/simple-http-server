CC = gcc
CFLAGS = -std=c99 -Iinclude -Wall -Werror
SRC = src/main.c \
	  src/init_server.c \
	  src/connection_handler.c \
	  src/utils.c \
	  src/http_parser.c
OBJ = $(SRC:.c=.o)
TARGET = simple-http-server

all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean