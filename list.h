#ifndef LIST_H
#define LIST_H

#define MAX_PLAYERS 10

typedef struct {
    char nome[30];
    int num_partite;
    int punteggio;
} Giocatore;

// Dichiarazioni delle funzioni
void inizializza_lista();
void aggiungi_giocatore(const char *nome);
int esiste_giocatore(const char *nome);
void stampa_giocatori();
void aggiorna_punteggio(int *ranking);
const char* get_nome_giocatore(int index);

#endif // LIST_H
