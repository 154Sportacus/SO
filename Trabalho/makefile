CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

CLIENT_SRC = client.c utils.c
SERVER_SRC = server.c utils.c

CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
SERVER_OBJ = $(SERVER_SRC:.c=.o)

all: directories tracer monitor

tracer: $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

monitor: $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	mkdir -p term tmp

clean:
	rm -f $(CLIENT_OBJ) $(SERVER_OBJ) tracer monitor ClientServerConnection SC* CS*
	rm -rf term/* tmp/*

.PHONY: all directories clean

