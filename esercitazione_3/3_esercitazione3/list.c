#include "list.h"

void add_player(Player **head, const char *name) {
    if (find_player(*head, name) != NULL) {
        return;
    }

    Player *new_player = (Player *)malloc(sizeof(Player));
    if (new_player == NULL) {
        perror("Errore allocazione memoria");
        exit(1);
    }
    strcpy(new_player->name, name);
    new_player->games_played = 0;
    new_player->points = 0;
    new_player->next = *head;
    *head = new_player;
}

Player* find_player(Player *head, const char *name) {
    while (head != NULL) {
        if (strcmp(head->name, name) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

void update_scores(Player *head, char winners[3][50]) {
    for (int i = 0; i < 3; i++) {
        Player *player = find_player(head, winners[i]);
        if (player) {
            player->games_played++;
            if (i == 0) player->points += 3;  // Primo posto: +3 punti
            else if (i == 1) player->points += 2;  // Secondo posto: +2 punti
            else if (i == 2) player->points += 1;  // Terzo posto: +1 punto
        }
    }
}

void print_players(Player *head) {
    printf("Lista giocatori iscritti:\n");
    while (head != NULL) {
        printf("Nome: %s | Partite giocate: %d | Punti: %d\n",
               head->name, head->games_played, head->points);
        head = head->next;
    }
    printf("\n");
}

void free_list(Player *head) {
    while (head != NULL) {
        Player *temp = head;
        head = head->next;
        free(temp);
    }
}

