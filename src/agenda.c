#include <stdio.h>
#include <stdlib.h>
#include "agenda.h"
#include "util.h"

/*****************************************************************************/
/* CONSTRUCTEUR ET DESTRUCTEUR                                               */
/*****************************************************************************/

/* allocation dynamique et initialisation des champs d'un nouvel agenda_t */
agenda_t *creeAgenda(unsigned short int annee[4], unsigned short int semaine[2]) {
  agenda_t *agenda =  (agenda_t *)malloc(sizeof(agenda_t));
  if (agenda != NULL) {
    cpyTab(agenda->annee, annee, 4);
    cpyTab(agenda->semaine, semaine, 2);
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

/* Ajoute un élément dans la liste triée qui contient les éléments de l'agenda
 */
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

/* Créer un agenda à partir des données stoquées dans le fichier `nom`
 * NOTE: format des donnée
 * AAAASSJHHNom tache_
 * (où _ est un espace qui sert à completer le nom à 10 caractères)
 */
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


/*****************************************************************************/
/* FONCTIONS DE DIVERSES                                                     */
/*****************************************************************************/

/* compare un élément de l'agenda à une annee et un numéro de semaine (utilisé
 * pour insertion dans liste triée)
 */
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

/*****************************************************************************/
/* FONCTIONS D'AFFICHAGE                                                     */
/*****************************************************************************/

/* affiche un élément de l'agenda */
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
