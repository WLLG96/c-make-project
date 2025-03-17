#include "list.h"

Book* find_book(Book *head, const char *title) {  
    while (head != NULL) {
        if (strcmp(head->title, title) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

void add_book(Book **head, const char *title, int copies) {
    if (find_book(*head, title) != NULL) {
        return;
    }

    Book *new_book = (Book *)malloc(sizeof(Book));
    if (!new_book) {
        perror("Errore allocazione memoria");
        exit(1);
    }
    strcpy(new_book->title, title);
    new_book->copies = copies;
    new_book->next = *head;
    *head = new_book;
}

void remove_book(Book **head, const char *title) {
    Book *temp = *head, *prev = NULL;
    while (temp != NULL && strcmp(temp->title, title) == 0) {
        *head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && strcmp(temp->title, title) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;
    prev->next = temp->next;
    free(temp);
}

void print_books(Book *head) {
    printf("Lista libri disponibili:\n");
    while (head != NULL) {
        printf("Titolo: %s | Copie disponibili: %d\n", head->title, head->copies);
        head = head->next;
    }
}

void free_list(Book *head) {
    while (head != NULL) {
        Book *temp = head;
        head = head->next;
        free(temp);
    }
}
