//structure pour les candidats d' une case ouverte

#ifndef STRUCTURES
#define STRUCTURES

typedef struct {
  int x; //numéro de la ligne
  int y; //numéro de colonne
} Case;

typedef struct {
  int nbc; //nombre de candidats
  int *tab; //table des candidats
} Cand;

#endif
