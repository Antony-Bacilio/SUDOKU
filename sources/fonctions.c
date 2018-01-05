#include <stdio.h>
#include <stdlib.h>
#include "../headers/fonctions.h"

int choixGrille()
{
  int choix = 0;
  do{
     printf("Choisir une grille entre la 1 et la 11\n"); //permet de choisir une grille sans passer par la modificiation du code source
     scanf("%d", &choix);
  }while(choix < 1 || choix > 11);
  return choix;
}

void lireGrille(int G[][9]) //lecture de la grille depuis un fichier texte
{
  FILE* fichier = NULL;
  char c;
  int i, j, x , y, nb, choix;

  for (i=0; i<9; i++)
  {
    for (j=0; j<9; j++)
    {
      G[i][j] = 0;  //initalisation de toutes les cases du tableau a 0
		}
  }
  choix = choixGrille();
  switch (choix) //ouverture du fichier contenant les variables de depart de la grille
  {
    case 1: fichier = fopen("../grilles/g1.txt", "r");
            break;
    case 2: fichier = fopen("../grilles/g2.txt", "r");
            break;
    case 3: fichier = fopen("../grilles/g3.txt", "r");
            break;
    case 4: fichier = fopen("../grilles/g4.txt", "r");
            break;
    case 5: fichier = fopen("../grilles/g5.txt", "r");
            break;
    case 6: fichier = fopen("../grilles/g6.txt", "r");
            break;
    case 7: fichier = fopen("../grilles/g7.txt", "r");
            break;
    case 8: fichier = fopen("../grilles/g8.txt", "r");
            break;
    case 9: fichier = fopen("../grilles/g9.txt", "r");
            break;
    case 10: fichier = fopen("../grilles/g10.txt", "r");
            break;
    case 11: fichier = fopen("../grilles/g11.txt", "r");
            break;
  }

  if (fichier != NULL) // <-- si le fichier s'est bien ouvert
  {
    do
    {
      fscanf(fichier,"%d %d %d\n", &x,&y,&nb);
      for (i=0 ; i<9 ; i++)
      {
        for (j=0 ; j<9 ; j++)
        {
          if ((x == i) && (y==j))
          {
            G[x][y] = nb;
						fscanf(fichier,"%d %d %d\n", &x,&y,&nb);  //recuperation des variables dans le fichier
					}
				}
			}
      c = fgetc(fichier);
    }while (c!= EOF); // Lecture des valeurs jusqu'a EOF = End Of File
    fclose(fichier);
  }
  else
  {
    printf("ERREUR : Impossible d'ouvrir le fichier de grille\n"); // Message d'erreur
    exit(0);                                                       // + Arret du porgramme
  }
}

void ecrireGrille(int G[][9]) // Affichage d'une grille de sudoku
{
	int i,j;
	printf("-------------------------------\n");
	for (i=0; i<9; i++) // i represente les lignes
	{
    printf("|");
    for (j=0; j<9; j++) // j represente les colonnes
    {
      if ((j+1)%3) printf(" %d ", G[i][j]); // si c'est la 1ere ou deuxieme valeurs on affiche juste la valeur
      else printf(" %d |", G[i][j]); // sinon on affiche la valeur avec un séparateur
    }
	  printf("\n");
	  if (((i+1)%3) == 0) // si c'est la troisieme, sixieme ou neuvieme ligne on affiche des tirets
	  {
      printf("-------------------------------\n");
    }
	}
}

int estCandidat(int G[][9], int nb, int id, int jd)
{
	int i,j,candidat=1,max_i,max_j,min_i,min_j;
	if (G[id][jd]==0) // case ouverte
	{
		for (i=0; i<9; i++)
		{
			if (G[i][jd]==nb) candidat=0; // test de la ligne //si ==nb --> nombre n'est pas candidat
		}
		for (j=0; j<9; j++)
		{
			if (G[id][j]==nb) candidat=0; // test de la cologne //si ==nb --> nombre n'est pas candidat
		}
	}
	else candidat=0; // deja une valeur dans le tableau -> case non ouverte
	if (candidat==1) // test du carré 3*3 ou se trouve la position de notre G[id][jd]
	{
		if((id%3==0)&&(jd%3==0)) // pour se retrouver en debut de carre 3*3
		{
			max_i=id+2;  // definir la case maximal du petit carre
			max_j=jd+2;
			for (min_i=id ; min_i <= max_i ; min_i++) // parcours du petit carre car impossible d'avoir deux valeurs identique
			{
				for (min_j=jd ; min_j <= max_j ; min_j++)
				{
					if (G[min_i][min_j]==nb) candidat=0;
				}
			}
		}
		else
		{
			while(id%3!=0) { // permet de se retrouver au debut pou tester l'unicite du petit carre entierement
				id--;
			}
			while(jd%3!=0) {
				jd--;
			}
			max_i=id+2;
			max_j=jd+2;
			for (min_i=id ; min_i <= max_i ; min_i++)
			{
				for (min_j=jd ; min_j <= max_j ; min_j++)
				{
					if (G[min_i][min_j]==nb) candidat=0;
				}
			}
		}
	}
	return candidat; // retour {0,1} --> 1=candidat , 0=pas candidat
}

int initTab(int G[][9], Cand C[][9], Case O[])
{
	int nbr,id,jd,NBO=0;
	for(id=0 ; id<9 ; id++)
	{
		for(jd=0 ; jd<9 ; jd++)
		{
			C[id][jd].nbc=0; // initalisation du nbc à 0
			C[id][jd].tab=(int*)malloc(9*sizeof(int)); // allocation mem dynamique, 9 car 9 candidats possible
			if(G[id][jd]==0) // case ouverte
			{
				O[NBO].x=id;  // on enregistre la position de la case ouverte
				O[NBO].y=jd;
				NBO++;  //  incrementation afin de recueillir le nombre de cases ouvertes
				for(nbr=1 ; nbr<=9 ; nbr++) //  test des valeurs possibles pour le sudoku pour une case donne
				{
					if (estCandidat(G, nbr, id, jd)==1)  // 1 --> candidat
					{
						C[id][jd].tab[C[id][jd].nbc]=nbr; // si candidat inscription du candidat dans la structure
						(C[id][jd].nbc)++;  // incrementation du nombre de candidat du coup
					}
				}
			}
			else
			{
				C[id][jd].nbc=0;  //  si la case est ferme
				C[id][jd].tab=NULL; // NULL car pointeur
			}
		}
	}
   return NBO;
}

void ecrireCand(int G[][9],  Cand C[][9], Case O[], int NBO)
{
  int i, j, id, jd;
  for (i = 0; i < NBO; i++) // pour ecrire chaque case candidat
  {
    id = O[i].x; // recuperation de la posititon de la prochaine case ouverte a traiter
    jd = O[i].y; //
    printf("Case [%d,%d] : ", id, jd);
    for (j = 0; j < C[id][jd].nbc; j++) // j < C[][].nbc afin de parcourir tous les candidats
    {
      printf("%d ",C[id][jd].tab[j]); // inscription de toutes les valeurs candidates pour une case donne
    }
    printf("\n");
  }
}

int admetUnique(int x, int y, int G[][9], Cand C[][9])
{
  int i,j,k,nbcand,supcand, min_x, max_x, min_y, max_y, valcase=0;
  if (G[x][y] == 0) // admet unique est utilisable seulement sur une case ouverte
  {
    i = x; //garder la valeur de x et en faire une copie --> pouvoir comparer plus tard
    while (i%3!=0) i--; //pour trouver le minimum

		min_x=i; //creation des bornes minimum et maximum
		max_x=min_x+2;
    j = y;  // garder la valeur de y et en faire une copie --> pouvoir comparer plus tard

		while (j%3!=0) j--; //trouver le minimum

		min_y=j;  //creation des bornes minimum et maximum
		max_y=min_y+2;

    if (C[x][y].nbc == 1) //le candidat est unique
    {
      valcase = C[x][y].tab[0]; //affectation a la variable valcase de la valeur du candidat unique
		}

		j = 0;

    while(valcase == 0 && j < C[x][y].nbc)// s'il  n'y a pas q'un seul candidat --> chercher unicité sur la ligne, colonne, petit carré
    {
      supcand=0;
      for (i=0; i<9; i++)
      {
        if (G[x][i] == 0 && i!=y)  // different de y pour ne pas tester une case que l'on veut determiner l'unicite
        {
          for (nbcand=0; nbcand < C[x][i].nbc; nbcand++)  //candidat potentielle vont être testé
          {
            if (C[x][y].tab[j]==C[x][i].tab[nbcand]) supcand+=1;  // si candidat est deja present dans la ligne du sudoku de la case a traiter --> supcand=1
          }
        }
      }
      if (supcand == 0)
      {
        valcase = C[x][y].tab[j];
      }

      j++;
    }

    j = 0;  // reinitialisation de j, afin que si valcase toujours =0 on peut continuer sans probleme

    while(valcase == 0 && j < C[x][y].nbc)// s'il  n'y a pas q'un seul candidat --> de l'unicité sur la ligne, colonne, petit carré
    {
      supcand = 0;
      for (i=0; i<9; i++)
      {
        if (G[i][y] == 0 && i!=x)
        {
          for (nbcand=0; nbcand < C[i][y].nbc; nbcand++)
          {
            if (C[x][y].tab[j]==C[i][y].tab[nbcand]) supcand+=1;
          }
        }
      }
      if (supcand == 0)
      {
        valcase = C[x][y].tab[j];
      }

      j++;
    }

    j = 0;

    while(valcase == 0 && j < C[x][y].nbc)// si les valeurs ne sont pas candidats dans le carre
    {
      supcand = 0;
      for (i=min_x; i<=max_x; i++)  //parcours des 3 cases composant le carre
      {
        for (k=min_y; k<=max_y; k++)
        {
          if (G[i][k] == 0 && (i!=x || k!=y))
          {
            for (nbcand=0; nbcand < C[i][k].nbc; nbcand++)
            {
              if (C[x][y].tab[j]==C[i][k].tab[nbcand]) supcand+=1;
            }
          }
        }
      }
      if (supcand == 0)
      {
        valcase = C[x][y].tab[j];
      }

      j++;
    }
  }
  return valcase; //retour de la valeur du candidat unique
}

int fermerCase(int x, int y, int NBO, int G[][9], Cand C[][9], Case *O)
{
  int i=0,j=0,k=0,casecand=0,temp_x=0, temp_y=0,cand=0, min_x=0, max_x=0, min_y=0, max_y=0,boole=0;

  if (admetUnique(x,y,G,C) != 0) // c-à-d une valeur compris entre 1 et 9 pour notre sudoku
  {
    G[x][y] = cand = admetUnique(x,y,G,C);
		C[x][y].nbc = 0;  //pour fermer la grille --> case ferme
		C[x][y].tab = NULL;

    while (boole == 0 && i < NBO)
    {
      temp_x = O[i].x;  //trouver dans le tableau des case ouvertes, la ou se trouve la case
			temp_y = O[i].y; /*qui est maintenant fermé pour pour pouvoir reorganiser le tableau à 1D*/

      if (temp_x == x && temp_y == y)
      {
        boole = 1;
        for (j=i ; j < NBO ; j++)
        {
          O[j].x = O[j+1].x;  //reoarganisation du tableau
          O[j].y = O[j+1].y;
        }
      }
      i++;
    }

    NBO--;  //on enleve une case candidat donc on enleve 1 a NBO
    i = x;  //affectation pour garder le x

    while (i%3!=0) i--; // même principe que pour estCandidat ou admetUnique
    min_x=i;  //minimum
		max_x=min_x+2;  //maximum

		j = y; //affectation pour garder le y

		while (j%3!=0) j--; // même principe que pour estCandidat ou admetUnique
		min_y=j;
		max_y=min_y+2;

    for (i=0; i<9; i++) //elimination des candidats dans la colonne
    {
      boole = 0;  // a vrai si candidat unique trouve la case
			casecand = 0;

      while(boole == 0 && casecand < C[x][i].nbc) //pour la colonne
      {
        if (cand == C[x][i].tab[casecand])  //cand --> candidat unique retourner de la fct admetUnique
        {                                  /*tester pour voir en quel position se trouve ce candidat dans le tableau C*/
          boole = 1;  //--> on l'a donc trouvé
          k = casecand; //reoarganisation du tableau
          while (k < C[x][i].nbc-1) //nbc-1 car pour une case trouvé le .tab et .nbc va être supprimé
          {
            C[x][i].tab[k] = C[x][i].tab[k+1];  //ecrasement des valeurs a partir de la ou on a trouvé un candidat unique
            k++;
          }
          C[x][i].nbc--;
        }
        casecand++;
      }
    }

    for (j=0; j<9; j++)//elimine les candidats pour la ligne
    {
      boole = 0;
			casecand = 0;

      while(boole == 0 && casecand < C[j][y].nbc)
      {
        if (cand == C[j][y].tab[casecand]) //cand --> candidat unique retourner de la fct admetUnique
        {                                /*tester pour voir en quel position se trouve ce candidat dans le tableau C*/
          boole = 1; // --> trouve
          k = casecand; //reoarganisation du tableau
          while (k < C[j][y].nbc-1) //nbc-1 car pour une case trouvé le .tab et .nbc va être supprimé
          {
            C[j][y].tab[k] = C[j][y].tab[k+1];  //ecrasement des valeurs a partir de la ou on a trouvé un candidat unique
            k++;
          }
          C[j][y].nbc--;
        }
        casecand++;
      }
    }

    for (i=min_x; i<=max_x; i++)  // eliminer les candidat pour le petit carre
    {
      for (j=min_y; j<=max_y; j++)
      {
        boole = 0;
				casecand = 0;

				while(boole == 0 && casecand < C[i][j].nbc)
        {
          if (cand == C[i][j].tab[casecand])
          {
            boole = 1;
            k = casecand;
            while (k < C[i][j].nbc-1)
            {
              C[i][j].tab[k] = C[i][j].tab[k+1];
              k++;
            }
            C[i][j].nbc--;
					}
          casecand++;
        }
      }
    }
  }
  return NBO;
}

int fermerGrille(int NBO, int G[][9], Cand C[][9], Case *O)
{
    int i=0, NBO_1 = NBO, NBO_2 = NBO; //garde la valeur initiale de NBO
    do{
      NBO = NBO_1 = NBO_2;
      i = 0;
      do{
        NBO_2 = fermerCase(O[i].x, O[i].y, NBO_2,G,C,O); //chaque case candidates vont être fermé grâce à i qui s'incéremente
        if(NBO_2==NBO_1) i++;
        else NBO_1 = NBO_2;

      }while(i<NBO_2);  //exemple pour la grille quand i=46 sa sortira de la boucle while

    }while(NBO_2 > 0 && NBO_2!= NBO); //NBO_2 qui lorsque fermerCase s'executera diminuera

return NBO_2;

}

void grilleDifficile(int G[][9],  Cand C[][9], Case O[], int NBO)
{
  if (NBO != 0)
  {
    printf("\nLa grille n'est pas totalement complete, on peut voir qu'il reste les candidats suivant :\n\n");
    ecrireCand(G,C,O,NBO);
  }
}
