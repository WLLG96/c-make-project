#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

Giocatore giocatori[MAX_PLAYERS]; // Definizione della variabile globale
int num_giocatori = 0; // Definizione della variabile globale

void inizializza_lista() {
    num_giocatori = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        giocatori[i].nome[0] = '\0';
        giocatori[i].num_partite = 0;
        giocatori[i].punteggio = 0;
    }
}

void aggiungi_giocatore(const char *nome) {
    if (num_giocatori < MAX_PLAYERS) {
        strcpy(giocatori[num_giocatori].nome, nome);
        giocatori[num_giocatori].num_partite = 0;
        giocatori[num_giocatori].punteggio = 0;
        num_giocatori++;
    }
}

int esiste_giocatore(const char *nome) {
    for (int i = 0; i < num_giocatori; i++) {
        if (strcmp(giocatori[i].nome, nome) == 0) {
            return 1;
        }
    }
    return 0;
}

void stampa_giocatori() {
    printf("\nGiocatori registrati:\n");
    for (int i = 0; i < num_giocatori; i++) {
        printf("Nome: %s | Partite: %d | Punti: %d\n", 
               giocatori[i].nome, giocatori[i].num_partite, giocatori[i].punteggio);
    }
}

void aggiorna_punteggio(int *ranking) {
    if (num_giocatori < 3) return;  // Evita errori se ci sono meno di 3 giocatori
    giocatori[ranking[0]].punteggio += 3;
    giocatori[ranking[1]].punteggio += 2;
    giocatori[ranking[2]].punteggio += 1;
    for (int i = 0; i < num_giocatori; i++) {
        giocatori[i].num_partite++;
    }
}

const char* get_nome_giocatore(int index) {
    if (index >= 0 && index < num_giocatori) {
        return giocatori[index].nome;
    }
    return "N/A";
}


    
