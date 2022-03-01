#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************/
/* FONCTIONS SECONDAIRES                                                     */
/*****************************************************************************/

/* récupère une suite de `n` nombres et les met dans tab
 * retourne le pointeur `s`
 */
char *parseSuiteNombres(char *s, char tab[], int n) {
  int i;
  for (i = 0; *s && i < n; ++i, ++s) {
    tab[i] = *s;
  }
  return s;
}

/* On utilise `getchar()` pour demander à l'utilisateur de confirmer lors d'un
 * affichage, cependant, cette fonction ne capture que le premier caratère dans
 * `stdin`. De ce fait, les caractère restants dans stdin seront capturés (à la
 * place de la prochaine entrée) au prochain getchar.
 * IMPORTANT: `fflush` ne doit pas être utilisée pour flush stdin
 * SOURCE: https://en.cppreference.com/w/cpp/io/c/fflush
 */
void flushInput() {
  char c;
  while ((c = fgetc(stdin)) != EOF && c != '\n')
    ;
}

/*****************************************************************************/
/* FONCTIONS DE SAISIE                                                       */
/*****************************************************************************/

/* demande les informations à l'utilisateur lors de la création d'un nouvel
 * élément
 */
void demandeInfos(char annee[TAILLE_ANNEE], char semaine[TAILLE_SEMAINE],
                  char heure[TAILLE_HEURE], int *jour) {
  do {
    printf("Saisir annee:\n");
    scanf("%4s%*c", annee);
  } while (atoi(annee) < 2021);

  do {
    printf("Saisir semaine:\n");
    scanf("%2s%*c", semaine);
  } while (atoi(semaine) < 1 || atoi(semaine) > 53);

  do {
    printf("Saisir jour:\n");
    scanf("%d%*c", jour);
  } while (*jour < 1 || *jour > 7);

  do {
    printf("Saisir heure:\n");
    scanf("%2s%*c", heure);
  } while (atoi(heure) < 0 || atoi(heure) > 24);
}

/* Permet à l'utilisateur de saisir le nom de l'activité qu'il souhaite
 * planifier.
 */
void demandeNom(char nom[TAILLE_NOM]) {
  printf("Saisir l'activité :\n");
  scanf("%10s%*c", nom);
}

/* Demande à l'utilisateur de confirmer la Suppression de l'agenda courrant
 * lors de l'import d'un nouveau fichier.
 */
char demandeConfirmation() {
  char reponse = 'n';
  printf("Cette action va effacer l'agenda, courrant, confirmez-vous cette "
         "opération? [y/n]\n");
  scanf("%c%*c", &reponse);
  return (reponse == 'y') ? 1 : 0;
}

/*****************************************************************************/
/* UI                                                                        */
/*****************************************************************************/

/* Affiche le menu indiquant à l'utilisateur quoi tapper pour interagire avec
 * l'application.
 */
void afficheMenu() {
  clear();
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

/* Affiche message de fin quand l'utilisateur quite le programme */
void afficheMessageFin() {
  clear();
  printf("\t ======================================================\n");
  printf("\t|                                                      |\n");
  printf("\t|                      AU REVOIR !                     |\n");
  printf("\t|                                                      |\n");
  printf("\t ======================================================\n");
}

/* Fait un appel systeme à la fonction clear si le programme n'est pas compilé
 * en mode DEBUG
 */
void clear() {
  if (DEBUG != 1) {
    system("clear");
  }
}
