#ifndef __TACHE__
#define __TACHE__

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
  char heure[3];
  char nom[10];
  struct tache *suiv;
} tache_t;


tache_t *creeTache(char j, char heure[3], char nom[10]);
int compTache(tache_t *elt, char j, char heure[3]);
tache_t *ajouteTache(tache_t *tache, jour_t jour, char heure[3], char nom[10]);
void freeTache(tache_t **tache);

#endif
