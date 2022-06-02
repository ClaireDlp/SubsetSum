#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define WORD_SIZE 32 // valeur de n

typedef short bool;
#define true 1
#define false 0

//stocker des chaînes de n bits.
typedef long unsigned word;

bool choixDeA(){
    short rep = 1;
    printf("Que souhaitez vous pour a :\n\t1: le générer aléatoirement\n\t2: le générer manuellement\n");
    scanf("%hd",&rep);
    if(rep == true){
        return true;
    }
    else{
        return false;
    }
}

// int choixDeN(){
//     int n = 8;
//     printf("\nCombien d'éléments dans a souhaitez vous ? Attention la valeur saisie doit être une puissance de 2 et >= 8\n");
//     scanf("%d",&n);
//     if(round(log2(n)) != log2(n) && n >=8){ //8 ou 4 ? 
//         return choixDeN();
//     }
//     return n;
// }

void afficherA(word* ai){
    printf("\na = (");
    for(int i = 0; i<WORD_SIZE-1; ++i){
        printf("%lu,",ai[i]);
    }
    printf("%lu)",ai[WORD_SIZE-1]);
}

void generationAleaA(word* ai){
    srand(time(NULL));
    //int n = choixDeN();
    ai = (word*) malloc(sizeof(word)*WORD_SIZE);

    for(word i = 0; i<WORD_SIZE; ++i){
        //%(max-min+1) + min;
        ai[i] = rand()%(word)(1ULL<<WORD_SIZE);
    }
    afficherA(ai);
}

void Algo1(int* ai, int s, int* T1S, int* T2S, int* T3S, int* T4S, int* T1x, int* T2x, int* T3x, int* T4x, int tailleTableau){
    for(int i = 0; i<WORD_SIZE/4; ++i){

    }
}

int main(){
    word* ai = NULL;
    bool choixDefDeA = true;

    choixDefDeA = choixDeA();
    if(choixDefDeA){
        generationAleaA(ai);
    }
    else{
        //generationManuel();
    }

    //ALORITHME 1
    word* xi = NULL;
    
    word tailleTableauS = WORD_SIZE/4;
    word tailleTableaux = (1<<(tailleTableauS));

    word T1S[tailleTableauS];
    word T2S[tailleTableauS];
    word T3S[tailleTableauS];
    word T4S[tailleTableauS];

    word T1x[tailleTableaux];
    word T2x[tailleTableaux];
    word T3x[tailleTableaux];
    word T4x[tailleTableaux];


}
