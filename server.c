#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include "list.h" // Includi la gestione della lista giocatori

#define PORT 8000
#define MAX_PLAYERS 10
#define MIN_PLAYERS 3

int client_sockets[MAX_PLAYERS];
int giocatori_connessi = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *gestisci_client(void *arg) {
    int sockfd = *(int *)arg;
    char nome[30];
    int ricevuto = recv(sockfd, nome, sizeof(nome), 0);

    if (ricevuto <= 0) {
        printf("Errore ricezione dati dal client.\n");
        close(sockfd);
        return NULL;
    }
    nome[ricevuto] = '\0';

    pthread_mutex_lock(&lock);

    if (!esiste_giocatore(nome)) {
        aggiungi_giocatore(nome);
    }

    client_sockets[giocatori_connessi++] = sockfd;
    printf("Giocatore %s connesso. In attesa...\n", nome);
    stampa_giocatori();

    if (giocatori_connessi >= MIN_PLAYERS) {
        printf("Simulazione partita in corso...\n");
        int ranking[MAX_PLAYERS];
        for (int i = 0; i < giocatori_connessi; i++) ranking[i] = i;
        for (int i = 0; i < giocatori_connessi; i++) {
            int rand_idx = rand() % giocatori_connessi;
            int temp = ranking[i];
            ranking[i] = ranking[rand_idx];
            ranking[rand_idx] = temp;
        }
        aggiorna_punteggio(ranking);
        char risultato[100];
        snprintf(risultato, sizeof(risultato), "Classifica: 1° %s, 2° %s, 3° %s\n", 
                 get_nome_giocatore(ranking[0]), get_nome_giocatore(ranking[1]), get_nome_giocatore(ranking[2]));
        for (int i = 0; i < giocatori_connessi; i++) {
            send(client_sockets[i], risultato, strlen(risultato), 0);
            close(client_sockets[i]);
        }
        giocatori_connessi = 0;
        printf("Partita terminata! Risultati inviati.\n");
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    srand(time(NULL));
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, MAX_PLAYERS);
    printf("Server in ascolto sulla porta %d...\n", PORT);
    
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        pthread_t thread;
        pthread_create(&thread, NULL, gestisci_client, (void *)&new_socket);
        pthread_detach(thread);
    }
    return 0;
}
