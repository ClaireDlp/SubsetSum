#include "word.h"
 
//Fonctions qui permettent de libérer de la mémoire

void liberation(word* w,unsigned long long N){
    for (unsigned long long i = 0; i < N; ++i){
        mpz_clear(w[i]);
    }
    free(w);
}

void liberationPair(pair* w,unsigned long long N){
    for (unsigned long long i = 0; i < N; ++i){
        mpz_clear(w[i].word);
    }
    free(w);
}

void liberer(word* ai,word* T1S,word* T2S,word* T3S,word* T4S,word* tabGray,ListeSolConca Solution,unsigned long long N){
    liberation(ai,N);
    liberation(T1S,N);
    liberation(T2S,N);
    liberation(T3S,N);
    liberation(T4S,N);
    liberation(tabGray,N);
    if(Solution!=NULL){
        while(Solution->suivant!=NULL){
            mpz_clear(Solution->valeur);
            Solution = Solution->suivant;
        }
        mpz_clear(Solution->valeur);
    }
    free(Solution);
}