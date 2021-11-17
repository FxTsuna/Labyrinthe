#ifndef __MUR__
#define __MUR__

#include <stdlib.h>
#include <stdio.h>
#define TRUE 1
#define BLOCK 128 /*en octets*/
/*attention dépasser un labyrinthe trop grand provoquera nécessairement des erreurs (unsigned int) et dépenser une mémoire extrèmement importante
note si RAND_MAX vaut 32 767 un si grand labyrinthe demanderait une fonction à part pour être aléatoire avec la méthode de base on conseilerait donc une taille d'au plus 182*182 (bien que ce soit amusant de voir son pc mourir en tentant du 60k*60k) */
typedef struct _pos_wall{
    unsigned int block;
    unsigned int stock;
    char bit;
    }pos_wall;

typedef struct stock{
    unsigned char a;
    char nb_bit_on;
}*wall,__wall__;

typedef struct _block{
    unsigned int murs_restants;
    __wall__ tab[BLOCK];
}*block,__block__;

typedef struct _lst_wall{
    unsigned int n;
    unsigned int m;
    block *tab;/* tableau de pointeurs vers des plus petits tableaux de taille BLOCK contenant les murs*/
    int ligne;
    unsigned int murs_restants;
    pos_wall pos;
}*lst_wall,__lst_wall__;

static char* intersections[2][2][2][2] = 
   { {{{" ","╶"},{"╴","─"}},{{"╵","└"},{"┘","┴"}}},
     {{{"╷","┌"},{"┐","┬"}},{{"│","├"},{"┤","┼"}}} };

/*affiche a bit à bit parce que pourquoi pas*/

void affbit(char a);

/*initialise la liste de mur pour un labyrinthe de taille de n*m*/

lst_wall init_wall(unsigned int n,unsigned int m);

/*cherche dans la liste de mur le mur numéro number supprime le et renvoie sa position */

int delete_wall(lst_wall walmart,unsigned int number);

/*détruit le mur à la position pos_wall sans prendre en compte les murs détruits  */

void delete_wall_strict(lst_wall affichage_alt,unsigned int pos_wall);

/*free les malloc utilisés par le module */
void free_lst_wall(lst_wall a);

/*prend la position d'un mur et renvoie en sortie standard si il est vertical ou horizontal dans res son numéro de case
prend m le nombre de colonnes ligne le nombre de murs par lignes(stocké dans les lst_wall) */

char vh_pos(unsigned int nb,unsigned int ligne,unsigned int m,unsigned int *res);

/* affiche 0 si il n'y a pas de mur 1 si il y en a un à la position courante et avance cette position */

unsigned char lecture(lst_wall a);



#endif
