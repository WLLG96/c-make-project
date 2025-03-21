#ifndef COMMON_H
#define COMMON_H

#define MAX_NOME 100
#define MAX_CF 20
#define MAX_VACCINO 50

typedef struct {
    char nome[MAX_NOME];
    char codiceFiscale[MAX_CF];
    char vaccino[MAX_VACCINO];
} PersonaVaccinata;

#endif
