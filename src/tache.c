/*                FICHIER TACHE.C :
 *
 * Contient les fonctions relatives au type tache_t
 */

#include "tache.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************/
/* CONSTRUCTEUR ET DESTRUCTEUR                                               */
/*****************************************************************************/

/*=============================================================================
 *                             Fonction creeTache :
 *=============================================================================
 *
 * Allocation dynamique et initialisation des champs d'un nouvelle tache_t
 *
 * En entrée :
 *    - j : Caractère contenant un entier représentant le jour de la tâche
 *          à créer
 *    - heure : Chaîne de caractères contenant l'heure de la tâche à créer
 *
 * Variables locales :
 *    - tache : Adresse indirecte de la tâche nouvellement créée
 *
 * En sortie :
 *    - tache : Adresse indirecte de la tâche nouvellement créée et initialisée
 *=============================================================================
 */

tache_t * creeTache(char j, char heure[TAILLE_HEURE], char nom[TAILLE_NOM]) {

  tache_t * tache = (tache_t *)malloc(sizeof(tache_t));

  // Si l'allocation a bien eu lieu
  if (tache != NULL) {
    tache->j = j;
    strcpy(tache->heure, heure);
    strcpy(tache->nom, nom);
    tache->suiv = NULL;
  }

  return tache;
}

/*=============================================================================
 *                            Fonction freeTache :
 *=============================================================================
 *
 * Libère la mémoire allouée pour une liste de tache_t
 *
 * En entrée :
 *    - tache : Pointeur sur la tête fictive de la liste de tâches à
 *              libérer
 *
 * Variables locales :
 *    - cour : Adresse indirecte de la tâche courante dans le parcours
 *             de la liste
 *    - suiv : Adresse indirecte de la tâche suivante dans le parcours
 *             de la liste
 *=============================================================================
 */
void freeTache(tache_t ** tache) {

  tache_t * cour;
  tache_t * suiv;

  // Si la liste de tâche n'est pas vide
  if (tache != NULL) {
    cour = *tache;

    // Tant que l'on est pas à la fin de la liste
    while (cour) {
      suiv = cour->suiv;
      free(cour);
      cour = suiv;
    }
    *tache = NULL;
  }
}

/*****************************************************************************/
/* FONCTIONS DE MANIPULATION DE LA LISTE                                     */
/*****************************************************************************/

/*=============================================================================
 *                           Fonction ajouteTache :
 *=============================================================================
 *
 * Ajout d'un élément dans la liste triée qui contient les actions à réaliser
 * pour un élément de l'agenda (liste principale). Ne l'ajoute que s'il
 * n'existe pas déjà une action au même jour et à la même heure dans la liste.
 *
 * En entrée :
 *    - tache : Tête fictive de la liste de tâches où ajouter l'élément
 *    - jour : Enumération représentant le jour de l'action à ajouter
 *    - heure : Chaîne de caractères contenant l'heure de l'action à ajouter
 *    - nom : Chaîne de caratcères contenant le nom de l'action à ajouter
 *
 * Variables locales :
 *    - r : Entier réprésentant le résultat des comparaisons
 *    - prec : Pointeur sur l'adresse indirecte de l'élément précédant
 *             celui considéré dans le parcours des tâches
 *    - tmp : tâche créée avec les informations données en paramètre
 *
 * En sortie :
 *    - tache : Tête fictive de la liste de tâches modifiée
 *=============================================================================
 */

tache_t *ajouteTache(tache_t * tache, jour_t jour, char heure[TAILLE_HEURE],
                     char nom[TAILLE_NOM]) {

  int		   r    = -1;
  tache_t	** prec = &tache;
  tache_t	 * tmp;

  /* Tant que l'on a pas parcouru toute la liste et que
     les informations de l'élément sont plus petites que
     celles données en paramètre */
  while (*prec != NULL && r < 0) {
    r = compTache(*prec, jour, heure);

    /* Si les informations de l'élément sont plus petites que
       celles données en paramètre */
    if (r < 0)
      prec = &(*prec)->suiv;
  }
  /* S'il n'existe pas déjà une action le meme jour à la même heure */
  if (r != 0) {
    tmp = creeTache(jour, heure, nom);

    // Si l'élément a pu être créé
    if (tmp != NULL) {
      tmp->suiv = *prec;
      *prec = tmp;
    }
  }
  return tache;
}

/*=============================================================================
 *                         Fonction supprimeTache :
 *=============================================================================
 *
 * Supprime la tache programmée pour `jour` à `heure` dans une liste de taches
 *
 * En entrée :
 *    - tache : Tête fictive de la liste de tâches où supprimer l'élément
 *    - jour : Enumération représentant le jour de l'action à supprimer
 *    - heure : Chaîne de caractères contenant l'heure de l'action à supprimer
 *
 * Variables locales :
 *    - r : Entier réprésentant le résultat des comparaisons
 *    - prec : Pointeur sur l'adresse indirecte de l'élément précédant
 *             celui considéré dans le parcours des tâches
 *    - tmp : tâche temporaire permettant la libération de l'élément
 *
 * En sortie :
 *    - tache : Tête fictive de la liste de tâches modifiée
 *=============================================================================
 */
tache_t * supprimeTache(tache_t * tache, jour_t jour, char heure[TAILLE_HEURE]) {

  int		   r    = -1;
  tache_t	** prec = &tache;
  tache_t	 * tmp;

  /* Tant que l'on a pas parcouru toute la liste et que
     les informations de l'élément sont plus petites que
     celles données en paramètre */
  while (*prec != NULL && r < 0) {
    r = compTache(*prec, jour, heure);

    /* Si les informations de l'élément sont plus petites que
       celles données en paramètre */
    if (r < 0)
      prec = &(*prec)->suiv;
  }

  // Si la tâche à supprimer existe
  if (r == 0) {
    tmp = *prec;
    *prec = (*prec)->suiv;
    free(tmp);
  }
  return tache;
}

/*=============================================================================
 *                              Fonction compTache :
 *=============================================================================
 *
 * Compare une tache à un jour et une heure (utilisé pour insertion dans liste
 * triée)
 *
 * En entrée :
 *    - elt : Adresse indirecte de l'élément à comparer
 *    - j : Caractère représentant le jour sur lequel faire la
 *          comparaison
 *    - heure : Chaîne de caractères contenant l'heure sur laquelle
 *              faire la comparaison
 *
 * Variables locales :
 *    - resultat : Entier contenant le résultat de la comparaison
 *    - i : Entier permettant le parcours manuel de l'heure
 *
 * En sortie :
 *    - resultat : Entier représentant le résultat de la comparaison.
 *                 Contient 0 si le jour et l'heure de l'élément et
 *		   ceux renseignés en paramètre sont égaux
 *		   Contient une valeur positive si :
 *		      - Le jour de l'élément est supérieur au jour en
 *		        paramètre
 *		      - Les deux jours sont égaux mais l'heure de
 *		        l'élément est supérieure à l'heure en paramètre
 *		   Contient une valeur négative si :
 *		      - Le jour de l'élément est inférieur au jour en
 *		        paramètre
 *		      - Les deux jours sont égaux mais l'heure de
 *		        l'élément est inférieure à l'heure en paramètre
 *=============================================================================
 */

int compTache(tache_t * elt, char j, char heure[TAILLE_HEURE]) {

  int	resultat = 0;
  int	i	 = 0;

  resultat = elt->j - j;

  /* Tant que les jours sont égaux et que les chiffres
     de chaque heure sont égaux et que l'on a pas vérifié
     les deux chiffres de l'heure */
  while (resultat == 0 && i < 2) {
    resultat = elt->heure[i] - heure[i];
    ++i;
  }

  return resultat;
}

/*=============================================================================
 *                        Fonction tacheToString :
 *=============================================================================
 *
 * Crée une chaîne de caractère correspondant aux information de
 * la tâche en paramètre sous la forme "jourheurenom".
 *
 * En entrée :
 *    - tache : Pointeur sur la tâche à convertir en chaîne de caractères
 *    - buff : Chaîne de caractère recevant la transformation de la
 *             tâche en chaîne de caractères
 * Variable globale :
 *    - i : indice de parcours de la chaîne de caractères
 *=============================================================================
 */

void tacheToString(tache_t * tache, char buff[]) {

  int i = 0;

  buff[TAILLE_ANNEE + TAILLE_SEMAINE - 2] = tache->j + '0';
  buff[TAILLE_ANNEE + TAILLE_SEMAINE - 1] = 0;
  strcat(buff, tache->heure);
  strncat(buff, tache->nom, TAILLE_NOM);

  // Si le nom ne contient pas dix caractères
  if (strlen(buff) != TAILLE_BUFFER - 1) {

    // On ajoute des espaces pour compléter
    for (i = strlen(buff); i < TAILLE_BUFFER; ++i) {
      buff[i] = ' ';
    }
    
    buff[i] = 0;
  }
  
  buff[TAILLE_BUFFER - 1] = 0;
}

/*****************************************************************************/
/* FONCTION D'AFFICHAGE                                                      */
/*****************************************************************************/

/*=============================================================================
 *                           Fonction afficheTache :
 *=============================================================================
 *
 * Affiche la l'ensemble des tâches d'une liste chaînée donnée.
 *
 * En entrée :
 *    - taches : Tête fictive de la liste de tâche à afficher
 *
 * Variables locales :
 *    - cour : Pointeur sur l'élément considéré dans le parcours
 *             de la liste
 *=============================================================================
 */

void afficheTache(tache_t * taches) {

  tache_t * cour = taches;

  // Tant que l'on a pas atteint la fin de la liste
  while (cour != NULL) {
    printf("|\t|  jour: %d\n", cour->j);
    printf("|\t|  heure: %s\n", cour->heure);
    printf("|\t|_ nom: %s", cour->nom);
    cour = cour->suiv;

    // Si l'on est pas à la fin de la liste
    if (cour != NULL)
      printf("\n|\t|\n");
  }
}

/*=============================================================================
 *                    Fonction afficheListeContigue :
 *=============================================================================
 * Affiche une liste contigue de `tache_t*`
 *
 * En entrée :
 *    - deb : Pointeur sur le début de la liste
 *    - fin : Pointeur sur la fin de la liste
 *=============================================================================
 */

void afficheListeContigue(char ** deb, char ** fin) {

  // Tant que l'on a pas parcouru toute la liste
  while (deb != fin) {
    printf("%s\n", *deb);
    ++deb;
  }
}
