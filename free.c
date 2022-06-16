#include "word.h"
 
 //ERREUR : quitter le prog si erreur..
    void liberation(word* w,unsigned long long N){
        for (unsigned long long i = 0; i < N; ++i){
            mpz_clear(w[i]);
        }
        free(w);
    }

    //CHANGEMENT : Voir si on peut associer cette fonction à liberation()
    void liberationPair(pair* w,unsigned long long N){
        for (unsigned long long i = 0; i < N; ++i){
            mpz_clear(w[i].word);
        }
        free(w);
    }

    void liberer(word* ai,word* T1S,word* T2S,word* T3S,word* T4S,word* T1x,word* T2x,word* T3x,word* T4x,ListeSolConca Solution,unsigned long long N){
        liberation(ai,N);
        liberation(T1S,N);
        liberation(T2S,N);
        liberation(T3S,N);
        liberation(T4S,N);
        liberation(T1x,N);
        liberation(T2x,N);
        liberation(T3x,N);
        liberation(T4x,N);
        if(Solution!=NULL){
            while(Solution->suivant!=NULL){
                mpz_clear(Solution->valeur);
                Solution = Solution->suivant;
            }
            mpz_clear(Solution->valeur);
        }
        free(Solution);
    }