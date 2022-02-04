#ifndef __UTIL__
#define __UTIL__
#include <stdlib.h>


/* STRUCTURES: */

typedef enum {
  LUNDI = 1,
  MARDI,
  MERCREDI,
  JEUDI,
  VENDREDI,
  SAMEDI,
  DIMANCHE
} jour_t;

typedef struct tache {
  jour_t j;
  unsigned short int heure[2];
  char nom[10];
  struct tache *suiv;
} tache_t;


typedef struct agenda {
  unsigned short int  annee[4];
  unsigned short int semaine[2];
  tache_t *actions;
  struct agenda *suiv;
} agenda_t;

#endif
