/*             FICHIER TACHE.H :
 *
 * Contient les déclarations du type jour_t,
 * du type tache_t ainsi que des fonctions se
 * rapportant au type tache_t
 */

#ifndef __TACHE__
#define __TACHE__
#include "util.h"

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
  char heure[TAILLE_HEURE];
  char nom[TAILLE_NOM];
  struct tache *suiv;
} tache_t;


tache_t * creeTache(char j, char heure[TAILLE_HEURE], char nom[TAILLE_NOM]);
void      freeTache(tache_t **tache);
tache_t * ajouteTache(tache_t *tache, jour_t jour, char heure[TAILLE_HEURE], char nom[TAILLE_NOM]);
tache_t * supprimeTache(tache_t *tache, jour_t jour, char heure[TAILLE_HEURE]);
int       compTache(tache_t *elt, char j, char heure[TAILLE_HEURE]);
void      tacheToString(tache_t *tache, char buff[]);
void      afficheTache(tache_t *taches);
void      afficheListeContigue(char **deb, char **fin);



#endif
