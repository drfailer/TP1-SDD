#include "agenda.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************/
/* CONSTRUCTEUR ET DESTRUCTEUR                                               */
/*****************************************************************************/

/* allocation dynamique et initialisation des champs d'un nouvel agenda_t */
agenda_t *creeAgenda(char annee[TAILLE_ANNEE], char semaine[TAILLE_SEMAINE]) {
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

/* Permet d'extraire les informations d'une ligne lors de l'importation d'un
 * fichier.
 */
void parseLigne(char *s, char annee[TAILLE_ANNEE], char semaine[TAILLE_SEMAINE],
                jour_t *jour, char heure[TAILLE_HEURE], char nom[TAILLE_NOM]) {
  s = parseSuiteNombres(s, annee, TAILLE_ANNEE - 1);
  annee[TAILLE_ANNEE - 1] = 0;
  s = parseSuiteNombres(s, semaine, TAILLE_SEMAINE - 1);
  semaine[TAILLE_SEMAINE - 1] = 0;
  *jour = *s - '0';
  s = parseSuiteNombres(s + 1, heure, TAILLE_HEURE - 1);
  heure[TAILLE_HEURE - 1] = 0;
  strncpy(nom, s, TAILLE_NOM - 1);
}

/* Ajoute un élément dans la liste triée qui contient les éléments de l'agenda
 */
agenda_t *ajouteAgenda(agenda_t *agenda, char annee[TAILLE_ANNEE],
                       char semaine[TAILLE_SEMAINE], jour_t jour,
                       char heure[TAILLE_HEURE], char nom[TAILLE_NOM]) {
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

/* Trouve les jour où le nom de l'action correspond à `motif` et met l'addresse
 * de la tache correspondante dans `deb`
 * retourne le pointeur de fin de la liste `deb`
 */
tache_t **listeMotif(agenda_t *agenda, tache_t **deb, char *motif) {
  agenda_t *cour_a = agenda;
  tache_t *cour_t = NULL;
  int i = 0;

  printf("debug1\n");
  // parcour de l'agenda
  while (cour_a != NULL) {
    cour_t = cour_a->actions;
    printf("debug2\n");
    // parcour de la liste d'actions
    while (cour_t != NULL) {
      printf("debug3\n");
      // si le motif correspond, on ajoute l'ellement à la liste
      if (motifCorrespond(motif, cour_t->nom)) {
        deb[i] = cour_t;
        ++i;
      }
      cour_t = cour_t->suiv;
    }
    cour_a = cour_a->suiv;
  }
  return deb + i; // on retourne la fin de la liste
}

/* Génère une liste contigue contenant les jours présents dans dans l'agenda où
 * le nom de l'action contient `motif`
 * La liste contigue est constituée d'un pointeur de début `deb` et d'un
 * poineur de fin `fin`
 * Allocation dynamique de taille `MAX_ESPACE_CONTIGU` pour la liste
 */
void creerListeContigue(agenda_t *agenda, tache_t ***deb, tache_t ***fin,
                        char *motif) {
  *deb = (tache_t **)malloc(MAX_ESPACE_CONTIGU * sizeof(tache_t *));
  if (*deb != NULL) {
    *fin = listeMotif(agenda, *deb, motif);
  } else {
    fprintf(stderr, "Error: impossible d'allouer l'espace contigu\n");
  }
}

/* Affiche liste contigue de `tache_t*`
 * `deb` est un pointeur sur le début de la liste et `fin` sur la fin de la
 * liste.
 */
void afficheListeContigue(tache_t **deb, tache_t **fin) {
  while (deb != fin) {
    printf("jour: %d\n", (*deb)->j);
    printf("heure: %s\n", (*deb)->heure);
    printf("nom: %s\n", (*deb)->nom);
    printf("\n");
    ++deb;
  }
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
  char buff[TAILLE_BUFFER];
  char nomAction[TAILLE_NOM];
  char annee[TAILLE_ANNEE];
  char semaine[TAILLE_SEMAINE];
  jour_t jour;
  char heure[TAILLE_HEURE];

  if (f != NULL) {
    while (!feof(f)) {
      fgets(buff, TAILLE_BUFFER, f);
      parseLigne(buff, annee, semaine, &jour, heure, nomAction);
      agenda = ajouteAgenda(agenda, annee, semaine, jour, heure, nomAction);
    }
    fclose(f);
  } else {
    fprintf(stderr, "ERREUR: ouverture de fichier.");
  }
  return agenda;
}

void ecritFichier(FILE *f, agenda_t *agenda) {
  agenda_t *cour = agenda;
  tache_t *courTache;
  char buff[TAILLE_BUFFER];

  if (f != NULL) {
    while (cour != NULL) {
      agendaToString(cour, buff);
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

/* Ouvrer le fichier `nom` en mode écriture et utilise la fonction
 * `ecritFichier` pour sauvegarder l'agenda courant dedans
 */
void sauvFichier(char *nom, agenda_t *agenda) {
  FILE *f = fopen(nom, "w");
  if (f != NULL) {
    ecritFichier(f, agenda);
    fclose(f);
  } else {
    fprintf(stderr, "ERREUR: impossible d'ouvrir fihcier de sauvegarde.");
  }
}

/*****************************************************************************/
/* FONCTIONS DE DIVERSES                                                     */
/*****************************************************************************/

/* Recherche l'élément de l'agenda programmé à `annee`, `semaine` pour `jour` à
 * `heure` et supprime la tache correspondante.
 * L'élément de l'agenda est aussi supprimé si la liste d'action à effectuer
 * est vide
 */
agenda_t *supprimeElt(agenda_t *agenda, char annee[TAILLE_ANNEE],
                      char semaine[TAILLE_SEMAINE], jour_t jour,
                      char heure[TAILLE_HEURE]) {
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
    // supprime l'élément si il n'y a plus d'actions
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
int compAgendaElt(agenda_t *elt, char annee[TAILLE_ANNEE],
                  char semaine[TAILLE_SEMAINE]) {
  int resultat = 0;

  // compare l'annee:
  resultat = strcmp(annee, elt->annee);
  // compare la semaine
  if (resultat == 0) {
    resultat = strcmp(elt->semaine, semaine);
  }
  return resultat;
}

void agendaToString(agenda_t *agendaElt, char buff[]) {
  strcpy(buff, agendaElt->annee);
  strcat(buff, agendaElt->semaine);
}

/*****************************************************************************/
/* FONCTION D'AFFICHAGE                                                     */
/*****************************************************************************/

/* affiche un élément de l'agenda */
void afficheAgendaElt(agenda_t *agendaElt) {
  printf("|  annee: %s\n", agendaElt->annee);
  printf("|  semaine: %s\n", agendaElt->semaine);
  afficheTache(agendaElt->actions);
  printf("\n");
}

/* affiche tous les éléments présents dans l'agenda */
void afficheAgenda(agenda_t *agenda) {
  int i = 0;
  agenda_t *cour = agenda;

  clear();
  printf("Agenda :\n");

  while (cour != NULL) {
    // printf("element %d:\n", i);
    afficheAgendaElt(cour);
    printf("|_\n\n");
    cour = cour->suiv;
    ++i;
  }
  printf("[Appuyer Entrer]");
}
