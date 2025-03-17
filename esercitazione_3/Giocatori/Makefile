CC = gcc
CFLAGS = -Wall -pthread
OBJ = list.o server.o

all: server

server: $(OBJ)
	$(CC) $(CFLAGS) -o server $(OBJ)

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

server.o: server.c list.h
	$(CC) $(CFLAGS) -c server.c

clean:
	rm -f *.o server

