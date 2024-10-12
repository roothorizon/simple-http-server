CC = gcc
CFLAGS = -Iinclude -Wall -Werror
SRC = src/main.c \
	  src/init_server.c \
	  src/connection_handler.c \
	  src/utils.c
OBJ = $(SRC:.c=.o)
TARGET = simple-http-server
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean
.PHONY: all