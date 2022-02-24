#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "agenda.h"
#include "util.h"


/*****************************************************************************/
/* FONCTIONS SECONDAIRES                                                     */
/*****************************************************************************/

/* récupère une suite de `n` nombres et les met dans tab
 * retourne le pointeur `s`
 */
char *parseSuiteNombres(char *s, char tab[], int n) {
  int i;
  for (i = 0; *s && i < n; ++i, ++s) {
    tab[i] = *s;
  }
  return s;
}
