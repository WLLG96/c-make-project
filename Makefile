CC=gcc
CFLAGS=-Wall -pthread

all: server client

server: server.o list.o
	$(CC) $(CFLAGS) server.o list.o -o server

client: client.o
	$(CC) $(CFLAGS) client.o -o client

server.o: server.c list.h
	$(CC) $(CFLAGS) -c server.c -o server.o

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c -o list.o

client.o: client.c
	$(CC) $(CFLAGS) -c client.c -o client.o

clean:
	rm -f server client server.o list.o client.o
