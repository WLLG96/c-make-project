#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player {
    char name[50];
    int games_played;
    int points;
    struct Player *next;
} Player;

void add_player(Player **head, const char *name);
Player* find_player(Player *head, const char *name);
void update_scores(Player *head, char winners[3][50]);
void print_players(Player *head);
void free_list(Player *head);

#endif

