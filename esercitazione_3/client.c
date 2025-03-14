#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[256] = {0};
    
    // Creazione socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Inserisce il nome del giocatore
    printf("Inserisci il tuo nome: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;

    // Invia il nome al server
    send(sock, buffer, strlen(buffer), 0);

    // Riceve il risultato della partita
    read(sock, buffer, sizeof(buffer));
    printf("Risultato partita: %s\n", buffer);

    close(sock);
    return 0;
}
