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

agenda_t *creeAgenda(char annee[TAILLE_ANNEE], char semaine[TAILLE_SEMAINE]);
int compAgendaElt(agenda_t *elt, char annee[TAILLE_ANNEE],
                  char semaine[TAILLE_SEMAINE]);
void afficheAgendaElt(agenda_t *agendaElt);
agenda_t *agendaViafichier(char *nom);
agenda_t *ajouteAgenda(agenda_t *agenda, char annee[TAILLE_ANNEE],
                       char semaine[TAILLE_SEMAINE], jour_t jour, char heure[TAILLE_HEURE],
                       char nom[TAILLE_NOM]);
void sauvFichier(char *nom, agenda_t *agenda);
void ecritFichier(FILE *f, agenda_t *agenda);
void agendaToString(agenda_t *agendaElt, char buff[]);
void afficheAgenda(agenda_t *agenda);
void freelst(agenda_t **agenda);
tache_t *supprimeTache(tache_t *tache, jour_t jour, char heure[3]);
agenda_t *supprimeElt(agenda_t *agenda, char annee[TAILLE_ANNEE],
                      char semaine[TAILLE_SEMAINE], jour_t jour, char heure[TAILLE_HEURE]);

#endif
