#include "agenda.h"
#include "tache.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************/
/* CONSTRUCTEUR ET DESTRUCTEUR                                               */
/*****************************************************************************/

/* allocation dynamique et initialisation des champs d'un nouvel agenda_t */
agenda_t *creeAgenda(char annee[5], char semaine[3]) {
  agenda_t *agenda = (agenda_t *)malloc(sizeof(agenda_t));
  if (agenda != NULL) {
    stpcpy(agenda->annee, annee);
    stpcpy(agenda->semaine, semaine);
    agenda->suiv = NULL;
    agenda->actions = NULL;
  }
  return agenda;
}

/* libère la liste qui contient l'agenda
 * NOTE: fait appel à la fonction `freeTache` définie dans `tache.c`
 */
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
}

/*****************************************************************************/
/* FONCTIONS DE MANIPULATION DE LA LISTE                                     */
/*****************************************************************************/

void parseLigne(char *s, char annee[5], char semaine[3], jour_t *jour,
                char heure[3], char nom[10]) {
  s = parseSuiteNombres(s, annee, 4);
  annee[4] = 0;
  s = parseSuiteNombres(s, semaine, 2);
  semaine[2] = 0;
  *jour = *s - '0';
  s = parseSuiteNombres(s + 1, heure, 2);
  heure[2] = 0;
  cpyTab(nom, s, 10);
}

/* Ajoute un élément dans la liste triée qui contient les éléments de l'agenda
 */
agenda_t *ajouteAgenda(agenda_t *agenda, char annee[5], char semaine[3],
                       jour_t jour, char heure[3], char nom[10]) {
  int r = -1;
  agenda_t **prec = &agenda;
  agenda_t *tmp;

  while (*prec != NULL && r < 0) {
    r = compAgendaElt(*prec, annee, semaine);
    if (r < 0)
      prec = &(*prec)->suiv;
  }
  if (r == 0) {
    (*prec)->actions = ajouteTache((*prec)->actions, jour, heure, nom);
  } else {
    tmp = creeAgenda(annee, semaine);
    if (tmp != NULL) {
      tmp->suiv = *prec;
      *prec = tmp;
    }
    tmp->actions = ajouteTache(tmp->actions, jour, heure, nom);
  }
  return agenda;
}

/*****************************************************************************/
/* FONCTIONS DU TP                                                           */
/*****************************************************************************/

/* Créer un agenda à partir des données stoquées dans le fichier `nom`
 * NOTE: format des donnée
 * AAAASSJHHNom tache_
 * (où _ est un espace qui sert à completer le nom à 10 caractères)
 */
agenda_t *agendaViafichier(char *nom) {
  FILE *f = fopen(nom, "r");
  agenda_t *agenda = NULL;
  char buff[30];
  char nomAction[10];
  char annee[5];
  char semaine[3];
  jour_t jour;
  char heure[3];

  if (f != NULL) {
    while (!feof(f)) {
      fgets(buff, 30, f);
      parseLigne(buff, annee, semaine, &jour, heure, nomAction);
      agenda = ajouteAgenda(agenda, annee, semaine, jour, heure, nomAction);
    }
    fclose(f);
  } else {
    fprintf(stderr, "ERREUR: ouverture de fichier.");
  }
  return agenda;
}

/*****************************************************************************/
/* FONCTIONS DE DIVERSES                                                     */
/*****************************************************************************/

tache_t *supprimeTache(tache_t *tache, jour_t jour, char heure[3]) {
  int r = -1;
  tache_t **prec = &tache;
  tache_t *tmp;

  while (*prec != NULL && r < 0) {
    r = compTache(*prec, jour, heure);
    if (r < 0)
      prec = &(*prec)->suiv;
  }
  /* si on a pas déjà une action le meme jour à la même heure */
  if (r != 0) {
    tmp = *prec;
    *prec = (*prec)->suiv;
    free(tmp);
  }
  return tache;
}

agenda_t *supprimeElt(agenda_t *agenda, char annee[5], char semaine[3],
                      jour_t jour, char heure[3]) {
  int r = -1;
  agenda_t **prec = &agenda;
  agenda_t *tmp;

  while (*prec != NULL && r < 0) {
    r = compAgendaElt(*prec, annee, semaine);
    if (r < 0)
      prec = &(*prec)->suiv;
  }
  if (r == 0) {
    (*prec)->actions = supprimeTache((*prec)->actions, jour, heure);
    if ((*prec)->actions == NULL) {
      tmp = *prec;
      *prec = (*prec)->suiv;
      free(tmp);
    }
  }
  return agenda;
}

/* compare un élément de l'agenda à une annee et un numéro de semaine (utilisé
 * pour ginsertion dans liste triée)
 */
int compAgendaElt(agenda_t *elt, char annee[5], char semaine[3]) {
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

void tacheToString(tache_t *tache, char buff[]) {
  buff[6] = tache->j + '0';
  buff[7] = 0;
  strcat(buff, tache->heure);
  strncat(buff, tache->nom, 10);
  buff[19] = 0;
}

void ecritFichier(FILE *f, agenda_t *agenda) {
  agenda_t *cour = agenda;
  tache_t *courTache;
  char buff[20];

  if (f != NULL) {
    while (cour != NULL) {
      strcpy(buff, cour->annee);
      strcat(buff, cour->semaine);
      courTache = cour->actions;
      while (courTache != NULL) {
        tacheToString(courTache, buff);
        fprintf(f, "%s\n", buff);
        courTache = courTache->suiv;
      }
      cour = cour->suiv;
    }
  }
}

void sauvFichier(char *nom, agenda_t *agenda) {
  FILE *f = fopen(nom, "w");
  if (f != NULL) {
    ecritFichier(f, agenda);
  } else {
    fprintf(stderr, "ERREUR: impossible d'ouvrir fihcier de sauvegarde.");
  }
}

/*****************************************************************************/
/* FONCTION D'AFFICHAGE                                                     */
/*****************************************************************************/

void afficheTache(tache_t *taches) {
  int i;
  tache_t *cour = taches;

  while (cour != NULL) {
    printf("jour: %d\n", cour->j);
    printf("heure: %s\n", cour->heure);
    printf("nom: ");
    for (i = 0; i < 10; ++i)
      printf("%c", cour->nom[i]);
    cour = cour->suiv;
  }
}

/* affiche un élément de l'agenda */
void afficheAgendaElt(agenda_t *agendaElt) {
  printf("annee: %s\n", agendaElt->annee);
  printf("semaine: %s\n", agendaElt->semaine);
  afficheTache(agendaElt->actions);
  printf("\n");
}

/* affiche tous les éléments présents dans l'agenda */
void afficheAgenda(agenda_t *agenda) {
  int i = 0;
  agenda_t *cour = agenda;

  while (cour != NULL) {
    printf("element %d:\n", i);
    afficheAgendaElt(cour);
    cour = cour->suiv;
    ++i;
  }
}
