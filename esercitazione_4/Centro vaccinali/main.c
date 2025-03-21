#include <stdio.h>
#include <string.h>
#include "common.h"

int main() {
    PersonaVaccinata p1 = { "Mario Rossi", "RSSMRA80A01H501U", "Pfizer" };
    printf("Nome: %s\nCodice fiscale: %s\nVaccino: %s\n", p1.nome, p1.codiceFiscale, p1.vaccino);
    return 0;
}
