#include "word.h"

//Fonction de tri par denombrement
void triParDenombrement(word* TS, pair* T, pair* RES, unsigned long long M, unsigned long long tailleT){
    
    //CHANGEMENT
    unsigned long long* C = malloc(sizeof(unsigned long long)*M);

    word* TStrie = malloc(sizeof(word)*tailleT);

    for (unsigned long long i = 0; i < tailleT; ++i){
        mpz_init(RES[i].word);
        mpz_init(TStrie[i]);
    }

    for (unsigned long long i = 0; i < M; ++i){
        C[i] = 0;
    }

    for (unsigned long long i = 0; i < tailleT; ++i){
        C[mpz_get_ui(T[i].word)] = C[mpz_get_ui(T[i].word)] + 1;
    }

    for (unsigned long long i = 1; i < M; ++i){
        C[i] = C[i] + C[i-1];
    }

    for (unsigned long long i = 0; i < tailleT; ++i){
        mpz_set(RES[C[mpz_get_ui(T[i].word)]-1].word,T[i].word);
        RES[C[mpz_get_ui(T[i].word)]-1].indexe = T[i].indexe;
        C[mpz_get_ui(T[i].word)] = C[mpz_get_ui(T[i].word)] -1;
    }


    //CHANGEMENT : OPTIMISER EN MEMOIRE : trie de TS
    for (unsigned long long i = 0; i < tailleT; ++i){
        mpz_set(TStrie[i],TS[RES[i].indexe]);
    }
    for (unsigned long long i = 0; i < tailleT; ++i){
        mpz_set(TS[i],TStrie[i]);
    }

    if(DEBUG){
        printf("\n-------------------------TRI FAIT");
        for (unsigned long long i = 0; i < tailleT; i++){
            gmp_printf("\n RES[i].word : %Zd   RES[i].indexe : %llu",RES[i].word,RES[i].indexe);
            gmp_printf("\n tstrie[i] : %Zd",TStrie[i]);
        }
    }

    //Libération de l'espace libre
    liberation(TStrie,tailleT);
    free(C);
}


//Vérifier la véracité de l'algo -> possible qu'il prenne pas l'indice le plus petit 
unsigned long long dichotomie(unsigned long long ot, pair* RES, unsigned long long tailleTableauS){
    unsigned long long milieu/* = ot*tailleTableauS/4*/, debut = 0, fin=tailleTableauS-1;
    //valeur impossible (max indice : tailleTableauS*tailleTableauS-1)
    unsigned long long res = tailleTableauS*tailleTableauS;

    while(debut < fin){
        milieu = (unsigned long long) (debut+fin)/2;
        unsigned long long val = mpz_get_ui(RES[milieu].word);
        if(val==ot){
            res = milieu;
            fin = milieu-1;
        }
        else{
            if(ot > val){
                debut = milieu+1;
            }
            else{
                fin = milieu-1; 
            }
        }
        //VERIFIER si on peut améliorer la syntaxe
        if(debut==fin){
            if(mpz_get_ui(RES[debut].word)==ot){
                res = debut;
            }
            break;
        }
    }

    return res;
}



triple* join(triple* S1, word* T1S, word* T2S, pair* RES1, unsigned long long tailleTableauS, unsigned long long M, unsigned long long om, unsigned long long* tailleS1, unsigned long long* nbReallocS1){
    word ol, tmp_w;
    mpz_inits(ol,tmp_w,NULL);
    for(unsigned long long i = 0; i < tailleTableauS; i++){
        mpz_set(ol,T1S[i]);
        mpz_set_ui(tmp_w,om);
        mpz_sub(tmp_w,tmp_w,ol);
        mpz_mod_ui(tmp_w,tmp_w,M);
        unsigned long long ot = mpz_get_ui(tmp_w);


        //dicho qui va retourner premier indice recherché (càd dont la valeur de sa cellule = ot)
        unsigned long long j = dichotomie(ot,RES1,tailleTableauS);

        //for (unsigned j = 0; j < tailleTableauS; j++){
        while (j < tailleTableauS){
            if(mpz_get_ui(RES1[j].word)==ot){
                //Si on dépasse la mémoire alloué à S1 alors
                // CHANGMENT regler le probleme de mpz_init dans une fonction
                if((*tailleS1) >= (tailleTableauS+tailleTableauS*(*nbReallocS1))){
                    (*nbReallocS1)++;
                    S1 =  (triple*) realloc(S1,sizeof(triple)*(tailleTableauS+(tailleTableauS*(*nbReallocS1))));
                    if(S1 == NULL){
                        return NULL;
                    }
                    for (unsigned long long k = tailleTableauS*(*nbReallocS1); k < (tailleTableauS+tailleTableauS*(*nbReallocS1)); ++k){
                        mpz_init(S1[k].word);
                    }  
                }
                triple t;
                t.i = i;
                t.j = RES1[j].indexe;
                mpz_init(t.word);
                mpz_add(t.word,T2S[j],ol);
                S1[(*tailleS1)] = t;
                (*tailleS1)++;
            }
            else{
                break;
            }
            j++;
        }
    }
    return S1;
    //Liération de l'espace libre  
    mpz_clears(ol,tmp_w,NULL);
}

ListeSol solver(ListeSol SOL,triple* S1, word* T3S, word* T4S, pair* RES2, unsigned long long tailleTableauS,word TargetSum, unsigned long long M, unsigned long long om, unsigned long long* tailleS1){
    word ol, tmp_w, Tprime;
    mpz_inits(ol,tmp_w,Tprime,NULL);
    
    for(unsigned long long i = 0; i < tailleTableauS; i++){
        mpz_set(ol,T3S[i]);
        mpz_set_ui(tmp_w,om);
        // mpz_sub(tmp_w,tmp_w,ol);
        // mpz_sub(tmp_w,TargetSum,tmp_w);
        // mpz_mod_ui(tmp_w,tmp_w,M);
        // unsigned long long ot = mpz_get_ui(tmp_w);

        //CHANGEMENT
        unsigned long long ot = (mpz_get_ui(TargetSum)-om-mpz_get_ui(ol)) % M;

        unsigned long long j = dichotomie(ot,RES2,tailleTableauS);

        while (j < (tailleTableauS)){
            if(mpz_get_ui(RES2[j].word)==ot){
                // word Tprime;
                // word T;
                // mpz_init(T);
                // mpz_sub(T,TargetSum,ol); //CHANGEMENT  : T3S[j]
                // mpz_sub(T,T,T4S[j]);
                // mpz_init_set(Tprime,T);

                //CHANGEMENT
                unsigned long long T = mpz_get_ui(TargetSum)-mpz_get_ui(ol)-mpz_get_ui(T4S[j]);
                mpz_set_ui(Tprime,T);

		//Trier S1: puis dichotomie plutôt que rechercher.
                for (unsigned long long k = 0; k < (*tailleS1); k++){
                    if(mpz_get_ui(S1[k].word)==mpz_get_ui(Tprime)){ //A optimiser (Ne pas transformer en ull...)
                        solution s;
                        s.i = S1[k].i;
                        s.j = S1[k].j;
                        s.k = i;
                        s.l = RES2[j].indexe;
                        SOL = AjouterListeSol(s,SOL);
                    }  
                }
            }
            else{
                break;
            }
            j++;
        }
    }
    mpz_clears(ol,tmp_w,Tprime,NULL);
    return SOL;
}

//Algorithme 3 Modular 4-Way merge routine
ListeSol Modular_merge(word* T1S, word* T2S, word* T3S, word* T4S, word TargetSum, ListeSol SOL){

    //CHANGEMENT
    unsigned long long M = (1ULL<<(WORD_SIZE/4));
    unsigned long long tailleTableauS = M;

    pair* T2S2 = malloc(sizeof(pair)*tailleTableauS);
    for (unsigned long long i = 0; i < tailleTableauS; ++i){
        mpz_init(T2S2[i].word);
        mpz_mod_ui(T2S2[i].word,T2S[i],M);
        T2S2[i].indexe = i;
    }

    pair* T4S2 = malloc(sizeof(pair)*tailleTableauS);
    for (unsigned long long i = 0; i < tailleTableauS; i++){
        mpz_init(T4S2[i].word);
        mpz_mod_ui(T4S2[i].word,T4S[i],M);
        T4S2[i].indexe = i; 
    }

    pair* RES1 = malloc(sizeof(pair)*tailleTableauS);
    pair* RES2 = malloc(sizeof(pair)*tailleTableauS);

    //CHANGEMENT : Regarder si on peut éviter de déclarer T2S2 et T4S2 pour gain de mémoire et calcul
    //TRIE : Répartition uniforme des éléments (à revoir fonction aléatoire)
    triParDenombrement(T2S,T2S2,RES1,M,tailleTableauS);
    liberationPair(T2S2,tailleTableauS);
    triParDenombrement(T4S,T4S2,RES2,M,tailleTableauS);
    liberationPair(T4S2,tailleTableauS);

    unsigned long long tailleS1 = 0, nbReallocS1 = 0;

    triple* S1 = malloc(tailleTableauS*sizeof(triple));
    if (S1 == NULL){
        printf("\nErreur");
        return NULL;
    }
    for (unsigned long long i = 0; i < tailleTableauS; ++i){
        mpz_init(S1[i].word);
    }

    for(unsigned long long om = 0; om < M; om++){

        tailleS1 = 0;
        S1 = join(S1,T1S,T2S,RES1,tailleTableauS,M,om,&tailleS1,&nbReallocS1);
        if (S1 == NULL){
            printf("\nErreur");
            return NULL;
        }
        //CHANGEMENT : TRIER LISTE S1 -> optimisation
        SOL = solver(SOL,S1,T3S,T4S,RES2,tailleTableauS,TargetSum,M,om,&tailleS1);
       printf("\n %llu",om);
    }

    //Libération de la mémoire
    liberationPair(RES1,tailleTableauS);
    liberationPair(RES2,tailleTableauS);

    for (size_t i = 0; i < tailleTableauS+nbReallocS1*tailleTableauS; i++){    
        mpz_clear(S1[i].word);
    }
    free(S1);
    return SOL;
}
