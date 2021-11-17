/* auteur : Xu Lecointre
Creer le : 16-03-2020 
modifier le : 23-03-2020
*/

#ifndef __AFFICHAGE__
#define __AFFICHAGE__

#include <X11/Xlib.h>
#include "mur.h"
//#include <MLV/MLV_all.h>
#include <stdlib.h>
#include <stdio.h>
#define LARGEUR 1080
#define HAUTEUR 1080
#define DEBUG 0

/* Affiche le labyrinthe grâce à libMLV
PARAMETRE:
lst_wall : connaitre tous les murs
size : taille des traits
x_init : point de départ du dessin en x
y_init : point de départ du dessin en y
 */
typedef struct {
	unsigned int x;
	unsigned int y;
	int fenetre;
	int fenetre_id;
	Display *server;
	GC contexte;
	XGCValues style;
	unsigned long blanc;
	unsigned long noir;
	}__MLV_du_pauvre__,*MLV_du_pauvre;

MLV_du_pauvre init_MLV(unsigned int x,unsigned int y);
void libere_MLV_pauvre(MLV_du_pauvre MLV);
void drawline(MLV_du_pauvre MLV,unsigned int x,unsigned int y,unsigned int x1,unsigned int y1);
char update(MLV_du_pauvre MLV);
void aff_wall_MLV(lst_wall a,MLV_du_pauvre MLV,int size,int x_init,int y_init);
void affiche_laby_mlv(lst_wall a,int size,int x_init,int y_init);

/*affiche le labyrinthe en ascii*/

void aff_wall_base(lst_wall a);

/*affiche le labyrinthe en ascii extended (attention demande des buffers) */

char aff_wall(lst_wall a);


#endif
