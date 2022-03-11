/*               FICHIER UTIL.C :
 *
 * Contient les fonctions annexes du programme
 */

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************/
/* FONCTIONS SECONDAIRES                                                     */
/*****************************************************************************/

/*=============================================================================
 *                    Fonction parseSuiteNombres :
 *=============================================================================
 *
 * Récupère une suite de `n` nombres et les met dans tab en consommant
 * dans s les caractères extraits
 *
 * En entrée :
 *    - s : Chaîne de caractères d'où extraire les n nombres
 *    - tab : Chaîne de caractères où stocker les n nombres extraits
 *    - n : Nombre de nombres à extraire de s
 *
 * Variables locales :
 *    - i : Entier permettant le parcours de s et de tab
 *
 * En sortie :
 *    - s : Chaîne de caractères où ont été supprimés n caractères
 *=============================================================================
 */

char *parseSuiteNombres(char * s, char tab[], int n) {

  int i;

  // Pour i de 0 à n et tel que la chaîne s n'est pas réduite à null
  for (i = 0; *s && i < n; ++i, ++s) {
    tab[i] = *s;
  }

  return s;
}

/*=============================================================================
 *                             Fonction flushInput :
 *=============================================================================
 *
 * On utilise `getchar()` pour demander à l'utilisateur de confirmer lors d'un
 * affichage, cependant, cette fonction ne capture que le premier caractère dans
 * `stdin`. De ce fait, les caractère restants dans stdin seront capturés (à la
 * place de la prochaine entrée) au prochain getchar.
 * IMPORTANT: `fflush` ne doit pas être utilisée pour flush stdin
 * SOURCE: https://en.cppreference.com/w/cpp/io/c/fflush
 *
 * Variables locales :
 *    - c : Caractère permettant de vider l'input
 *=============================================================================
 */

void flushInput() {

  char c;

  // Tant que l'on a pas récupéré tous les caractères restants en input
  while ((c = fgetc(stdin)) != EOF && c != '\n')
    ;
}

/*=============================================================================
 *                          Fonction motifCorrespond :
 *=============================================================================
 *
 * Cherche un motif dans le nom donné en procédant par morceaux
 *
 * En entrée :
 *    - motif : Chaîne de caractères contenant le motif à chercher
 *    - nom : Chaîne de caractères où chercher le motif
 *
 * Variables locales :
 *    - i : Indice permettant de parcourir la chaîne où chercher
 *    - tailleMotif : Entier représentant la taille du motif
 *    - r : Entier représentant le résultat de la recherche du motif
 *          S'il est à 0, le motif a été trouvé, sinon, il n'a pas été trouvé
 *
 * En sortie :
 *    - Retourne True si le motif apparaît dans le nom donné,
 *      False sinon
 *=============================================================================
 */

int motifCorrespond(char * motif, char nom[TAILLE_NOM]) {

  int	i	    = 0;
  int	tailleMotif = strlen(motif);
  int	r	    = -1;

  // On cherche le motif dans tous le mot tant qu'on ne l'a pas trouvé
  while (r && i + tailleMotif < TAILLE_NOM) {
    r = strncmp(motif, nom + i, tailleMotif);
    ++i;
  }
  return (r == 0);
}

/*****************************************************************************/
/* FONCTIONS DE SAISIE                                                       */
/*****************************************************************************/

/*=============================================================================
 *                         Fonction demandeInfos :
 *=============================================================================
 *
 * Demande les informations à l'utilisateur lors de la création d'un nouvel
 * élément. L'utilisateur doit saisir des informations valides.
 *
 * En entrée :
 *    - annee : Chaîne de caractères qui reçoit l'input de l'utilisateur
 *              correspondant à l'année
 *    - semaine : Chaîne de caractères recevant l'input de l'utilisateur
 *                correspondant à la semaine
 *    - jour : Pointeur sur un entier reçevant le nombre en input correspondant
 *             au jour
 *    - heure : Chaîne de caractères recevant l'input de l'utilisateur
 *              correspondant à l'heure de l'action
 *
 * Variables locales :
 *    - heure_tmp : Entier receuillant temporairement l'input direct de
 *                  l'utilisateur
 *    - semaine_tmp : Entier receuillant temporairement l'input direct de
 *                    l'utilisateur
 *=============================================================================
 */

void demandeInfos(char annee[TAILLE_ANNEE], char semaine[TAILLE_SEMAINE],
                  char heure[TAILLE_HEURE], int * jour) {

  int heure_tmp;
  int semaine_tmp;

  printf("Saisir annee:\n");
  scanf("%4s", annee);
  flushInput();

  // Tant que l'année saisie est inférieure à 2021
  while (atoi(annee) < 2021 && atoi(annee) > 9999) {
    printf("Veuillez saisir une année supérieure ou égale à 2021 :");
    scanf("%4s", annee);
    flushInput();
  }

  printf("Saisir semaine:\n");
  scanf("%d%*c", &semaine_tmp);

  // Tant que la semaine n'est pas comprise entre 1 et 53
  while (semaine_tmp < 1 || semaine_tmp > 53) {
    printf("Veuillez saisir une semaine entre 1 et 53 :\n");
    scanf("%d%*c", &semaine_tmp);
  }

  // Si la semaine est inférieure à 10, on ajoute un 0 manuellement
  if (semaine_tmp < 10) {
    semaine[0] = '0';
    semaine[1] = semaine_tmp + '0';
  }

  else {
    sprintf(semaine, "%d", semaine_tmp);
  }

  printf("Saisir jour:\n");
  scanf("%d%*c", jour);

  // Tant que le jour saisi n'est pas entre 1 et 7
  while (*jour < 1 || *jour > 7) {
    printf("Veuillez saisir un jour entre 1 (lundi) et 7(dimanche) :\n");
    scanf("%d%*c", jour);
  }

  printf("Saisir heure:\n");
  scanf("%d%*c", &heure_tmp);

  // Tant que l'heure saisie n'est pas entre 0 et 24
  while (heure_tmp < 0 || heure_tmp > 24) {
    printf("Veuillez saisir une heure entre 0 et 24 :\n");
    scanf("%d%*c", &heure_tmp);
  }

  // Si l'heure est inférieure à 10, on ajoute un 0 manuellement
  if (heure_tmp < 10) {
    heure[0] = '0';
    heure[1] = heure_tmp + '0';
  }

  else {
    sprintf(heure, "%d", heure_tmp);
  }
}

/*=============================================================================
 *                           Fonction demandeNom :
 *=============================================================================
 *
 * Permet à l'utilisateur de saisir le nom de l'activité qu'il souhaite
 * planifier.
 *
 * En entrée :
 *    - nom : Chaîne de caractères permettant de récupérer l'input
 *            de l'utilisateur sur 10 caractères
 *=============================================================================
 */

void demandeNom(char nom[TAILLE_NOM]) {
  
  int i = 0;

  printf("Saisir l'activité :\n");
  fgets(nom, TAILLE_NOM, stdin);

  // On supprime le '\n' si besoin
  while (nom[i] != 0 && nom[i] != '\n') {
    ++i;
  }
  if (nom[i] == '\n') {
    nom[i] = 0;
  } else {
    // si `nom[i] != 0` alors, il reste des caratère sur stdin (au moins un
    // '\n'), donc on les supprime
    flushInput();
  }
}

/*=============================================================================
 *                      Fonction demandeConfirmation :
 *=============================================================================
 *
 * Demande à l'utilisateur de confirmer la Suppression de l'agenda courrant
 * lors de l'import d'un nouveau fichier.
 *
 * Variable globale :
 *    - reponse : Caractère contenant la réponse de l'utilisateur
 *
 * En sortie :
 *    - Retourn True si l'utilisateur a répondu 'y', False sinon
 *=============================================================================
 */

char demandeConfirmation() {

  char reponse = 'n';

  printf("Cette action va effacer l'agenda courrant, confirmez-vous cette "
         "opération ? [y/n]\n");
  scanf("%c%*c", &reponse);

  return (reponse == 'y') ? 1 : 0;
}

/*****************************************************************************/
/* UI                                                                        */
/*****************************************************************************/

/*=============================================================================
 *                             Fonction afficheMenu :
 *=============================================================================
 *
 * Affiche le menu indiquant à l'utilisateur quoi tapper pour interagire avec
 * l'application.
 *=============================================================================
 */

void afficheMenu() {

  clear();
  printf("\n");
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
  printf("\t|                [5]: Recherche de motif               |\n");
  printf("\t|                [Autre]: Quitter                      |\n");
  printf("\t|                                                      |\n");
  printf("\t ------------------------------------------------------\n\n");
}

/*=============================================================================
 *                    Fonction afficheMessageFin :
 *=============================================================================
 *
 * Affiche message de fin quand l'utilisateur quite le programme
 *=============================================================================
 */

void afficheMessageFin() {

  clear();
  printf("\t ======================================================\n");
  printf("\t|                                                      |\n");
  printf("\t|                      AU REVOIR !                     |\n");
  printf("\t|                                                      |\n");
  printf("\t ======================================================\n");
}

/*=============================================================================
 *                             Fonction clear :
 *=============================================================================
 *
 * Fait un appel systeme à la fonction clear si le programme n'est pas compilé
 * en mode DEBUG
 *=============================================================================
 */

void clear() {

  // Si le programme n'est pas compilé en mode DEBUG
  if (DEBUG != 1) {
    system("clear");
  }
}
