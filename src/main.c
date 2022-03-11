/*            FICHIER MAIN :
 *
 * Contient le programme principal
 *
 */
#include "agenda.h"
#include "tache.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*=============================================================================
 *                       Fonction gestionEvenements :
 *=============================================================================
 *
 * exécute des actions en fonction de `entree`, qui est le caratère entré par
 * l'utilisateur dans `run`
 *
 * En entrée :
 *    - entree : Caractère représentant le choix de l'utilisateur à traiter.
 *    - agenda : Tête fictive de l'agenda de l'utilisateur.
 *    - continuer : Pointeur sur un entier utilisé comme booléen permettant
 *                  au programme de s'arrêter s'il est à 0.
 *
 * Variables locales :
 *    - jour : Entier permettant de recueillir un numéro correspondant à
 *             un jour de la semaine auprès de l'utilisateur.
 *    - annee : Chaîne de caractères permettant de recueillir une année auprès
 *              de l'utilisateur.
 *    - heure : Chaîne de caractères permettant de recueillir une heure auprès
 *              de l'utilisateur.
 *    - nom : Chaîne de caractères permettant de recueillir un nom d'action a
 *près de l'utilisateur.
 *    - nomFichier : Chaîne de caractères permettant de recueillir un nom de
 *                   fichier auprès de l'utilisateur.
 *    - motif : Chaîne de caractères permettant de recueillir un motif auprès
 *              de l'utilisateur.
 *    - deb : Adresse de l'adresse indirecte de la tache de départ dans la liste
 *            contigue des actions dont le nom contient un certain motif.
 *    - fin : Adresse de l'adresse indirecte de la dernière tache dans la liste
 *            contigue des actions dont le nom contient un certain motif.
 *
 * En sortie :
 *    - agenda : L'agenda modifié
 =============================================================================
 */
agenda_t * gestionEvenements(char entree, agenda_t *agenda, int *continuer) {
  
  int     jour;
  char    annee[TAILLE_ANNEE];
  char    semaine[TAILLE_SEMAINE];
  char    heure[TAILLE_HEURE];
  char    nom[TAILLE_NOM];
  char    nomFichier[50] = "save_agenda.txt";
  char    motif[TAILLE_NOM];
  char ** deb = NULL;
  char ** fin = NULL;

  switch (entree) {

    // Si l'utilisateur veut ajouter un élément
  case '0':
    demandeInfos(annee, semaine, heure, &jour);
    demandeNom(nom);
    agenda = ajouteAgenda(agenda, annee, semaine, jour, heure, nom);
    break;

    // Si l'utilisateur veut supprimer un élément
  case '1':
    demandeInfos(annee, semaine, heure, &jour);
    agenda = supprimeElt(agenda, annee, semaine, jour, heure);
    break;

    // Si l'utilisatur veut afficher son agenda
  case '2':
    afficheAgenda(agenda);
    scanf("%*c");
    break;

    // Si l'utilisateur veut sauvegarder son agenda ou...
  case '3':

    // ...si l'utilisateur veut importer un nouvel agenda
  case '4':
    clear();
    printf("Saisir nom du fichier\n");
    scanf("%s%*c", nomFichier);
    // Si l'utilisateur veut sauvegarder son agenda
    if (entree == '3') {
      sauvFichier(nomFichier, agenda);
    }
    // Si l'utilisateur veut importer un nouvel agenda
    else {

      /* Si l'utilisateur a confirmé vouloir
         remplacer son ancien agenda */
      if (demandeConfirmation()) {
        freelst(&agenda); // suppression de l'ancient agenda
        agenda = agendaViafichier(nomFichier); // importation du nouvel agenda
        printf("agenda: %p\n", agenda);
      }
    }
    break;

    /* Si l'utilisateur veut chercher toutes les actions dont le nom
       contient un certain motif */
  case '5':
    printf("Entreer motif:\n");
    scanf("%s%*c", motif);
    creerListeContigue(agenda, &deb, &fin, motif);
    clear();
    printf("Actions correspondants à \"%s\":\n", motif);
    afficheListeContigue(deb, fin);
    printf("[Appuyer Entrer]");
    scanf("%*c");

    for (int i = 0; i < fin - deb; i++) {
      free(deb[i]);
    }
    free(deb);
    break;

    // Si l'utilisateur veut quitter l'application
  default:
    afficheMessageFin();
    *continuer = 0;
    break;
  }

  return agenda;
}



/*=============================================================================
 *                            Fonction run :
 *=============================================================================
 *
 * La boucle principale, execution de l'application
 *
 * En entrée :
 *    - agenda : Tête fictive de la liste chaînée représentant l'agenda
 *               de l'utilisateur.
 *
 * Variables locales :
 *    - continuer : Booléen permettant l'arrêt de la boucle principale
 *                  lorsqu'il est à 0.
 *    - reponse : Caractère représentant le choix de l'utilisateur.
 *=============================================================================
 */
void run(agenda_t * agenda) {
  
  int  continuer = 1;
  char reponse;

  // Tant que le programme ne doit pas s'arrêter
  while (continuer) {
    afficheMenu();
    scanf("%c%*c", &reponse);
    printf("reponse: %c\n", reponse);
    agenda = gestionEvenements(reponse, agenda, &continuer);
  }
  
  freelst(&agenda);
}




/*=============================================================================
 *                           Fonction main :
 *=============================================================================
 *
 * En entrée :
 *    - argc : Nombre d'arguments donnés à l'appel du programme.
 *    - argv : Liste des arguments donnés à l'appel du programme.
 *
 * Variable locale :
 *    - agenda : Début de la liste chaînée des agenda qui sera utilisée
 *               par le programme.
 *=============================================================================
 */

int main(int argc, char ** argv) {
  
  agenda_t * agenda = NULL;

  // Si le programme a été lancé avec des arguments
  if (argc > 1) {

    /* Si les arguments sont 'import' suivi d'un nom de fichier */
    if (0 == strcmp(argv[1], "import") && 3 == argc) {
      printf("importation de %s.\n", argv[2]);
      agenda = agendaViafichier(argv[2]);
    }
  }
  run(agenda);
  return 0;
}
