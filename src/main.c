#include "agenda.h"
#include "tache.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Simple fonction de test de lecture dans un fichier
 * TODO: faire un vrai fichier de test
 */
agenda_t *testFichier() {
  agenda_t *agenda = agendaViafichier("test/test.txt");
  printf("Agenda:\n");
  afficheAgenda(agenda);
  printf("\ntest ecriture fichier (sdtout):\n");
  ecritFichier(stdout, agenda);
  return agenda;
}

/* exécute des action en fonction de `entree`, qui est le caratère entré par l'
 * tilisateur dans `run`
 */
agenda_t *gestionEvenements(char entree, agenda_t *agenda, int *continuer) {
  int jour;
  char annee[TAILLE_ANNEE];
  char semaine[TAILLE_SEMAINE];
  char heure[TAILLE_HEURE];
  char nom[TAILLE_NOM];
  char nomFichier[50] = "save_agenda.txt";

  switch (entree) {
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
    scanf("%*c");
    break;
  case '3':
  case '4':
    clear();
    printf("Saisir nom du fichier\n");
    scanf("%s%*c", nomFichier);
    if (entree == 3) {
      sauvFichier(nomFichier, agenda);
      getchar();
      flushInput();
    } else {
      if (demandeConfirmation()) {
        freelst(&agenda); // suppression de l'ancient agenda
        agenda = agendaViafichier(nomFichier); // importation du nouvel agenda
        printf("agenda: %p\n", agenda);
      }
    }
    break;
  default:
    afficheMessageFin();
    *continuer = 0;
    break;
  }
  return agenda;
}

/* Boucle principale: Execution de l'application */
void run(agenda_t *agenda) {
  int continuer = 1;
  char reponse;

  while (continuer) {
    afficheMenu();
    scanf("%c%*c", &reponse);
    printf("reponse: %c\n", reponse);
    agenda = gestionEvenements(reponse, agenda, &continuer);
  }
  freelst(&agenda);
}

int main(int argc, char **argv) {
  agenda_t *agenda = NULL;
  if (argc > 1) {
    if (0 == strcmp(argv[1], "import") && 3 == argc) {
      printf("importation de %s.\n", argv[2]);
      agenda = agendaViafichier(argv[2]);
      run(agenda);
    }
  } else {
    agenda = testFichier();
    run(agenda);
  }
  return 0;
}
