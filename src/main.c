#include <stdio.h>
#include <string.h>
#include "util.h"

char *parseAnnee(char *s, agenda_t *agendaElt) {
  int i;
  for (i = 0; *s && i < 4; ++i, ++s) {
    agendaElt->annee[i] = *s - '0';
  }
  return s;
}

char *parseSemaine(char *s, agenda_t *agendaElt) {
  int i;
  for (i = 0; *s && i < 2; ++i, ++s) {
    agendaElt->semaine[i] = *s - '0';
  }
  return s;
}

char *parseHeure(char *s, tache_t *tache) {
  int i;
  for (i = 0; *s && i < 2; ++i, ++s) {
    tache->heure[i] = *s - '0';
  }
  return s;
}


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

void afficheAgendaElt(agenda_t *agendaElt) {
  int i;

  printf("annee: ");
  for (i = 0; i < 4; ++i)
    printf("%d", agendaElt->annee[i]);
  printf("\n");
  for (i = 0; i < 2; ++i)
    printf("semaine = %d\n", agendaElt->semaine[i]);
  printf("jour: %d\n", agendaElt->actions->j);
  for (i = 0; i < 2; ++i)
    printf("heure: %d\n", agendaElt->actions->heure[i]);
  printf("nom: ");
  for (i = 0; i < 10; ++i)
    printf("%c", agendaElt->actions->nom[i]);
  printf("\n");
}

int compAgendaElt(agenda_t *elt1, agenda_t *elt2) {
  int resultat = 0;
  return resultat;
}

int main (int argc, char** argv)
{
  char *test = (char*) malloc(50*sizeof(char));
  agenda_t *agendaElt;
  if (test != NULL) {
    strcpy(test, "202215108TPs de SDD");
    agendaElt = parseLigne(test);
    afficheAgendaElt(agendaElt);
    free(test);
    free(agendaElt);
  } else {
    fprintf(stderr, "NULL\n");
  }
  return 0;
}
