#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_PLAYERS 5  // Numero massimo di giocatori
#define MIN_PLAYERS 3  // Numero minimo per avviare la partita

typedef struct {
    char name[50];
    int games_played;
    int score;
} Player;

Player players[MAX_PLAYERS];
int connected_players = 0;
int client_sockets[MAX_PLAYERS];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Funzione per stampare la lista dei giocatori iscritti
void print_players() {
    printf("\nElenco giocatori iscritti:\n");
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (strlen(players[i].name) > 0) {
            printf("Nome: %s | Partite: %d | Punti: %d\n", 
                   players[i].name, players[i].games_played, players[i].score);
        }
    }
}

// Simula la partita assegnando punti ai primi 3 giocatori
void start_game() {
    printf("\nPartita in corso...\n");

    int rankings[MAX_PLAYERS];
    for (int i = 0; i < connected_players; i++) {
        rankings[i] = i;
    }

    // Mescola i giocatori per determinare il vincitore casualmente
    for (int i = 0; i < connected_players; i++) {
        int rand_index = rand() % connected_players;
        int temp = rankings[i];
        rankings[i] = rankings[rand_index];
        rankings[rand_index] = temp;
    }

    // Assegna punti ai primi tre classificati
    players[rankings[0]].score += 3;  // Primo classificato
    players[rankings[1]].score += 2;  // Secondo classificato
    players[rankings[2]].score += 1;  // Terzo classificato

    // Aggiorna il numero di partite giocate
    for (int i = 0; i < connected_players; i++) {
        players[i].games_played++;
    }

    // Comunica il risultato ai client
    char result_message[256];
    sprintf(result_message, "Risultati: 1° %s - 2° %s - 3° %s\n",
            players[rankings[0]].name,
            players[rankings[1]].name,
            players[rankings[2]].name);

    for (int i = 0; i < connected_players; i++) {
        send(client_sockets[i], result_message, strlen(result_message), 0);
        close(client_sockets[i]);
    }

    connected_players = 0;
    printf("\nPartita terminata! I risultati sono stati inviati.\n");
    print_players();
}

// Gestione di ogni nuovo client
void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[50];
    read(client_socket, buffer, sizeof(buffer));
    
    pthread_mutex_lock(&lock);

    // Controlla se il giocatore è già iscritto
    int player_index = -1;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (strcmp(players[i].name, buffer) == 0) {
            player_index = i;
            break;
        }
    }

    // Se il giocatore è nuovo, lo aggiunge
    if (player_index == -1) {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (strlen(players[i].name) == 0) {
                strcpy(players[i].name, buffer);
                players[i].games_played = 0;
                players[i].score = 0;
                player_index = i;
                break;
            }
        }
    }

    client_sockets[connected_players] = client_socket;
    connected_players++;

    printf("Giocatore %s si è connesso!\n", players[player_index].name);
    print_players();

    if (connected_players >= MIN_PLAYERS) {
        start_game();
    }

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Creazione socket del server
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, MAX_PLAYERS);

    printf("Server in ascolto sulla porta %d...\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void *)&new_socket);
        pthread_detach(thread);
    }

    return 0;
}
