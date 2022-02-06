#include <stdlib.h>
#include "tache.h"
#include "util.h"


/*****************************************************************************/
/* CONSTRUCTEUR ET DESTRUCTEUR                                               */
/*****************************************************************************/

/* allocation dynamique et initialisation des champs d'un nouvelle tache_t */
tache_t *creeTache(unsigned short int j, unsigned short int heure[2], char nom[10]) {
  tache_t *tache = (tache_t *)malloc(sizeof(tache_t));
  if (tache != NULL) {
    tache->j = j;
    cpyTab(tache->heure, heure, 2);
    cpyCharTab(tache->nom, nom, 10);
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
tache_t *ajouteTache(tache_t *tache, char *s) {
  int r = -1;
  tache_t **prec = &tache;
  tache_t *tmp;
  unsigned short int j = *s - '0';
  unsigned short int heure[2];

  s = parseSuiteNombres(s+1, heure, 2);
  while (*prec != NULL && r < 0) {
    r = compTache(*prec, j, heure);
    if (r < 0)
      prec = &(*prec)->suiv;
  }
  if (r != 0) {
    tmp = creeTache(j, heure, s);
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
int compTache(tache_t *elt, unsigned short int j, unsigned short int heure[2]) {
  int resultat = 0;
  int i = 0;
  resultat = elt->j - j;
  while (resultat == 0 && i < 2) {
    resultat = elt->heure[i] - heure[i];
    ++i;
  }
  return resultat;
}


