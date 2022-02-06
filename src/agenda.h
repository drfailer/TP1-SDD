#ifndef __AGENDA__
#define __AGENDA__
#include "tache.h"

/* STRUCTURES: */

typedef struct agenda {
  unsigned short int  annee[4];
  unsigned short int semaine[2];
  tache_t *actions;
  struct agenda *suiv;
} agenda_t;


/* FONCTIONS: */

agenda_t *creeAgenda(unsigned short int annee[4], unsigned short int semaine[2]);
int compAgendaElt(agenda_t *elt, unsigned short int annee[4], unsigned short int semaine[2]);
void afficheAgendaElt(agenda_t *agendaElt);
agenda_t *agendaViafichier(char *nom);
void afficheAgenda(agenda_t *agenda);
void freelst(agenda_t **agenda);

#endif
