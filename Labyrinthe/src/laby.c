
/* auteur : Xu Lecointre
Creer le : 16-03-2020 
modifier le : 23-03-2020
*/

#include "laby.h"


Labyrinthe cree_laby(int n, int m){
	Labyrinthe lab;
	int i, j;

	lab = (Labyrinthe)malloc(sizeof(laby));
	if (lab == NULL)
		return NULL;

	lab->n = n;
	lab->m = m;

	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			lab->cases[i][j] = cree_case(i, j);
			if (lab->cases[i][j] == NULL)
				return NULL;
			if (i == n-1) 
				lab->cases[i][j]->mur_sud = 0; /* mur extérieur bas */

		}
		lab->cases[i][j-1]->mur_est = 0; /* mur extérieur droite*/
	}
	return lab;
}

Coord cree_coord(int i, int j){
	Coord co;

	co = (Coord)malloc(sizeof(coord));
	if (co == NULL)
		return NULL;

	co->i = i;
	co->j = j;

	return co;

}

Case cree_case(int i, int j){
	Case cas;
	Coord co;

	cas = (Case)malloc(sizeof(casee));
	if (cas == NULL)
		return NULL;

	cas->mur_sud = 1;
	cas->mur_est = 1;
	cas->rang = 1;
	/*cas->taille_total = 1;*/
	co = cree_coord(i, j);
	if(co == NULL)
		return NULL;
	cas-> pere = co;

	return cas;
}



int laby_valide(Labyrinthe lab){

	return trouve_compresse(lab,0,0) == trouve_compresse(lab,lab->n-1,lab->m-1);
}

int enleve_mur (Labyrinthe lab, int i , int j, char sud, char unique){ /* sud = 1 , est = 0   */
	
	if (sud == 1){
		if (unique && i + 1 < lab->n && trouve_compresse(lab, i, j) == trouve_compresse(lab, i+1, j))
			lab->cases[i][j]->mur_sud = 0;
			fusion_rang(lab, lab->cases[i][j]->pere, lab->cases[i+1][j]->pere);
			return 1;
		if (unique == 0)
			lab->cases[i][j]->mur_sud = 0;
			fusion_rang(lab, lab->cases[i][j]->pere, lab->cases[i+1][j]->pere);
			return 1;
	}
	else {
		if (unique && j+1 < lab->m && trouve_compresse(lab, i, j) == trouve_compresse(lab,i , j+1))
			lab->cases[i][j]->mur_est = 0;
			fusion_rang(lab, lab->cases[i][j]->pere, lab->cases[i][j+1]->pere);
			return 1;
		if (unique == 0)
			lab->cases[i][j]->mur_sud = 0;
			fusion_rang(lab, lab->cases[i][j]->pere, lab->cases[i+1][j]->pere);
			return 1;
	}
	return 0;

} 


Coord trouve_compresse(Labyrinthe lab, int i, int j){
	Case fils;

	fils = lab->cases[i][j];
	if (fils == lab->cases[fils->pere->i][fils->pere->j])
		return fils->pere;
	else{
		lab->cases[i][j]->pere = trouve_compresse(lab, fils->pere->i, fils->pere->j);
		return lab->cases[i][j]->pere;
	}
}

void fusion_rang(Labyrinthe lab, Coord fils1, Coord fils2){
	Coord pere1, pere2;

	pere1 = trouve_compresse((lab), fils1->i, fils1->j);
	pere2 = trouve_compresse((lab), fils2->i, fils2->j);

	if (lab->cases[pere1->i][pere1->j]->rang < lab->cases[pere2->i][pere2->j]->rang){
		lab->cases[pere1->i][pere1->j]->pere = pere2;
		lab->cases[fils1->i][fils1->j]->pere = pere2;
	}else{
		lab->cases[pere2->i][pere2->j]->pere = pere1;
		lab->cases[fils2->i][fils2->j]->pere = pere1;

		if (lab->cases[pere1->i][pere1->j]->rang == lab->cases[pere2->i][pere2->j]->rang)
			lab->cases[pere1->i][pere1->j]->rang +=1;
	}
}	

