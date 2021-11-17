/* auteur : Xu Lecointre
Creer le : 16-03-2020 
modifier le : 23-03-2020
*/

#ifndef __LABY__
#define __LABY__

#define PERE 0
#define RANG 1
#define MAX 1000
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <MLV/MLV_all.h>


typedef struct {
	int i;
	int j;
}coord, *Coord;

typedef struct {
	int mur_est;
	int mur_sud;
	Coord pere;
	int rang;
	/*int taille_total;*/
} casee, *Case;

typedef struct {
	int n;
	int m;
	Case cases[MAX][MAX];
}laby, *Labyrinthe;

/* Créé un labyrinthe de taille donné, toutes les cases sont leur propre pere 
PARAMETRE:
n : hauteur du labyrinthe
m : largeur du labyrinthe

*/
Labyrinthe cree_laby(int n, int m);

/* teste si l'entrée et la sortie du labyrinthe (en haut à gauche et en bas à droite) sont reliés 
PARAMETRE:
lab : teste le labyrinthe

return 1 si le labyrinthe est valide , 0 sinon
*/

int laby_valide(Labyrinthe lab);

/* Créer une case de coordonnées données
PARAMETRE:
i : ligne de la case
j : colonne de la case

return la case.
 */

Case cree_case(int i, int j);

/* Créer une structure avec le couple de coordonnées données
PARAMETRE:
i : ligne 
j : colonne

return le coordonnée
*/

Coord cree_coord(int i, int j);

/* Détruit le mur donné
PARAMETRE:
lab : teste sur le labyrinthe
i : la ligne du labyrinthe
j : la colonne du labyrinthe
sud : SUD si on le donne 1 , EST pour 0
 */

int enleve_mur (Labyrinthe lab, int i , int j, char sud, char unique);

/* Cherche le représentant de l'ensemble d'une case d'un labyrinthe
et le définit comme celui des autres cases de l'ensemble
PARAMETRE
lab : teste sur le labyrinthe
i :  ligne
j : colonne
unique : 1 pour rendre le labyrinthe unique , 0 sinon
return : coordonnee du pere de la case
 */

Coord trouve_compresse(Labyrinthe lab, int i, int j);

/* Fusionne deux ensembles de cases selon leur rang
PARAMETRE:
lab : test sur le labyrinthe contenant les ensembles
fils1 : case contenant le premier ensemble
fils2 : case contenant le second ensemble
 */

void fusion_rang(Labyrinthe lab, Coord fils1, Coord fils2);



#endif
