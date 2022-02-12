#ifndef __AGENDA__
#define __AGENDA__
#include "tache.h"
#include <stdlib.h>
#include <stdio.h>

/* STRUCTURES: */

typedef struct agenda {
  char annee[5];
  char semaine[3];
  tache_t *actions;
  struct agenda *suiv;
} agenda_t;


/* FONCTIONS: */

agenda_t *creeAgenda(char annee[5], char semaine[3]);
int compAgendaElt(agenda_t *elt, char annee[5], char semaine[3]);
void afficheAgendaElt(agenda_t *agendaElt);
agenda_t *agendaViafichier(char *nom);
void ecritFichier(FILE *f, agenda_t *agenda);
void sauvFichier(char *nom, agenda_t *agenda);
void afficheAgenda(agenda_t *agenda);
void freelst(agenda_t **agenda);

#endif
