#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>



//Représentation d'un mot
typedef mpz_t word;

//valeur de la taille de n
extern int WORD_SIZE;

typedef short bool;
#define true 1
#define false 0

//Définition d'un type permettant de retourner plusieurs informations à la suite de l'utilisation de la fonction gray
typedef struct{ 
    unsigned long long n; // Représente le code de gray actuel
    unsigned long long nPrec; // Représente le code de gray antérieur à l'appel de la fonction gray
    //CHANGEMENT : short suffit pour les deux variables qui précédent
    unsigned long long bitChangement; // Précise l'emplacement du bit modifié
    unsigned long long signe; // Précise s'il y a eu un retrait ou ajout de bit à 1
} graytab;

//Définition du type pair : Utile pour stocker en pair un mot et son indexe dans son tableau
typedef struct
{
    word word;
    unsigned long long indexe;
}pair;

//Définition du type triple : Utile pour stocker en pair un mot, et un couple d'indexe représentant 
//l'emplacement dans leur deux tableaux respectives des deux valeurs somme du mot
typedef struct
{
    word word;
    unsigned long long i;
    unsigned long long j;
}triple;

//Définition du type solution : stocke les 4 indices représentant 
//l'emplacement dans leur tableaux respectives des valeurs somme du mot
typedef struct{
    unsigned long long i;
    unsigned long long j;
    unsigned long long k;
    unsigned long long l;
} solution;

//Définition des différents type de liste

typedef struct cellule2{
    solution valeur;
    struct cellule2* suivant;
} *ListeSol;

typedef struct cellule3{
    word valeur;
    struct cellule3* suivant;
} *ListeSolConca;

//Signatures des fonctions

//gray.c
unsigned long long nombreDe0gauche(unsigned long long);
graytab gray(unsigned long long, graytab);

//free.c
void liberation(word*, unsigned long long);
void liberationPair(pair*, unsigned long long);
void liberer(word*, word*, word*, word*, word*, word*, ListeSolConca, unsigned long long);

//verif.c
int verificationSol(word*, word, word);

//modular_merge.c
unsigned long long inverse(unsigned long long, int);
void triParDenombrement(word*, pair*, pair*, unsigned long long, unsigned long long);
triple* join(triple*, word*, word*, pair*, unsigned long long, unsigned long long, unsigned long long, unsigned long long*, unsigned long long*);
ListeSol solver(ListeSol, triple*, word*, word*, pair*, unsigned long long, word, unsigned long long, unsigned long long, unsigned long long*, word*);
int chargement(unsigned long long, unsigned long long, int*);
ListeSol Modular_merge(word*, word*, word*, word*, word, ListeSol, word*);
unsigned long long dichotomie(unsigned long long, pair*, unsigned long long);
unsigned long long dichotomie2(unsigned long long, triple*, unsigned long long);

//liste.c
ListeSol AjouterListeSol(solution, ListeSol);
ListeSolConca AjouterListeSolConca(word, ListeSolConca);

//Schroeppel_Shamir.c
void CreationT(word*, word*, word*, unsigned long long);
ListeSolConca concatenation(unsigned long long, unsigned long long, unsigned long long, unsigned long long, unsigned long long, ListeSolConca);
ListeSolConca Schroeppel_Shamir(word*, word, word*, word*, word*, word*, word*, ListeSolConca);

//parametre.c
bool choixDeA();
void afficherA(word*);
void generationAleaA(word**);
int choixFichier(FILE*,char*);
void generationManuelle(word**);
