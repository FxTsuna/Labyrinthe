#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "mur.h"
#include "laby.h"
#include "affichage.h"
#define PRECIS 1 //1 pour avoir les étapes au lancement 0 sinon

unsigned int min(unsigned int a,unsigned int b){
	return a>b?b:a;
	}
void sig_time(unsigned int a){usleep(a);}

void sig_user(unsigned int a){scanf("%lc",&a);}

void gottagofast(unsigned int a){}

void wbprintf(char *a){
	printf("%s",a);
	fflush(stdout);
	}
char validate(char *a){
	char res=0;
	printf("invalid argument:%s\n",a);
	while(1){
		printf("ignore ? (y/n)\n");
		scanf("%c",&res);
		if(res=='y')return 1;
		if(res=='n')return 0;
		}
	}
char lecture_int(char *a,char nb_chiffre,unsigned int *x,unsigned int *y,unsigned int *z){//renvoie 0 en cas d'échec 1 sinon les valeurs omises seront set à 0 si des arguments en trop sont donnés on considérera que l'utilisateur s'est trompé et on arrêtera le processus
	switch(nb_chiffre){
		case 3:*z=0;
		case 2:*y=0;
		case 1:*x=0;break;
	default : return 0;
	}
	unsigned int *ptr[3]={x,y,z};
	char cpt=0;
	while(cpt<nb_chiffre){
		while(*a<='9'&&*a>='0'){
			*ptr[cpt]=*ptr[cpt]*10+(*a-'0');//les parenthèses ne servent à priori pas
			a++;
			}
		cpt++;
		switch(*a){
			case 0:return cpt==nb_chiffre;
			case ':':;
			case 'x':a++;break;
			default:return 0;
			}
		}
	return 0;
	}

char cmp(char **a,char *b){
	char cpt=0;
	if(*b==0)return 1;
	while(**a){
		if(**a!=*b){
			(*a)-=cpt;
			return 0;
			}
		(*a)++;
		b++;
		if(*b==0){
			return 1;
			}
		cpt++;
		}
	(*a)-=cpt;
	return 0;
	}
char cmp_strict(char *str1,char *str2){
	const char *a=(const char *)str1;
	const char *b=(const char *)str2;
	do{
		if(*a=='\0')return 1;
		*a++;
		*b++;
	}while(*a==*b);
	return 0;
	}

char verify(char *a,char *b,char *boolean,char pos,char nb_value,unsigned int *x,unsigned int *y,unsigned int *z){//0 to leave programme 1 to continue
	if (cmp(&a,b)==0){
		return validate(a);
		}
	
	if(*boolean&1<<pos){
		printf("conflicting value for %s\n",b);
			return 0;
	}	
	if(nb_value&&(lecture_int(a,nb_value,x,y,z)==0)){
		printf("invalid value:%s\n",a);
			return 0;
	}
	*boolean+=1<<pos;
	return 1;
	}

char verify_texte(char *a ,char *b,char *boolean,char pos){
	if(cmp_strict(a,b)){
		if ((*boolean&1<<pos)==0){
			*boolean+=1<<pos;
			}
		return 1;
		}
	return 0;
	}

void main(char argc,char **argv){
	unsigned int n=5;
	unsigned int m=5;
	unsigned int seed;
	unsigned int wait;
	unsigned int position;
	unsigned int pos_wall;
	int test; //à enlever
	char sud=0;
	char boolean=0;
	MLV_du_pauvre MLV=0;
	void (*ptr_wait)(unsigned int);
	lst_wall liste_murs;
	lst_wall affichage_alt=0;	// afichage en cas d'unique
	lst_wall affichage_mur=0;	//gère quel liste sera affichée
	Labyrinthe lab;
	if(argc<2){
		printf("too few argument\n");
		return;
		}
	while(--argc){
		if(*argv[argc]!='-'||*(argv[argc]+1)!='-'){
			if(validate(argv[argc])==0)return;
			continue;
			}
		argv[argc]+=2;
		switch(*argv[argc]){
			case 't':{
				if(verify_texte(argv[argc],"texte=texte",&boolean,3))continue;
				if(verify(argv[argc],"taille=",&boolean,0,2,&n,&m,NULL)==0)return;
				if(m<3||n<3){
					printf("Taille is too small\n");
					return;
					}
				continue;
				}
			case 'g':{
				if(verify(argv[argc],"graine=",&boolean,1,1,&seed,NULL,NULL)==0)return;
				continue;
				}
			case 'a':{
				if(verify_texte(argv[argc],"acces",&boolean,4))continue;
				if(verify(argv[argc],"attente=",&boolean,2,1,&wait,NULL,NULL)==0)return;
				continue;
				}
			case 'u':{
				if(verify_texte(argv[argc],"unique",&boolean,5))continue;
				if(validate(argv[argc]))continue;
				return;
				}
			default:if(validate(argv[argc])==0)return;continue;
			}
		}
		//taille
		if((boolean&1)==0){
			printf("Error expected size of labyrinthe\n");
			return;
		}else{
			if(PRECIS)wbprintf("Initialization labyrinthe wall ");
			if((liste_murs=init_wall(n,m))==0)return;
			if(PRECIS)printf("[OK]\n");
			if(PRECIS)wbprintf("Initialization labyrinthe ");
			if((lab=cree_laby(m,n))==0)return;
			if(PRECIS)printf("[OK]\n");
			}
		//graine
		if((boolean&1<<1)==0){
			if(PRECIS)wbprintf("No seed mode ");
			srand(time(0));
			if(PRECIS)printf("[OK]\n");
		}else{
			if(PRECIS)wbprintf("Seed mode ");
			srand(seed);
			if(PRECIS)printf("[OK]\n");
			}
		//attente
		if((boolean&1<<2)==0){
			if(PRECIS)wbprintf("Standard wait mode ");
			ptr_wait=&sig_user;
			printf("[OK]\n");
		}else{
			
			if(wait){
				if(PRECIS){printf("Wait mode:%d ",wait);fflush(stdout);}
				ptr_wait=&sig_time;
				if(PRECIS)printf("[OK]\n");
				}
			else{
				if(PRECIS)wbprintf("No wait mode ");
				ptr_wait=&gottagofast;
				if(PRECIS)printf("[OK]\n");
				}
				
			}
		//affichage
		if((boolean&1<<3)==0){
			if(PRECIS)wbprintf("Affichage MLV (ne vous inquiètez pas ça va bien se passer) ");
			MLV=init_MLV(600,400);
			if(PRECIS)printf("[OK]\n");
		}else{
			if(PRECIS)wbprintf("Affichage texte ");
			if(PRECIS)printf("[OK]\n");
			}
		//acces
		if((boolean&1<<4)==0){
			if(PRECIS)wbprintf("Mode non acces ");
			if(PRECIS)printf("[OK]\n");
		}else{
			if(PRECIS)wbprintf("Mode acces ");
			if(PRECIS)printf("[OK]\n");
			}
		//unique
		if((boolean&1<<5)==0){
			if(PRECIS)wbprintf("Mode multiple ");
			affichage_mur=liste_murs;
			if(PRECIS)printf("[OK]\n");
		}else{
			if(PRECIS)wbprintf("Mode unique ");
			if((affichage_alt=init_wall(n,m))==0)return;
			affichage_mur=affichage_alt;
			if(PRECIS)printf("[OK]\n");
			}

		//boucle principale
		while(laby_valide(lab)==0){
			(*ptr_wait)(wait);
			if(wait){//selection de l'affichage
				if(boolean&1<<3){
					aff_wall(affichage_mur);
				}else{
					XClearWindow(MLV->server,MLV->fenetre);
					MLV->x=DisplayWidth(MLV->server,MLV->fenetre_id);
					MLV->y=DisplayHeight(MLV->server,MLV->fenetre_id);
					printf("%d\n",MLV->x);
					aff_wall_MLV(affichage_mur,MLV,  min((MLV->x)/n,(MLV->y)/m)/2-1  ,0,0);
					XFlush(MLV->server);
				}
			}

			pos_wall=delete_wall(liste_murs,rand());
			sud=!vh_pos(pos_wall,liste_murs->ligne,m,&position);
			test=enleve_mur(lab,position/n,position%n,sud,boolean&1<<5);

			if( (boolean&1<<5)  &&  (test)  ){//mur a bien été supprimé +mode unique
				delete_wall_strict(affichage_alt,pos_wall);
			}
			
		}
		if(boolean&1<<3){//selection de l'affichage
			aff_wall(affichage_mur);
		}else{
			XClearWindow(MLV->server,MLV->fenetre);
			/*MLV->x=DisplayWidth(MLV->server,MLV->fenetre_id);
			MLV->y=DisplayHeight(MLV->server,MLV->fenetre_id);*/
			aff_wall_MLV(affichage_mur,MLV,  min(600/(n+2),400/(m+2))  ,0,0);
			XFlush(MLV->server);
		}
		if(PRECIS)wbprintf("Free pointers ");
		libere_MLV_pauvre(MLV);
		free(affichage_alt);
		free(liste_murs);
		if(PRECIS)printf("[OK]\n");
		sig_user(pos_wall);//argument inutile mais bon
	}
