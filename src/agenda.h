/*             FICHIER AGENDA.H :
   Contient les définitions du type agenda_t
   ainsi que des fonctions qui s'y rapportent.
*/

#ifndef __AGENDA__
#define __AGENDA__
#include "tache.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

/* STRUCTURES: */

typedef struct agenda {
  char annee[TAILLE_ANNEE];
  char semaine[TAILLE_SEMAINE];
  tache_t *actions;
  struct agenda *suiv;
} agenda_t;

/* FONCTIONS: */

agenda_t * creeAgenda(char annee[TAILLE_ANNEE], char semaine[TAILLE_SEMAINE]);
void       freelst(agenda_t **agenda);
void       parseLigne(char *s, char annee[TAILLE_ANNEE], char semaine[TAILLE_SEMAINE],
                jour_t *jour, char heure[TAILLE_HEURE], char nom[TAILLE_NOM]);
agenda_t * ajouteAgenda(agenda_t *agenda, char annee[TAILLE_ANNEE],
                       char semaine[TAILLE_SEMAINE], jour_t jour, char heure[TAILLE_HEURE],
                       char nom[TAILLE_NOM]);
char **    listeMotif(agenda_t *agenda, char **deb, char *motif);
void       creerListeContigue(agenda_t *agenda, char ***deb, char ***fin,
                        char *motif);
agenda_t * agendaViafichier(char *nom);
void       ecritFichier(FILE *f, agenda_t *agenda);
void       sauvFichier(char *nom, agenda_t *agenda);
agenda_t * supprimeElt(agenda_t *agenda, char annee[TAILLE_ANNEE],
                      char semaine[TAILLE_SEMAINE], jour_t jour, char
		      heure[TAILLE_HEURE]);
int        compAgendaElt(agenda_t *elt, char annee[TAILLE_ANNEE],
                  char semaine[TAILLE_SEMAINE]);
void       agendaToString(agenda_t *agendaElt, char buff[]);
void       afficheAgendaElt(agenda_t *agendaElt);
void       afficheAgenda(agenda_t *agenda);

#endif
