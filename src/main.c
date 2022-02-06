#include "util.h"
#include "agenda.h"
#include "tache.h"
#include <stdio.h>

// ce qu'on a fait à la première séance:
/* void test1() { */
/*   char *test = (char *)malloc(50 * sizeof(char)); */
/*   agenda_t *agendaElt; */
/*   if (test != NULL) { */
/*     strcpy(test, "202215108TPs de SDD"); */
/*     agendaElt = parseLigne(test); */
/*     afficheAgendaElt(agendaElt); */
/*     free(test); */
/*     free(agendaElt); */
/*   } else { */
/*     fprintf(stderr, "NULL\n"); */
/*   } */
/* } */

int main() {
  agenda_t *agenda = agendaViafichier("test.txt");
  afficheAgenda(agenda);
  freelst(&agenda);
  return 0;
}
