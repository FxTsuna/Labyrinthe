
#include "mur.h"
#define DEBUG 0
//je viens de comprendre que mon programme ne fait qu'envoyer des nombres aléatoires différents les uns des autres en boucle mais il le fait presque bien et c'est déjà ça 

void affbit(char a){
    char i;
    for(i=0;i<8;i++){printf("%d",a>>i&1);}
    printf("\n");
}

int set(wall a,char bit_to_change){//supprime le mur numéro bit_to_change,renvoie sa position
    char cmp=0;
    unsigned char tmp=a->a;
    while(TRUE){
        if(tmp&0x01){
            if(bit_to_change==0)break;
            bit_to_change--;
	    //if(bit_to_change<0)break;/if(bit_to_change&0x80)   je ne sais pas lequel irait plus vite ? une décrementation est plus lourde que la différence entre ++0 et <0 ?=> on évite un parcourt entier avec un seul bit mais la décrémenation apporte un parcourt entier ?
        }
        tmp=tmp>>1;
        cmp++;
    }
    a->a-=(1<<cmp);//modification du bit
    a->nb_bit_on--;//un mur en moins 
    return cmp;
}

int delete_block(wall ptr,unsigned int number){//détruit le mur numéro number d'un block mémoire
    unsigned int cmp=0;
    while(number>=ptr->nb_bit_on){
        number-=ptr->nb_bit_on;
        ptr++;
        cmp++;
    }  
    return 8*cmp+set(ptr,number);
}

int delete_wall(lst_wall walmart,unsigned int number){//détruit le mur numéro number
    block *ptr=walmart->tab;// /!\ tout doit être défini
    unsigned int cmp=0;
    number=number%walmart->murs_restants;//on s'assure de ne pas dépasser le nombre de murs
    while(number>=(*ptr)->murs_restants){//avoid underflow
        cmp++;
        number-=(*ptr)->murs_restants;
        ptr++;
    }
    (*ptr)->murs_restants--;
    walmart->murs_restants--;
    return cmp*8*BLOCK+delete_block((*ptr)->tab,number);
}

void alloc_stock(char number,wall tmp){
    tmp->a=0xff;
    tmp->nb_bit_on=number;
}

block alloc_block(unsigned int number){
    block tmp;
    int i;
    if((tmp=(block)malloc(sizeof(__block__)))==0)return 0;
    tmp->murs_restants=number;
    for(i=0;i<BLOCK;i++){
        if(number>7){
            alloc_stock(8,tmp->tab+i);
            number-=8;
        }
        else{
            alloc_stock(number,tmp->tab+i);
            break;
        }
    }
    return tmp;
}

block alloc_block_entier(void){//alloue des blocks entiers (optimisation temps de calcul)
    block tmp;
    int i;
    if((tmp=(block)malloc(sizeof(__block__)))==0)return 0;
    for(i=0;i<BLOCK;i++){
        alloc_stock(8,tmp->tab+i);
    }
    tmp->murs_restants=BLOCK*8;
    return tmp;
}
void free_lst_wall(lst_wall a){ //modification de l'initialisation
    unsigned int wall_nb=2*(a->n)*(a->m)-(a->n)-(a->m);
    unsigned int nb_block=(wall_nb-1)/(BLOCK*8)+1; 
    unsigned int cmp;
    for(cmp=0;cmp<nb_block;cmp++){
        free(a->tab[cmp]);
    }
    free(a->tab);
    free(a);
}

lst_wall init_wall(unsigned int n,unsigned int m){//le labyrinthe fera au moins 3*3 aucune garantie de fonctionnement en dessous pour l'affichage
    lst_wall tmp;
    unsigned int wall_nb=2*n*m-n-m;
    unsigned int nb_block=(wall_nb-1)/(BLOCK*8)+1;           //arrondit au supérieur
    unsigned int cmp;
    if((tmp=(lst_wall)malloc(sizeof(__lst_wall__)))==0)return 0;
    tmp->n=n;
    tmp->m=m;
    tmp->ligne=n*2-1;
    tmp->murs_restants=wall_nb;
    if((tmp->tab=(block *)malloc(sizeof(block)*nb_block))==0)return 0;//tableau de pointeur
    nb_block--;
    for(cmp=0;cmp<nb_block;cmp++){//le dernier peut contenir moins de bit codants
        tmp->tab[cmp]=alloc_block_entier();
    }
    wall_nb-=(cmp)*8*BLOCK;//pour avoir le nombre restant de murs à placer
    tmp->tab[cmp]=alloc_block(wall_nb);
    return tmp;
}

char vh_pos(unsigned int nb,unsigned int ligne,unsigned int m,unsigned int *res){//TRUE si vertical false sinon ,ligne nombre mur par ligne m le nombre de lignes 
	unsigned int a;
	if((a=nb/ligne)!=m-1){//on est pas sur la dernière colonne
		*res=nb+a>>1;		
		return nb&1^a&1;
		}
	a=((m-1)*ligne);//nb virtuel maximum avant la dernière ligne
	nb-=a;//reste
	*res=(a+m-1>>1)+nb;
	return 1;
	}

unsigned char lecture(lst_wall a){//donne l'état d'un mur et avance de 1 unité : 1 si il a un mur 0 sinon
	pos_wall *tmp=&a->pos;
	if (tmp->bit++==7){
		tmp->bit=0;
		tmp->stock++;
		}
	if (tmp->stock==BLOCK){
		tmp->stock=0;
		tmp->block++;
		}
	if(DEBUG)
	printf("%d", a->tab[tmp->block]->tab[tmp->stock].a&(1<<tmp->bit)?1:0);
	return a->tab[tmp->block]->tab[tmp->stock].a&(1<<tmp->bit)?1:0;

	}

void delete_wall_strict(lst_wall walmart,unsigned int number){//détruit le mur à la posiiton number
    block *ptr=walmart->tab;// /!\ tout doit être défini
    wall ptr2;
    unsigned int cmp=0;
    while(number>=(BLOCK*8)){//avoid underflow
        cmp++;
        number-=(BLOCK*8);
        ptr++;
    }
    cmp=0;
    ptr2=(*ptr)->tab;
    while(number>=8){
        number-=8;
        ptr2++;
        cmp++;
    }  
    (ptr2->a)-=(1<<number);
}