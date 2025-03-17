#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "list.h"

#define PORT 8080
#define MAX_PLAYERS 3  // Numero minimo di giocatori per avviare la partita

Player *players = NULL;
Player *connected[MAX_PLAYERS];
int player_count = 0;
pthread_mutex_t lock;

void *handle_client(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char name[50];

    // Riceve il nome del giocatore
    recv(sock, name, sizeof(name), 0);
    name[strcspn(name, "\n")] = 0;  // Rimuove eventuali caratteri di nuova riga

    pthread_mutex_lock(&lock);
    add_player(&players, name);
    connected[player_count++] = find_player(players, name);
    print_players(players);
    pthread_mutex_unlock(&lock);

    // Aspetta che il numero minimo di giocatori sia raggiunto
    while (player_count < MAX_PLAYERS) {
        sleep(1);
    }

    // Simula il gioco e sceglie i vincitori casualmente
    char winners[3][50];
    pthread_mutex_lock(&lock);
    for (int i = 0; i < 3; i++) {
        strcpy(winners[i], connected[rand() % player_count]->name);
    }
    update_scores(players, winners);
    print_players(players);
    pthread_mutex_unlock(&lock);

    // Invia i risultati ai giocatori
    send(sock, winners[0], sizeof(winners[0]), 0);
    send(sock, winners[1], sizeof(winners[1]), 0);
    send(sock, winners[2], sizeof(winners[2]), 0);

    close(sock);
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
    listen(server_fd, 5);

    pthread_mutex_init(&lock, NULL);
    printf("Server in attesa di connessioni...\n");

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket >= 0) {
            pthread_create(&thread_id, NULL, handle_client, (void*)&new_socket);
            pthread_detach(thread_id); // Libera il thread dopo la terminazione
        }
    }

    close(server_fd);
    free_list(players);
    pthread_mutex_destroy(&lock);
    return 0;
}
