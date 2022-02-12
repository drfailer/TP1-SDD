#include <stdlib.h>
#include <string.h>
#include "tache.h"
#include "util.h"


/*****************************************************************************/
/* CONSTRUCTEUR ET DESTRUCTEUR                                               */
/*****************************************************************************/

/* allocation dynamique et initialisation des champs d'un nouvelle tache_t */
tache_t *creeTache(char j, char heure[3], char nom[10]) {
  tache_t *tache = (tache_t *)malloc(sizeof(tache_t));
  if (tache != NULL) {
    tache->j = j;
    strcpy(tache->heure, heure);
    cpyTab(tache->nom, nom, 10);
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
tache_t *ajouteTache(tache_t *tache, jour_t jour, char heure[3], char nom[10]) {
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

/*****************************************************************************/
/* FONCTIONS DE DIVERSES                                                     */
/*****************************************************************************/

/* compare une tache à un jour et une heure (utilisé pour insertion dans liste
 * triée)
 */
int compTache(tache_t *elt, char j, char heure[3]) {
  int resultat = 0;
  int i = 0;
  resultat = elt->j - j;
  while (resultat == 0 && i < 2) {
    resultat = elt->heure[i] - heure[i];
    ++i;
  }
  return resultat;
}


