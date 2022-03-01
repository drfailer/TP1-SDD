#ifndef __UTIL__
#define __UTIL__
#define DEBUG 0
#define MAX_ESPACE_CONTIGU 30
#define TAILLE_ANNEE 5
#define TAILLE_SEMAINE 3
#define TAILLE_NOM 11
#define TAILLE_HEURE 3
#define TAILLE_BUFFER                                                          \
  TAILLE_ANNEE + TAILLE_SEMAINE + TAILLE_HEURE + TAILLE_NOM - 1

/* FONCTIONS: */

char *parseSuiteNombres(char *s, char tab[], int n);
void flushInput();
int motifCorrespond(char *motif, char nom[TAILLE_NOM]);
void recupereLigne(char buff[], int n);
void demandeNom(char nom[TAILLE_NOM]);
void afficheMenu();
void afficheMessageFin();
void demandeInfos(char annee[TAILLE_ANNEE], char semaine[TAILLE_SEMAINE],
                  char heure[TAILLE_HEURE], int *jour);
char demandeConfirmation();
void clear();

#endif
