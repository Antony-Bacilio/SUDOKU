#ifndef FONCTIONS
#define FONCTIONS

#include "structures.h"

int choixGrille();
void lireGrille(int G[][9]);
void ecrireGrille(int G[][9]);
int estCandidat(int G[][9], int nb, int id, int jd);
int initTab(int G[][9], Cand C[][9], Case O[]);
void ecrireCand(int G[][9],  Cand C[][9], Case O[], int NBO);
int admetUnique(int, int, int G[][9], Cand C[][9]);
int fermerCase(int, int, int, int G[][9], Cand C[][9], Case *O);
int fermerGrille(int, int G[][9], Cand C[][9], Case *O);
void grilleDifficile(int G[][9],  Cand C[][9], Case O[], int NBO);

#endif
