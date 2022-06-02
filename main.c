#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

typedef short bool;
#define true 1
#define false 0

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

int choixDeN(){
    int n = 8;
    printf("\nCombien d'éléments dans a souhaitez vous ? Attention la valeur saisie doit être une puissance de 2 et >= 8\n");
    scanf("%d",&n);
    if(round(log2(n)) != log2(n) && n >=8){ //8 ou 4 ? 
        return choixDeN();
    }
    return n;
}

void afficherA(int* ai, int n){
    printf("\na = (");
    for(int i = 0; i<n-1; ++i){
        printf("%d,",ai[i]);
    }
    printf("%d)",ai[n-1]);
}

int generationAleaA(int* ai){
    srand(time(NULL));
    int n = choixDeN();

    ai = (int*) malloc(sizeof(int)*n);

    for(int i = 0; i<n; ++i){
        //%(max-min+1) + min;
        ai[i] = rand()%16;//(INT64_MAX);
    }
    afficherA(ai,n);
    return n;
}


int puissance(int a, int b){
    if(b==0){
        return 1;
    }
    else{
        return a * puissance(a, b-1);
    }
}

void Algo1(int* ai, int s, int* T1S, int* T2S, int* T3S, int* T4S, int* T1x, int* T2x, int* T3x, int* T4x, int tailleTableau, int n){
    for(int i = 0; i<n/4; ++i){

    }
}

int main(){
    int* ai = NULL;
    bool choixDefDeA = true;
    int n;

    choixDefDeA = choixDeA();
    if(choixDefDeA){
        n = generationAleaA(ai);
    }
    else{
        //generationManuel();
    }

    //ALORITHME 1
    int* xi = NULL;
    
    int tailleTableau = puissance(2,n)/4;

    int T1S[tailleTableau];
    int T2S[tailleTableau];

    int T3S[tailleTableau];
    int T4S[tailleTableau];

    int T1x[tailleTableau*2];
    int T2x[tailleTableau*2];
    int T3x[tailleTableau*2];
    int T4x[tailleTableau*2];


}
