#ifndef _HACHAGE_H_
#define _HACHAGE_H_
#include "Reseau.h"

typedef struct{
  int nbElement; //pas necessaire ici
  int tailleMax;
  CellNoeud** T;
} TableHachage ;

unsigned long key(double x, double y);
unsigned long fonctionHachage(unsigned long key, int tailleMax);
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage *H, double x, double y);
Reseau* reconstitueReseauHachage(Chaines *C, int M);
void libererTableHachage(TableHachage *table);

#endif