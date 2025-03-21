CC = gcc
CFLAGS = -Wall -g

all: server agente investitore

server: server.c common.h
	$(CC) $(CFLAGS) -o server server.c

agente: agente.c common.h
	$(CC) $(CFLAGS) -o agente agente.c

investitore: investitore.c common.h
	$(CC) $(CFLAGS) -o investitore investitore.c

clean:
	rm -f server agente investitore
