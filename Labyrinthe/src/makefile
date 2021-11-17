CC=gcc
CCFLAGS=-Wall -ansi -pedantic
LDFLAGS=-lX11
OBJ=main.o mur.o laby.o affichage.o

labyrinthe: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

main.o: main.c mur.h laby.h affichage.h
mur.o: mur.c mur.h affichage.h
laby.o: laby.c laby.h
affichage.o: affichage.c affichage.h mur.h


%.o: %.c
	$(CC) -c $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o

rpropre : clean
	rm -f Projet
