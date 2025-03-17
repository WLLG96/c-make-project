CC = gcc
CFLAGS = -Wall -pthread

all: server

server: server.c list.c list.h
	$(CC) $(CFLAGS) -o server server.c list.c

clean:
	rm -f server