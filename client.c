/* client.c - Client per il gioco multi-utente */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8000

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char nome[30], buffer[256];

    // Creazione socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // Connessione al server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connessione fallita");
        exit(EXIT_FAILURE);
    }

    // Inserimento del nome del giocatore
    printf("Inserisci il tuo nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    // Invio del nome al server
    send(sockfd, nome, strlen(nome), 0);

    // Ricezione del risultato della partita
    read(sockfd, buffer, sizeof(buffer));
    printf("Risultato partita: %s\n", buffer);

    close(sockfd);
    return 0;