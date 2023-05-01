CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

CLIENT_SRC = client.c myreadln.c
SERVER_SRC = server.c myreadln.c

CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
SERVER_OBJ = $(SERVER_SRC:.c=.o)

all: tracer monitor

tracer: $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

monitor: $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(CLIENT_OBJ) $(SERVER_OBJ) tracer monitor
