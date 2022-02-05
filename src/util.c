#include "util.h"


/*****************************************************************************/
/* FONCTIONS SECONDAIRES                                                     */
/*****************************************************************************/

void cpyTab(unsigned short int *dest, unsigned short int *src, int n) {
  int i;
  for (i = 0; i < n; ++i) {
    dest[i] = src[i];
  }
}

void cpyCharTab(char *dest, char *src, int n) {
  int i;
  for (i = 0; i < n; ++i) {
    dest[i] = src[i];
  }
}

/* Constructeurs */

agenda_t *creeAgenda(unsigned short int annee[4], unsigned short int semaine[2]) {/*{{{*/
  agenda_t *agenda =  (agenda_t *)malloc(sizeof(agenda_t));
  if (agenda != NULL) {
    cpyTab(agenda->annee, annee, 4);
    cpyTab(agenda->semaine, semaine, 2);
    agenda->suiv = NULL;
    agenda->actions = NULL;
  }
  return agenda;
}/*}}}*/

tache_t *creeTache(unsigned short int j, unsigned short int heure[2], char nom[10]) {/*{{{*/
  tache_t *tache = (tache_t *)malloc(sizeof(tache_t));
  if (tache != NULL) {
    tache->j = j;
    cpyTab(tache->heure, heure, 2);
    cpyCharTab(tache->nom, nom, 10);
    tache->suiv = NULL;
  }
  return tache;
}/*}}}*/

/* fonctions de comparaisons */

int compAgendaElt(agenda_t *elt, unsigned short int annee[4], unsigned short int semaine[2]) {
  int resultat = 0;
  int i = 0;

  // compare l'annee:
  while (resultat == 0 && i < 4) {
    resultat = elt->annee[i] - annee[i];
    ++i;
  }
  i = 0;
  // compare la semaine
  while (resultat == 0 && i < 2) {
    resultat = elt->semaine[i] - semaine[i];
    ++i;
  }
  return resultat;
}

int compTache(tache_t *elt, unsigned short int j, unsigned short int heure[2]) {
  int resultat = 0;
  int i = 0;
  resultat = elt->j - j;
  while (resultat == 0 && i < 2) {
    resultat = elt->heure[i] - heure[i];
    ++i;
  }
  return resultat;
}


/* Transformation d'une chaine de caractère en `agenda_t` */

/* récupère une suite de `n` nombres et les met dans tab
 * retourne le pointeur `s`
 */
char *parseSuiteNombres(char *s, unsigned short int tab[], int n) {
  int i;
  for (i = 0; *s && i < n; ++i, ++s) {
    tab[i] = *s - '0';
  }
  return s;
}

/* Permet de récupérer l'année sur une ligne */
static inline char *parseAnnee(char *s, agenda_t *agendaElt) {
  return parseSuiteNombres(s, agendaElt->annee, 4);
}


/* Permet de récupérer la semaine sur une ligne */
static inline char *parseSemaine(char *s, agenda_t *agendaElt) {
  return parseSuiteNombres(s, agendaElt->semaine, 2);
}

/* Permet de récupérer l'heure sur une ligne */
static inline char *parseHeure(char *s, tache_t *tache) {
  return parseSuiteNombres(s, tache->heure, 2);
}

/* Récupère les donnée présententes dans une chaine de caractères au format:
 * AAAASSJHHNom tache_ (ou _ est un espace qui complète la ligne pour qu'elle
 * face 10 caractères)
 */
agenda_t *parseLigne(char *ligne) {
  agenda_t *agendaElt = (agenda_t*) malloc(sizeof(agenda_t));
  tache_t *tache = NULL;
  int i = 0;
  ligne = parseAnnee(ligne, agendaElt);
  ligne = parseSemaine(ligne, agendaElt);
  tache = (tache_t*) malloc(sizeof(tache_t));
  if (tache != NULL) {
    tache->j = *ligne - '0';
    ++ligne;
    ligne = parseHeure(ligne, tache);
    for (i = 0; i < 10; ++i)
      tache->nom[i] = ligne[i];
  }
  agendaElt->actions = tache;
  return agendaElt;
}

/* affiche un agenda_t */
void afficheAgendaElt(agenda_t *agendaElt) {
  int i;

  printf("annee: ");
  for (i = 0; i < 4; ++i)
    printf("%d", agendaElt->annee[i]);
  printf("\nsemaine: ");
  for (i = 0; i < 2; ++i)
    printf("%d", agendaElt->semaine[i]);
  printf("\njour: %d\n", agendaElt->actions->j);
  printf("heure: ");
  for (i = 0; i < 2; ++i)
    printf("%d", agendaElt->actions->heure[i]);
  printf("\nnom: ");
  for (i = 0; i < 10; ++i)
    printf("%c", agendaElt->actions->nom[i]);
  printf("\n");
}

/* Ajout d'élément dans la liste triée */

tache_t *ajouteTache(tache_t *tache, char *s) {
  int r = -1;
  tache_t **prec = &tache;
  tache_t *tmp;
  unsigned short int j = *s - '0';
  unsigned short int heure[2];

  s = parseSuiteNombres(s+1, heure, 2);
  while (*prec != NULL && r < 0) {
    r = compTache(*prec, j, heure);
    if (r < 0)
      prec = &(*prec)->suiv;
  }
  if (r != 0) {
    tmp = creeTache(j, heure, s);
    if (tmp != NULL) {
      tmp->suiv = *prec;
      *prec = tmp;
    }
  }
  return tache;
}

agenda_t *ajouteAgenda(agenda_t *agenda, char *s) {
  int r = -1;
  agenda_t **prec = &agenda;
  agenda_t *tmp;
  unsigned short int annee[4];
  unsigned short int semaine[2];

  s = parseSuiteNombres(s, annee, 4);
  s = parseSuiteNombres(s, semaine, 2);
  while (*prec != NULL && r < 0) {
    r = compAgendaElt(*prec, annee, semaine);
    if (r < 0)
      prec = &(*prec)->suiv;
  }
  if (r == 0) {
    (*prec)->actions = ajouteTache((*prec)->actions, s);
  } else {
    tmp = creeAgenda(annee, semaine);
    if (tmp != NULL) {
      tmp->suiv = *prec;
      *prec = tmp;
    }
    tmp->actions = ajouteTache(tmp->actions, s);
  }
  return agenda;
}


/*****************************************************************************/
/* FONCTIONS DU TP                                                           */
/*****************************************************************************/

/* récup fichier text */

agenda_t *agendaViafichier(char *nom) {
  FILE *f = fopen(nom, "r");
  agenda_t *agenda = NULL;
  char buff[21];

  if (f != NULL) {
    while (!feof(f)) {
      fgets(buff, 21, f);
      agenda = ajouteAgenda(agenda, buff);
    }
    fclose(f);
  } else {
    fprintf(stderr, "ERREUR: ouverture de fichier.");
  }
  return agenda;
}

void freeTache(tache_t **tache) {
  tache_t *cour;
  tache_t *suiv;
  if (tache != NULL) {
    cour = *tache;
    while (cour) {
      suiv = cour->suiv;
      free(cour);
      cour = suiv;
    }
    *tache = NULL;
  }
}

/* libère une liste chainée *//*{{{*/
void freelst(agenda_t **agenda) {
  agenda_t *cour;
  agenda_t *suiv;
  if (agenda != NULL) {
    cour = *agenda;
    while (cour) {
      suiv = cour->suiv;
      freeTache(&cour->actions);
      free(cour);
      cour = suiv;
    }
    *agenda = NULL;
  }
}/*}}}*/

/* affiche tous les éléments présents dans l'agenda *//*{{{*/
void afficheAgenda(agenda_t *agenda) {
  int i = 0;
  agenda_t *cour = agenda;

  while (cour != NULL) {
    printf("element %d:\n", i);
    afficheAgendaElt(cour);
    cour = cour->suiv;
    ++i;
  }
}/*}}}*/

/* save fichier text */
/* suppression d'action */
