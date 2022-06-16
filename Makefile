OBJ = main.o gray.o free.o verif.o modular_merge.o liste.o Schroeppel_Shamir.o parametre.o
BIBLIO = -lm -lgmp

prog : $(OBJ)
	gcc $(OBJ) -o start $(BIBLIO) 
	# Pour utiliser le programme saisir la commande : ./start

main.o : main.c
	gcc -c main.c

gray.o : gray.c word.h
	gcc -c gray.c

free.o : free.c word.h
	gcc -c free.c

verif.o : verif.c word.h
	gcc -c verif.c

modular_merge.o : modular_merge.c word.h
	gcc -c modular_merge.c

liste.o : liste.c word.h
	gcc -c liste.c

Schroeppel_Shamir.o : Schroeppel_Shamir.c word.h
	gcc -c Schroeppel_Shamir.c

parametre.o : parametre.c word.h
	gcc -c parametre.c

clean :
	rm -f *.o
