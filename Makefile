CC = gcc
CFLAGS = -std=c99 -Iinclude -Wall -Werror
DEBUG_FLAGS = -g
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

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean