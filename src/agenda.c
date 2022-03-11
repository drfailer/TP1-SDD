/*               FICHIER AGENDA.C
 *
 * Contient les fonctions relatives au type agenda_t
 */
#include "agenda.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************/
/* CONSTRUCTEUR ET DESTRUCTEUR                                               */
/*****************************************************************************/

/*=============================================================================
 *                         Fonction creeAgenda :
 *=============================================================================
 *
 * allocation dynamique et initialisation des champs d'un nouvel agenda_t
 *
 * En entrée :
 *   - annee : Chaîne de caractères contenant l'année de l'élément à créer.
 *   - semaine : Chaîne de caractères contenant la semaine de l'élément à créer.
 *
 * Variables locales :
 *   - agenda : Adresse indirecte de l'agenda nouvellement créé.
 *
 * En sortie :
 *   - agenda : Adresse indirecte de l'agenda nouvellement créé ou NULL si
 *              l'allocation n'a pas abouti
 *=============================================================================
 */

agenda_t *creeAgenda(char annee[TAILLE_ANNEE], char semaine[TAILLE_SEMAINE]) {

  agenda_t * agenda = (agenda_t *)malloc(sizeof(agenda_t));

  // Si l'allocation s'est bien passée
  if (agenda != NULL) {
    stpcpy(agenda->annee, annee);
    stpcpy(agenda->semaine, semaine);
    agenda->suiv = NULL;
    agenda->actions = NULL;
  }

  return agenda;
}

/*=============================================================================
 *                              Fonction freelst :
 *=============================================================================
 *
 * libère la liste qui contient l'agenda
 * NOTE: fait appel à la fonction `freeTache` définie dans `tache.c`
 *
 * En entrée :
 *    - agenda : Adresse de la tête fictive de l'agenda à libérer
 *
 * Variables locales :
 *    - cour : Adresse indirecte sur l'élément courant dans le parcours de la
 *             liste
 *    - suiv : Adresse indirecte sur l'élément suivant dans le parcours de la
 * liste
 *=============================================================================
 */

void freelst(agenda_t ** agenda) {

  agenda_t * cour;
  agenda_t * suiv;

  // Si l'agenda à supprimer n'est pas vide
  if (agenda != NULL) {

    cour = *agenda;

    // Tant que la liste commençant à cour n'est pas vide
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


/*=============================================================================
 *                                Fonction parseLigne :
 *=============================================================================
 *
 * Permet d'extraire les informations d'une ligne lors de l'importation d'un
 * fichier.
 *
 * En entrée :
 *    - s : Chaîne de caractères correspondant à la ligne à traiter
 *    - annee : Chaîne de caractères qui reçoit les caractères de s
 *              correspondant à l'année
 *    - semaine : Chaîne de caractères recevant les caractères de s
 *                correspondant à la semaine
 *    - jour : Enumération reçevant le nombre de s correspondant au jour
 *    - heure : Chaîne de caractères recevant les caractères de s correspondant
 *              à l'heure de l'action
 *    - nom : Chaîne de caractères recevant les caractères de s correspondant au
 *            nom de l'action
 *=============================================================================
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

/*=============================================================================
 *                           Fonction ajouteAgenda :
 *=============================================================================
 *
 * Ajoute un élément dans la liste triée qui contient les éléments de l'agenda
 *
 * En entrée :
 *    - agenda : Tête fictive de la liste chaînée triée représentant l'agenda
 *    - annee : Chaîne de caractères contenant l'année de l'élément à ajouter
 *    - semaine : Chaîne de caractères contenant la semaine de l'élément à
 *                ajouter
 *    - jour : Enumération représentant le jour de l'action à ajouter
 *    - heure : Chaîne de caractères contenant l'heure de l'action à ajouter
 *    - nom : Chaîne de caratcères contenant le nom de l'action à ajouter
 *
 * Variables globales :
 *    - r : Entier représentant le résultat de la comparaison entre l'élément
 *          courant et les informations en paramètres
 *    - prec : Adresse du pointeur sur l'élément précédant dans le parcours
 *             de l'agenda
 *    - tmp : Elément temporaire contenant l'adresse de l'élément agenda_t dont
 *            les informations sont données en paramètres
 *
 * En sortie :
 *    - agenda : Tête fictive de l'agenda modifié
 *=============================================================================
 */

agenda_t * ajouteAgenda(agenda_t * agenda, char annee[TAILLE_ANNEE],
                       char semaine[TAILLE_SEMAINE], jour_t jour,
                       char heure[TAILLE_HEURE], char nom[TAILLE_NOM]) {

  int         r = -1;
  agenda_t ** prec = &agenda;
  agenda_t  * tmp;

  /* Tant que l'on est pas à la fin de l'agenda et que l'année ou la semaine
     est inférieure à celles où l'on souhaite insérer
  */
  while (*prec != NULL && r < 0) {

    r = compAgendaElt(*prec, annee, semaine);

    /* Si l'année ou la semaine de l'élément de l'agenda considéré est
       inférieure à celles que l'on cherche à insérer
     */
    if (r < 0)
      prec = &(*prec)->suiv;
  }

  // S'il existe un élément avec la même année et la même semaine
  if (r == 0) {
    (*prec)->actions = ajouteTache((*prec)->actions, jour, heure, nom);
  }

  else {
    tmp = creeAgenda(annee, semaine);

    // Si la création s'est bien passée
    if (tmp != NULL) {
      tmp->suiv = *prec;
      *prec = tmp;
      tmp->actions = ajouteTache(tmp->actions, jour, heure, nom);
    }
  }

  return agenda;
}

/*=============================================================================
 *                            Fonction listeMotif :
 *=============================================================================
 * Trouve les jour où le nom de l'action correspond à `motif` et met l'addresse
 * de la tache correspondante dans `deb`
 *
 * En entrée :
 *    - agenda : Tête fictive de l'agenda à parcourir
 *    - deb : Adresse indirecte sur le pointeur de début de la liste contigue
 *            de tâches contenant le motif dans leur nom
 *    - motif : Chaîne de caractère contenant le motif à rechercher
 *
 * Variables locales :
 *    - cour_a : Adresse indirecte sur l'élément courant lors du parcours
 *               de l'agenda
 *    - cour_t : Adresse indirecte de la tache courante lors du parcours
 *               des actions
 *    - i : Compteur du nombre d'éléments dans la liste contigue
 *    - continuer : Entier utilisé comme bouléen qui permet d'arrêter
 *                  prématurément le programme s'il y a eu un problème
 *                  d'allocation
 *
 * En sortie :
 *    - deb + i : L'adresse de fin de la liste 'deb'
 *===============================================================================
 */

char ** listeMotif(agenda_t * agenda, char ** deb, char * motif) {
  agenda_t * cour_a = agenda;
  tache_t  * cour_t = NULL;
  int        i = 0;
  int        continuer = 1;

  // parcour de l'agenda
  while (cour_a != NULL && continuer) {
    cour_t = cour_a->actions;

    // parcour de la liste d'actions
    while (cour_t != NULL && continuer) {

      // si le motif correspond, on ajoute l'ellement à la liste
      if (motifCorrespond(motif, cour_t->nom)) {

        deb[i] = (char *)malloc(TAILLE_BUFFER * sizeof(char));

        // Si l'espace a été alloué
        if (deb[i] != NULL) {
          agendaToString(cour_a, deb[i]);
          tacheToString(cour_t, deb[i]);
          ++i;
        }

        else {
          continuer = 0;

          // Libération de l'espace ayant été alloué
          for (i = i - 1; i >= 0; i--) {
            free(deb[i]);
          }
        }
      }

      cour_t = cour_t->suiv;
    }
    cour_a = cour_a->suiv;
  }

  return deb + i; // on retourne la fin de la liste
}

/*=============================================================================
 *                       Fonction creerListeContigue :
 *=============================================================================
 *
 * Génère une liste contigue contenant les jours présents dans dans l'agenda où
 * le nom de l'action contient `motif`
 * La liste contigue est constituée d'un pointeur de début `deb` et d'un
 * poineur de fin `fin`
 * Allocation dynamique de taille `MAX_ESPACE_CONTIGU` pour la liste
 *
 * En entrée :
 *    - agenda : Tête fictive de l'agenda à parcourir
 *    - deb : Pointeur sur le pointeur sur le pointeur sur le début de l'espace
 *            contigu
 *    - fin : Pointeur sur le pointeur sur le pointeur sur la fin de l'espace
 *            contigu
 *    - motif : Chaîne de caractères contenant le motif à chercher
 *=============================================================================
 */

void creerListeContigue(agenda_t * agenda, char *** deb, char *** fin,
                        char * motif) {

  *deb = (char **)malloc(MAX_ESPACE_CONTIGU * sizeof(char *));

  // Si l'allocation a eu lieu
  if (*deb != NULL) {

    *fin = listeMotif(agenda, *deb, motif);
  }

  else {
    fprintf(stderr, "Error: impossible d'allouer l'espace contigu\n");
  }
}

/*=============================================================================
 *                      Fonction agendaViafichier :
 *=============================================================================
 *
 * Créer un agenda à partir des données stoquées dans le fichier `nom`
 * NOTE: format des donnée
 * AAAASSJHHNom tache_
 * (où _ est un espace qui sert à completer le nom à 10 caractères)
 *
 * En entrée :
 *    - nom : Chaîne de caractères contenant le nom du fichier à ouvrir
 *
 * Variables locales :
 *    - f : Pointeur FILE sur le fichier ouvert
 *    - agenda : Tête fictive de l'agenda créé à partir du fichier
 *    - buff : Buffer permettant de récupérer les données du fichier
 *    - nomAction : Chaîne de caractères récupérant le nom de l'action pour
 *                  chaque ligne du fichier
 *    - annee : Chaîne de caractères récupérant l'année de l'action pour
 *              chaque ligne du fichier
 *    - semaine : Chaîne de caractères récupérant la semaine de l'action
 *                pour chaque ligne du fichier
 *    - jour : Enumération récupérant le jour de l'action pour chaque ligne
 *             du fichier
 *    - heure : Chaîne de caractères récupérant l'heure de l'action pour
 *              chaque ligne du fichier
 *
 * En sortie :
 *    - agenda : Tête fictive de l'agenda nouvellement créé
 *=============================================================================
 */

agenda_t * agendaViafichier(char * nom) {
  
  FILE     * f = fopen(nom, "r");
  agenda_t * agenda = NULL;
  char       buff[TAILLE_BUFFER] = { 0 };
  char       nomAction[TAILLE_NOM];
  char       annee[TAILLE_ANNEE];
  char       semaine[TAILLE_SEMAINE];
  jour_t     jour;
  char       heure[TAILLE_HEURE];

  // Si l'ouverture du fichier s'est bien passée
  if (f != NULL) {

    // Parcours du fichier ligne par ligne
    while (!feof(f)) {
      fgets(buff, TAILLE_BUFFER, f);
      // si la ligne n'est pas vide
      if (buff[0] != '\n' && buff[0] != 0) {
        parseLigne(buff, annee, semaine, &jour, heure, nomAction);
        agenda = ajouteAgenda(agenda, annee, semaine, jour, heure, nomAction);
      }
    }

    fclose(f);

  }

  else {
    fprintf(stderr, "ERREUR: ouverture de fichier.");
  }

  return agenda;
}

/*=============================================================================
 *                         Fonction ecritFichier :
 *=============================================================================
 *
 * Sauvegarde les données de l'agenda dans un fichier.
 *
 * En entrée :
 *    - agenda : Tête fictive de l'agenda
 *
 * Variables locales :
 *    - cour : Adresse indirecte de l'élément considéré dans le parcours
 *             de l'agenda
 *    - courTache : Adresse indirecte de la tache considérée dans le parcours
 *                  des taches
 *    - buff : Chaîne de caractères servant de buffer récupérant la
 *             transformation de chaque action de l'agenda en chaîne
 *	       de caractères
 *=============================================================================
 */

void ecritFichier(FILE * f, agenda_t * agenda) {
  agenda_t	* cour = agenda;
  tache_t	* courTache;
  char		  buff[TAILLE_BUFFER];

  // Si le fichier donné a bien été ouvert correctement
  if (f != NULL) {

    // Parcours de l'agenda
    while (cour != NULL) {
      agendaToString(cour, buff);
      courTache = cour->actions;

      // Parcours des tâches
      while (courTache != NULL) {
        tacheToString(courTache, buff);
        fprintf(f, "%s\n", buff);
        courTache = courTache->suiv;
      }

      cour = cour->suiv;
    }
  }
}

/*=============================================================================
 *                         Fonction sauvFichier :
 *=============================================================================
 *
 * Ouvre le fichier `nom` en mode écriture et utilise la fonction
 * `ecritFichier` pour sauvegarder l'agenda courant dedans
 *
 * En entrée :
 *    nom : Chaîne de caractères contenant le nom du fichier à ouvrir
 *    agenda : Tête fictive de l'agenda à sauvegarder dans un fichier
 *
 * Variables locales :
 *    f : Pointeur FILE sur le fichier ouvert
 *=============================================================================
 */

void sauvFichier(char * nom, agenda_t * agenda) {
  FILE * f = fopen(nom, "w");

  // Si l'ouverture du fichier a bien eu lieu
  if (f != NULL) {
    ecritFichier(f, agenda);
    fclose(f);
  }

  else {
    fprintf(stderr, "ERREUR: impossible d'ouvrir fichier de sauvegarde.");
  }
}

/*=============================================================================
 *                          Fonction supprimeElt :
 *=============================================================================
 *
 * Recherche l'élément de l'agenda programmé à `annee`, `semaine` pour `jour` à
 * `heure` et supprime la tache correspondante si elle existe.
 * L'élément de l'agenda est aussi supprimé si la liste d'action à effectuer
 * est vide.
 *
 * En entrée :
 *    - agenda : Tête fictive de l'agenda où supprimer l'élément
 *    - annee : Chaîne de caractères contenant l'année de l'élément
 *              à supprimer
 *    - semaine : Chaîne de caractères contenant la semaine de l'élément
 *                à supprimer
 *    - jour : Itération de l'énumération jour_t représentant le jour de
 *             l'action à supprimer
 *    - heure : Chaîne de caractères contenant l'heure de l'action à
 *              supprimer
 *
 * Variables locales :
 *    - r : Entier représentant le résultat de la comparaison entre
 *          les éléments
 *    - prec : Pointeur sur l'adresse indirecte de l'élément précédant
 *             l'élément considéré dans le parcours de l'agenda
 *    - tmp : Element temporaire permettant la libération de l'élément
 *            à supprimer
 *
 * En sortie :
 *    - agenda : La tête fictive de l'agenda modifié
 *=============================================================================
 */

agenda_t *supprimeElt(agenda_t * agenda, char annee[TAILLE_ANNEE],
                      char semaine[TAILLE_SEMAINE], jour_t jour,
                      char heure[TAILLE_HEURE]) {

  int         r = -1;
  agenda_t ** prec = &agenda;
  agenda_t  * tmp;

  /* Tant que la liste n'est pas terminée et que l'année ou la
     semaine de l'élément considéré est inférieure à celle donnée
     en paramètre */
  while (*prec != NULL && r < 0) {
    r = compAgendaElt(*prec, annee, semaine);

    /* Si l'année ou la semaine de l'élément considéré est
       inférieure à celle donnée en paramètre */
    if (r < 0)
      prec = &(*prec)->suiv;
  }

  // Si l'élément considéré correspond aux informations données
  if (r == 0) {
    (*prec)->actions = supprimeTache((*prec)->actions, jour, heure);

    // Suppression de l'élément s'il n'y a plus d'actions
    if ((*prec)->actions == NULL) {
      tmp = *prec;
      *prec = (*prec)->suiv;
      free(tmp);
    }
  }

  return agenda;
}

/*=============================================================================
 *                          Fonction compAgendaElt :
 *=============================================================================
 *
 * Compare un élément de l'agenda à une année et un numéro de semaine (utilisé
 * pour l'insertion dans liste triée)
 *
 * En entrée :
 *    - elt : Element de l'agenda à comparer
 *    - annee : Chaîne de caractères contenant l'année sur laquelle faire
 *              la comparaison
 *    - semaine : Chaîne de caractères contenant la semaine sur laquelle
 *                faire la comparaison
 *
 * Variables locales :
 *    - resultat : Entier contenant le résultat de la comparaison
 *
 * En sortie :
 *    - resultat : Entier représentant le résultat de la comparaison.
 *                 Contient 0 si l'année et la semaine de l'élément et
 *		   celles renseignées en paramètre sont égales
 *		   Contient une valeur positive si :
 *		      - L'année de l'élément est supérieure à l'année en
 *		        paramètre
 *		      - Les deux années sont égales mais la semaine de
 *		        l'élément est supérieure à la semaine en paramètre
 *		   Contient une valeur négative si :
 *		      - l'année de l'élément est inférieure à l'année en
 *		        paramètre
 *		      - Les deux années sont égales mais la semaine de
 *		        l'élément est inférieure à la semaine en paramètre
 *=============================================================================
 */

int compAgendaElt(agenda_t * elt, char annee[TAILLE_ANNEE],
                  char semaine[TAILLE_SEMAINE]) {

  int resultat = 0;

  // compare l'annee:
  resultat = strcmp(elt->annee, annee);

  // compare la semaine si les années sont égales
  if (resultat == 0) {
    resultat = strcmp(elt->semaine, semaine);
  }

  return resultat;
}

/*=============================================================================
 *                        Fonction agendaToString :
 *=============================================================================
 * Transforme les informations de l'élément en paramètre en une
 * chaîne de caractère stockée dans le buffer. La chaîne de
 * caractères sera constituée de l'année suivie de la semaine.
 *
 * En entrée :
 *    - agendaElt : Element de l'agenda dont on souhaite transformer
 *                  les informations en une chaîne de caractères
 *    - buff : Chaîne de caractère où est stockée la transformation
 *=============================================================================
 */

void agendaToString(agenda_t * agendaElt, char buff[]) {

  strcpy(buff, agendaElt->annee);
  strcat(buff, agendaElt->semaine);
}

/*****************************************************************************/
/* FONCTION D'AFFICHAGE                                                      */
/*****************************************************************************/

/*=============================================================================
 *                        Fonction afficheAgendaElt :
 *=============================================================================
 *
 * Affiche un élément de l'agenda
 *
 * En entrée :
 *    - agendaElt : Adresse indirecte de l'élément à afficher
 *=============================================================================
 */

void afficheAgendaElt(agenda_t * agendaElt) {

  printf("|  annee: %s\n", agendaElt->annee);
  printf("|  semaine: %s\n", agendaElt->semaine);
  afficheTache(agendaElt->actions);
  printf("\n");
}

/*=============================================================================
 *                        Fonction afficheAgenda :
 *=============================================================================
 *
 * Affiche tous les éléments présents dans l'agenda
 *
 * En entrée :
 *    - agenda : Tête fictive de l'agenda à afficher
 *
 * Variables locales :
 *    - cour : Adresse indirecte sur l'élément courant dans
 *             le parcours de l'agenda
 *=============================================================================
 */

void afficheAgenda(agenda_t * agenda) {

  agenda_t * cour = agenda;

  clear();

  // Si la liste (l'agenda) est vide
  if (cour == NULL) {
    printf(" =================================== \n"
           "|    _                              |\n"
           "|   / \\                             |\n"
           "|  / | \\  Votre agenda est vide !   |\n"
           "| /  .  \\                           |\n"
           "|/_______\\                          |\n"
           "|                                   |\n"
           " ===================================\n\n");
  }

  else {
    printf("Agenda :\n");

    // Tant que l'on a pas parcouru toute la liste (l'agenda)
    while (cour != NULL) {
      afficheAgendaElt(cour);
      printf("|_\n\n");
      cour = cour->suiv;
    }
  }
  printf("[Appuyer Entrer]\n");
}
