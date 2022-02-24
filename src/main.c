#include "agenda.h"
#include "tache.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

agenda_t *testFichier() {
  agenda_t *agenda = agendaViafichier("test.txt");
  printf("Agenda:\n");
  afficheAgenda(agenda);
  printf("\ntest ecriture fichier (sdtout):\n");
  ecritFichier(stdout, agenda);
  return agenda;
}

void recupereLigne(char buff[], int n) {
  fflush(stdin);
  fgets(buff, n, stdin);
  buff[n - 2] = '\0';
  fflush(stdin);
}

void demandeInfos(char annee[5], char semaine[3], char heure[3], int *jour) {
  char annee_temp[6];
  char semaine_temp[4];
  char heure_temp[4];

  do {
    printf("Saisir annee:\n");
    recupereLigne(annee_temp, 6);
  } while (atoi(annee_temp) < 2021);
  for (int i = 0; i < 5; i++) {
    annee[i] = annee_temp[i];
  }

  do {
    printf("Saisir semaine:\n");
    recupereLigne(semaine_temp, 4);
  } while (atoi(semaine_temp) < 1 || atoi(semaine_temp) > 53);
  for (int i = 0; i < 3; i++) {
    semaine[i] = semaine_temp[i];
  }

  do {
    printf("Saisir jour:\n");
    scanf("%d%*c", jour);
  } while (*jour < 1 || *jour > 7);

  do {
    printf("Saisir heure:\n");
    recupereLigne(heure_temp, 4);
  } while (atoi(heure_temp) < 0 || atoi(heure_temp) > 24);
  for (int i = 0; i < 3; i++) {
    heure[i] = heure_temp[i];
  }
}

void demandeNom(char nom[10]) {
  int i;
  char nom_temp[12];
  printf("Saisir l'activité :\n");
  recupereLigne(nom_temp, 12);
  for (i = 0; i < (int)strlen(nom_temp) - 1; i++) {
    nom[i] = nom_temp[i];
  }
  for (; i < 10; i++) {
    nom[i] = ' ';
  }
}

void afficheMenu() {
  system("clear");
  printf("\t ======================================================\n");
  printf("\t|                                                      |\n");
  printf("\t|                         AGENDA                       |\n");
  printf("\t|                                                      |\n");
  printf("\t ======================================================\n");
  printf("\t|                                                      |\n");
  printf("\t|                [0]: Ajout element                    |\n");
  printf("\t|                [1]: Suppression element              |\n");
  printf("\t|                [2]: Afficher l'agenda                |\n");
  printf("\t|                [3]: Sauvegarde l'agenda              |\n");
  printf("\t|                [4]: Importer un fichier              |\n");
  printf("\t|                [Autre]: Quitter                      |\n");
  printf("\t|                                                      |\n");
  printf("\t ------------------------------------------------------\n\n");
}

void tounerLeProgramme(agenda_t *agenda) {
  int continuer = 1;
  char reponse;
  int jour;
  char annee[TAILLE_ANNEE];
  char semaine[TAILLE_SEMAINE];
  char heure[TAILLE_HEURE];
  char nom[TAILLE_NOM];
  char nomFichier[50] = "save_agenda.txt";

  while (continuer) {
    afficheMenu();
    scanf("%c%*c", &reponse);
    switch (reponse) {
    case '0':
      demandeInfos(annee, semaine, heure, &jour);
      demandeNom(nom);
      agenda = ajouteAgenda(agenda, annee, semaine, jour, heure, nom);
      break;
    case '1':
      demandeInfos(annee, semaine, heure, &jour);
      agenda = supprimeElt(agenda, annee, semaine, jour, heure);
      break;
    case '2':
      afficheAgenda(agenda);
      getchar();
      break;
    case '3':
      system("clear");
      printf("Saisir nom du fichier\n");
      scanf("%s%*c", nomFichier);
      sauvFichier(nomFichier, agenda);
      getchar();
      break;
    case '4':
      system("clear");
      printf("Saisir nom du fichier\n");
      scanf("%s%*c", nomFichier);
      printf("Cette action va effacer l'agenda, courrant, confirmez-vous cette "
             "opération? [y/n]\n");
      scanf("%c*c", &reponse);
      if (reponse == 'y') {
        freelst(&agenda); // suppression de l'ancient agenda
        agenda = agendaViafichier(nomFichier); // importation du nouvel agenda
      }
      break;
    default:
      system("clear");
      printf("\t ======================================================\n");
      printf("\t|                                                      |\n");
      printf("\t|                      AU REVOIR !                     |\n");
      printf("\t|                                                      |\n");
      printf("\t ======================================================\n");
      continuer = 0;
      break;
    }
  }
  freelst(&agenda);
}

int main(int argc, char **argv) {
  agenda_t *agenda = NULL;
  if (argc > 1) {
    if (0 == strcmp(argv[1], "import") && 3 == argc) {
      printf("importation de %s.\n", argv[2]);
      agenda = agendaViafichier(argv[2]);
      tounerLeProgramme(agenda);
    }
  } else {
    agenda = testFichier();
    tounerLeProgramme(agenda);
    freelst(&agenda);
  }
  return 0;
}
