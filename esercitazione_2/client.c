#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define NUM_CLIENTS 5  // Numero di processi client simultanei

void client_process() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    int num, received_num;

    // Genera un numero unico per ogni processo
    srand(time(NULL) + getpid()); 
    num = rand() % 100;

    // Crea socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Errore creazione socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converte indirizzo IP
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Indirizzo non valido");
        exit(EXIT_FAILURE);
    }

    // Connessione al server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connessione fallita");
        exit(EXIT_FAILURE);
    }

    // Invia il numero al server
    write(sock, &num, sizeof(num));

    // Riceve la risposta
    read(sock, &received_num, sizeof(received_num));

    printf("Client %d ha inviato: %d, ricevuto: %d\n", getpid(), num, received_num);

    close(sock);
}

int main() {
    for (int i = 0; i < NUM_CLIENTS; i++) {
        pid_t pid = fork();
        if (pid == 0) {  // Processo figlio
            client_process();
            exit(0);
        }
        sleep(1);  // Ritardo per una visualizzazione più chiara
    }

    // Attende la fine di tutti i processi figli
    for (int i = 0; i < NUM_CLIENTS; i++) {
        wait(NULL);
    }

    return 0;
}
