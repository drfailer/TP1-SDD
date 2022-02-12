#include "util.h"
#include "agenda.h"
#include "tache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testFichier() {
  agenda_t *agenda = agendaViafichier("test.txt");
  printf("Agenda:\n");
  afficheAgenda(agenda);
  printf("\ntest ecriture fichier (sdtout):\n");
  ecritFichier(stdout, agenda);
  freelst(&agenda);
}

void afficheMenu() {
  system("clear");
  printf("[0]: ajout element.\n");
  printf("[1]: suppression element.\n");
  printf("[2]: quitter.\n");
  printf("\nreponse: \n");
}

void recupereLigne(char buff[], int n) {
  fgets(buff, n, stdin);
  fflush(stdin);
}

void demandeInfos(char annee[5], char semaine[3], char heure[3], int *jour) {
  printf("Saisir annee:\n");
  recupereLigne(annee, 5);
  printf("Saisir semaine:\n");
  recupereLigne(semaine, 3);
  printf("Saisir heure:\n");
  recupereLigne(heure, 3);
  printf("Saisir jour:\n");
  scanf("%d", jour);
}

void tounerLeProgramme(agenda_t *agenda) {
  int continuer = 1;
  int reponse = 0;
  int jour;
  char annee[5];
  char semaine[3];
  char heure[3];

  while (continuer) {
    afficheMenu();
    scanf("%d", &reponse);
    switch (reponse) {
      case 0:
        printf("TODO: ajout.\n");
        demandeInfos(annee, semaine, heure, &jour);
        break;
      case 1:
        printf("TODO: suppression.\n");
        demandeInfos(annee, semaine, heure, &jour);
        break;
      case 2:
        continuer = 0;
        break;
    }
  }
}

int main(int argc, char **argv) {
  agenda_t *agenda = NULL;
  if (argc > 1) {
    if (0 == strcmp(argv[1], "import") && 2 == argc) {
      printf("importation de %s.\n", argv[2]);
      agenda = agendaViafichier(argv[2]);
      tounerLeProgramme(agenda);
    }
  } else {
    testFichier();
  }
  return 0;
}
