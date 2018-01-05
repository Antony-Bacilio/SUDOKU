#include <stdio.h>
#include <stdlib.h>
#include "../headers/fonctions.h"

int main(int argc, char const *argv[])
{

  //  Déclarations
  int G[9][9];
  int NBO=0;
  Cand C[9][9];
  Case O[81];



  // Appels de fonctions
  lireGrille(G);
  printf("\nOn tenter de resoudre ce sudoku\n");
  ecrireGrille(G);
  NBO = initTab(G,C,O);
  printf("Nombre de cases ouvertes: %d\n",NBO);
  printf("Candidats pour chaque case ouverte: \n");
  ecrireCand(G,C,O,NBO);
  NBO = fermerGrille(NBO,G,C,O);
  printf("\nSolution du sudoku \n");
  ecrireGrille(G);
  printf("Case(s) candidate(s) : %d\n",NBO);
  grilleDifficile(G,C,O,NBO);
  return 0;
}
