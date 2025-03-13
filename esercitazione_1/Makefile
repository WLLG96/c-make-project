# Compilatore
CC = gcc

# Opzioni di compilazione
CFLAGS = -Wall -Wextra -I.

# File sorgenti e oggetti
SRCS = main.c fast_trigo.c
OBJS = main.o fast_trigo.o

# Nome dell'eseguibile
TARGET = programma

# Regola principale: compila il programma
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Regole per compilare i file .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Pulizia dei file compilati
clean:
	rm -f $(OBJS) $(TARGET)
