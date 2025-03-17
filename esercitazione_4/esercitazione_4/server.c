#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "list.h"

#define PORT 8080
#define MAX_CLIENTS 5

Book *books = NULL;
pthread_mutex_t lock;

void *handle_client(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char buffer[50];
    read(sock, buffer, sizeof(buffer));

    char type = buffer[0];
    char title[20];
    int copies;

    if (type == 'L') {  // Cliente Lettore
        sscanf(buffer + 2, "%s", title);
        pthread_mutex_lock(&lock);
        Book *book = find_book(books, title);
        if (book && book->copies > 0) {
            book->copies--;
            write(sock, "Libro disponibile!", 19);
            if (book->copies == 0) remove_book(&books, title);
        } else {
            write(sock, "Libro non disponibile!", 23);
        }
        print_books(books);
        pthread_mutex_unlock(&lock);
    } else if (type == 'C') {  // Casa Editrice
        sscanf(buffer + 2, "%s %d", title, &copies);
        pthread_mutex_lock(&lock);
        if (!find_book(books, title)) {
            add_book(&books, title, copies);
        }
        print_books(books);
        pthread_mutex_unlock(&lock);
    }

    close(sock);
    free(socket_desc);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t thread_id;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, MAX_CLIENTS);

    pthread_mutex_init(&lock, NULL);
    printf("Server avviato...\n");

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket >= 0) {
            int *new_sock = malloc(sizeof(int));
            *new_sock = new_socket;
            pthread_create(&thread_id, NULL, handle_client, (void*)new_sock);
            pthread_detach(thread_id);
        }
    }

    close(server_fd);
    free_list(books);
    pthread_mutex_destroy(&lock);
    return 0;
}

