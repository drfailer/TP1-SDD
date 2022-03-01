#include "tache.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************/
/* CONSTRUCTEUR ET DESTRUCTEUR                                               */
/*****************************************************************************/

/* allocation dynamique et initialisation des champs d'un nouvelle tache_t */
tache_t *creeTache(char j, char heure[TAILLE_HEURE], char nom[TAILLE_NOM]) {
  tache_t *tache = (tache_t *)malloc(sizeof(tache_t));
  if (tache != NULL) {
    tache->j = j;
    strcpy(tache->heure, heure);
    strcpy(tache->nom, nom);
    tache->suiv = NULL;
  }
  return tache;
}

/* Libère la mémoire allouée pour une liste de tache_t */
void freeTache(tache_t **tache) {
  tache_t *cour;
  tache_t *suiv;
  if (tache != NULL) {
    cour = *tache;
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

/* Ajout un élément dans la liste triée qui contient les actions à réaliser
 * pour un élément de l'agenda (liste principale)
 */
tache_t *ajouteTache(tache_t *tache, jour_t jour, char heure[TAILLE_HEURE],
                     char nom[TAILLE_NOM]) {
  int r = -1;
  tache_t **prec = &tache;
  tache_t *tmp;

  while (*prec != NULL && r < 0) {
    r = compTache(*prec, jour, heure);
    if (r < 0)
      prec = &(*prec)->suiv;
  }
  /* si on a pas déjà une action le meme jour à la même heure */
  if (r != 0) {
    tmp = creeTache(jour, heure, nom);
    if (tmp != NULL) {
      tmp->suiv = *prec;
      *prec = tmp;
    }
  }
  return tache;
}

/* supprime la tache programmée pour `jour` à `heure` dans une liste de taches
 */
tache_t *supprimeTache(tache_t *tache, jour_t jour, char heure[TAILLE_HEURE]) {
  int r = -1;
  tache_t **prec = &tache;
  tache_t *tmp;

  while (*prec != NULL && r < 0) {
    r = compTache(*prec, jour, heure);
    if (r < 0)
      prec = &(*prec)->suiv;
  }
  if (r == 0) {
    tmp = *prec;
    *prec = (*prec)->suiv;
    free(tmp);
  }
  return tache;
}

/*****************************************************************************/
/* FONCTIONS DE DIVERSES                                                     */
/*****************************************************************************/

/* compare une tache à un jour et une heure (utilisé pour insertion dans liste
 * triée)
 */
int compTache(tache_t *elt, char j, char heure[TAILLE_HEURE]) {
  int resultat = 0;
  int i = 0;
  resultat = elt->j - j;
  while (resultat == 0 && i < 2) {
    resultat = elt->heure[i] - heure[i];
    ++i;
  }
  return resultat;
}

// TODO: move this in `tache.c`
void tacheToString(tache_t *tache, char buff[]) {
  buff[TAILLE_ANNEE + TAILLE_SEMAINE - 2] = tache->j + '0';
  buff[TAILLE_ANNEE + TAILLE_SEMAINE - 1] = 0;
  strcat(buff, tache->heure);
  strncat(buff, tache->nom, TAILLE_NOM);
  buff[TAILLE_BUFFER - 1] = 0;
}

// TODO: move this in `tache.c`
void afficheTache(tache_t *taches) {
  tache_t *cour = taches;

  while (cour != NULL) {
    printf("|\t|  jour: %d\n", cour->j);
    printf("|\t|  heure: %s\n", cour->heure);
    printf("|\t|_ nom: %s", cour->nom);
    cour = cour->suiv;
    if (cour != NULL)
      printf("\n|\t|\n");
  }
}

