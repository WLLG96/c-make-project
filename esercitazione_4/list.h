#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book {
    char title[20];
    int copies;
    struct Book *next;
} Book;

void add_book(Book **head, const char *title, int copies);
void remove_book(Book **head, const char *title);
Book* find_book(Book *head, const char *title);  
void print_books(Book *head);
void free_list(Book *head);

#endif  