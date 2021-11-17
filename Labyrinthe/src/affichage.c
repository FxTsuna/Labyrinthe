
/* auteur : Xu Lecointre
Creer le : 16-03-2020 
modifier le : 23-03-2020
*/


#include "affichage.h"
#include <X11/Xlib.h>


/*void affiche_laby_mlv(lst_wall a,int size,int x_init,int y_init){

	int x=x_init;
	int y=y_init;
	unsigned int n=a->n*(size+2);
	unsigned int m=a->m;
	a->pos.block=0;
    a->pos.stock=0;
    a->pos.bit=-1;

	MLV_create_window("Labyrinthe", "MLV", LARGEUR, HAUTEUR );


	for(;x<n;x+=(size+1))			//haut
		MLV_draw_line(x, y, x+size, y, MLV_COLOR_GREEN);	//
	y += (size+1);
	x=x_init;
	while(--m){
		n=a->ligne;
		n--;
		if(lecture(a))				//
			MLV_draw_line(x, y, x+size, y, MLV_COLOR_GREEN);	//gauche
		x+=size;
		do{
			if(lecture(a))
				MLV_draw_line(x, y, x, y-size, MLV_COLOR_GREEN);
			x++;
			if(lecture(a))
				MLV_draw_line(x, y, x+size, y, MLV_COLOR_GREEN);
			x+=size;
			}while(n -= 2);
		MLV_draw_line(x, y, x, y-size, MLV_COLOR_GREEN);
		x = x_init;
		y += (size + 1);
		MLV_draw_line(x, y, x, y-size, MLV_COLOR_GREEN);	//droite
		}
	n = a->n;
	MLV_draw_line(x, y, x+size, y, MLV_COLOR_GREEN);
	x += size;

	for(m = 1; m < n; m++){
		if(lecture(a))	{	//
			MLV_draw_line(x, y, x, y-size, MLV_COLOR_GREEN);
			MLV_draw_line(x, y, x+size, y, MLV_COLOR_GREEN);	//
			x += size+1;
		}else			//bas
			MLV_draw_line(x, y, x+size+1, y, MLV_COLOR_GREEN);	//
		}
	}*/


void aff_wall_base(lst_wall a){
	unsigned int n;
	unsigned int m=a->m;
	a->pos.block=0;
    	a->pos.stock=0;
    	a->pos.bit=-1;
	printf(".");		//
	for(n=0;n<a->n;n++)	//haut
		printf("_.");	//
	printf("\n.");
	while(--m){
		n=a->ligne;
		n--;
		if(lecture(a))		//
			printf("_");	//
		else			//gauche
			printf(" ");	//
		do{
			if(lecture(a))
				printf("|");
			else
				printf(".");
			if(lecture(a))
				printf("_");
			else
				printf(" ");
			}while(n-=2);
		
		printf("|\n|");	//droite
		}
	n=a->n;
	printf("_");
	for(m=1;m<n;m++){
		if(lecture(a))		//
			printf("|_");	//
		else			//bas
			printf("._");	//
		}
	printf("\n");
	}


char aff_wall(lst_wall a){
	void fill(lst_wall a,unsigned char *buffer,unsigned int size){
		int i;
		unsigned char *b=buffer;
		for(i=0;i<size;i++,b++){
			*b=lecture(a);
			}
		if(DEBUG)
		printf("\n");
		}
	void draw_line(unsigned char *b1,unsigned char *b2,unsigned int size){
		unsigned int i=0;
		size--;//on ne veut pas tenter de dessiner la bordure droite dans la boucle
		printf("%s",intersections[1][1][0][ b1[i] ]);//bordure gauche
		while(i++!=size){
			printf("%s",intersections[ b2[i] ][ b1[i] ][ b1[i-1] ][ b1[++i] ]);
			}
		printf("%s\n",intersections[ 1 ][ 1 ][ b1[i-1] ][0]); //dessin de la bordure droite
		}

	unsigned int n=a->ligne;
	unsigned int m=0;
	unsigned char *buffer1;
	unsigned char *buffer2;
	unsigned char *tmp;//échanger les pointeurs
	if((buffer1=(unsigned char*)malloc(sizeof(char)*n))==0)return 0;
	if((buffer2=(unsigned char*)malloc(sizeof(char)*n))==0)return 0;
	a->pos.block=0;//réinitialisation des pointeurs
    	a->pos.stock=0;
    	a->pos.bit=-1;
	fill(a,buffer1,n);
	// dessin de la bordure haut

	n--;//on ne veut pas tenter de dessiner la bordure droite dans la boucle
	printf("%s",intersections[0][0][1][1]);//bordure gauche
	while(m++!=n){
		printf("%s",intersections[ buffer1[m] ][0][1][1]);
		m++;
		}
	printf("%s\n",intersections[1][0][1][0]); //dessin de la bordure droite
	n++;
	//
	m=a->m;
	if(m!=2){
		fill(a,buffer2,n);
		m-=2;//le dernier buffer est moins plein 
		while(--m){
			draw_line(buffer1,buffer2,n);
			fill(a,buffer1,n);
			tmp=buffer1;
			buffer1=buffer2;
			buffer2=tmp;
			}
		draw_line(buffer1,buffer2,n);
		n--;
	}else{buffer2=buffer1;m=0;n--;}
	while(m<n){//on remplie le dernier buffer
	m++;//:buffer1[m++]=0; cette valeur ne sera jamais lu
	buffer1[m++]=lecture(a);
	}
	n++;
	buffer1[m]=0;
	tmp=buffer1;
	buffer1=buffer2;
	buffer2=tmp;
	draw_line(buffer1,buffer2,n);
	printf("%s",intersections[0][1][0][1]);	//on dessine la dernière ligne
	m=0;
	//bordure basse finale
	while(++m!=n){
		printf("%s",intersections[0][ buffer2[m] ][1][1]);
		m++;
		}
	printf("%s\n",intersections[0][0][1][1]);//bordure finale :on veut un trou à droit -> on a juste besoin d'une barre
	return 1;
	}

MLV_du_pauvre init_MLV(unsigned int x,unsigned int y){
	MLV_du_pauvre tmp;
	int fenetre_id;
	unsigned long int blanc,noir,fenetre,pere;
	XEvent ev;
	Display *server;
	GC contexte;
	XGCValues style;
	if((server=XOpenDisplay(0))==0)return 0;
	pere=DefaultRootWindow(server);
	fenetre_id=DefaultScreen(server);
	noir=BlackPixel(server,fenetre_id);
	blanc=WhitePixel(server,fenetre_id);
	fenetre=XCreateSimpleWindow(server,pere,0,0,x,y,1,noir,noir);
	style.foreground=blanc;
	style.line_width=1;
	style.line_style= LineSolid;
	contexte=XCreateGC(server,fenetre,GCForeground|GCLineWidth|GCLineStyle,&style);
	XSelectInput(server,fenetre,StructureNotifyMask);
	XMapWindow(server,fenetre);
	if ((tmp=(MLV_du_pauvre)malloc(sizeof(__MLV_du_pauvre__)))==0)return 0;
	tmp->x=x;
	tmp->y=y;
	tmp->fenetre=fenetre;
	tmp->fenetre_id=fenetre_id;
	tmp->server=server;
	tmp->contexte=contexte;
	tmp->blanc=blanc;
	tmp->noir=noir;
	while(1){
		XNextEvent(server,&ev);
		if(ev.type == MapNotify)break;
		}
	return tmp;
}

void libere_MLV_pauvre(MLV_du_pauvre MLV){
	if(MLV==NULL)return;
	XFree(MLV->server);
	XFree(MLV->contexte);
	free(MLV);
	}

void drawline(MLV_du_pauvre MLV,unsigned int x,unsigned int y,unsigned int x1,unsigned int y1){
	XDrawLine(MLV->server,MLV->fenetre,MLV->contexte,x,y,x1,y1);
	}
char update(MLV_du_pauvre MLV){
	XEvent ev;
	if(XNextEvent(MLV->server,&ev)==LeaveNotify)return 1;
	XFlush(MLV->server);
	return 0;
	}

void aff_wall_MLV(lst_wall a,MLV_du_pauvre MLV,int size,int x_init,int y_init){
	int x=x_init;
	int y=y_init;
	unsigned int n=a->n*(size+1);
	unsigned int m=a->m;
	a->pos.block=0;
    	a->pos.stock=0;
    	a->pos.bit=-1;
						//
	for(;x<n;x+=(size+1))			//haut
		drawline(MLV,x,y,x+size,y);	//
	y+=(size+1);
	x=x_init;
	while(--m){
		n=a->ligne;
		n--;
		if(lecture(a))				//
			drawline(MLV,x,y,x+size,y);	//gauche
		x+=size;
		do{
			if(lecture(a))
				drawline(MLV,x,y,x,y-size);
			x++;
			if(lecture(a))
				drawline(MLV,x,y,x+size,y);
			x+=size;
			}while(n-=2);
		drawline(MLV,x,y,x,y-size);
		x=x_init;
		y+=(size+1);
		drawline(MLV,x,y,x,y-size);	//droite
		}
	n=a->n;
	drawline(MLV,x,y,x+size,y);
	x+=size;
	for(m=1;m<n;m++){
		if(lecture(a)){	//
			drawline(MLV,x,y,x,y-size);
			drawline(MLV,x,y,x+size+1,y);	//
			x+=size+1;
		}else	{		//bas
			drawline(MLV,x,y,x+size+1,y);	//
			x+=(size+1);
		}
	}
}
