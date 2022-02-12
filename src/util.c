#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "agenda.h"
#include "util.h"


/*****************************************************************************/
/* FONCTIONS SECONDAIRES                                                     */
/*****************************************************************************/

/**
 * TODO: trouver une meilleur alternative à ces fonctions
 */

void cpyTab(char *dest, char *src, int n) {
  int i;
  for (i = 0; i < n; ++i) {
    dest[i] = src[i];
  }
}

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

/* Récupère les donnée présententes dans une chaine de caractères au format:
 * AAAASSJHHNom tache_ (ou _ est un espace qui complète la ligne pour qu'elle
 * face 10 caractères)
 */
/* agenda_t *parseLigne(char *ligne) { */
/*   agenda_t *agendaElt = (agenda_t*) malloc(sizeof(agenda_t)); */
/*   tache_t *tache = NULL; */
/*   int i = 0; */
/*   ligne = parseAnnee(ligne, agendaElt); */
/*   ligne = parseSemaine(ligne, agendaElt); */
/*   tache = (tache_t*) malloc(sizeof(tache_t)); */
/*   if (tache != NULL) { */
/*     tache->j = *ligne - '0'; */
/*     ++ligne; */
/*     ligne = parseHeure(ligne, tache); */
/*     for (i = 0; i < 10; ++i) */
/*       tache->nom[i] = ligne[i]; */
/*   } */
/*   agendaElt->actions = tache; */
/*   return agendaElt; */
/* } */





/* save fichier text */
/* suppression d'action */
