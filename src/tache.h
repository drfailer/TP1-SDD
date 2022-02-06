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
  unsigned short int heure[2];
  char nom[10];
  struct tache *suiv;
} tache_t;


tache_t *creeTache(unsigned short int j, unsigned short int heure[2], char nom[10]);
tache_t *ajouteTache(tache_t *tache, char *s);
int compTache(tache_t *elt, unsigned short int j, unsigned short int heure[2]);
void freeTache(tache_t **tache);

#endif
