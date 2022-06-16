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

Liste3 join(Liste3 S1, word* T1S, word* T2S, pair* RES1, unsigned long long tailleTableauS, unsigned long long M, unsigned long long om){
    S1 = NULL;
    word ol, tmp_w;
    mpz_inits(ol,tmp_w,NULL);
    for(unsigned long long i = 0; i < tailleTableauS; i++){
        mpz_set(ol,T1S[i]);
        mpz_set_ui(tmp_w,om);
        mpz_sub(tmp_w,tmp_w,ol);
        mpz_mod_ui(tmp_w,tmp_w,M);
        unsigned long long ot = mpz_get_ui(tmp_w);
        for (unsigned long long j = 0; j < tailleTableauS; j++){ //Claire: remplacer par recherche plus efficace, la bonne valeur devrait être autour de RES1[ot]
            if(mpz_get_ui(RES1[j].word)==ot){
                triple t;
                t.i = i;
                t.j = RES1[j].indexe;
                mpz_init(t.word);
                mpz_add(t.word,T2S[j],ol);
                S1 = AjouterListe3(t,S1);
            }
        }
    }
    
    //affichage
    if(DEBUG){
        Liste3 parcours = S1;
        if(parcours!=NULL){
            printf("\nListe %llu",om);
            while(parcours->suivant!=NULL){
                gmp_printf("\n%llu %llu %Zd",parcours->valeur.j, parcours->valeur.i, parcours->valeur.word);
                parcours = parcours->suivant;
            }
            gmp_printf("\n%llu %llu %Zd",parcours->valeur.j, parcours->valeur.i, parcours->valeur.word);
        }
    }

    //Liération de l'espace libre
    mpz_clears(ol,tmp_w,NULL);
    
    return S1;
}

ListeSol solver(ListeSol SOL,Liste3 S1, word* T3S, word* T4S, pair* RES2, unsigned long long tailleTableauS,word TargetSum, unsigned long long M, unsigned long long om){
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

        for (unsigned long long j = 0; j < tailleTableauS; j++)
        {
            if(mpz_get_ui(RES2[j].word)==ot){
                // word Tprime;
                // word T;
                // mpz_init(T);
                // mpz_sub(T,TargetSum,ol); //CHANGEMENT  : T3S[j]
                // mpz_sub(T,T,T4S[j]);
                // mpz_init_set(Tprime,T);

                //CHANGEMENT
                unsigned long long T = mpz_get_ui(TargetSum)-mpz_get_ui(ol)-mpz_get_ui(T4S[j]);
                //mpz_init(Tprime);
                mpz_set_ui(Tprime,T);


                Liste3 parcoursS1 = S1;
                if(parcoursS1!=NULL){
                    while(parcoursS1->suivant!=NULL){
                        if(mpz_get_ui(parcoursS1->valeur.word)==mpz_get_ui(Tprime)){ //A optimiser (Ne pas transformer en ull...)
                            solution s;
                            s.i = parcoursS1->valeur.i;
                            s.j = parcoursS1->valeur.j;
                            s.k = i;
                            s.l = RES2[j].indexe;
                            SOL = AjouterListeSol(s,SOL);
                        }
                        parcoursS1 = parcoursS1->suivant;
                    }
                    if(mpz_get_ui(parcoursS1->valeur.word)==mpz_get_ui(Tprime)){ //A optimiser (Ne pas transformer en ull...)
                        solution s;
                        s.i = parcoursS1->valeur.i;
                        s.j = parcoursS1->valeur.j;
                        s.k = i;
                        s.l = RES2[j].indexe;
                        SOL = AjouterListeSol(s,SOL);
                    }
                }
            }
        }
    }
    mpz_clears(ol,tmp_w,Tprime,NULL);
    return SOL;
}

//Algorithme 3 Modular 4-Way merge routine
ListeSol Modular_merge(word* T1S, word* T2S, word* T3S, word* T4S, word TargetSum, ListeSol SOL){

    //CHANGEMENT
    unsigned long long M = (1ULL<<(WORD_SIZE/4));
    unsigned long long tailleTableauS = (1ULL<<(WORD_SIZE/4));

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
    //TRIE : Répartition uniforme des éléments (a revoir fonction aléatoire)
    triParDenombrement(T2S,T2S2,RES1,M,tailleTableauS);
    liberationPair(T2S2,tailleTableauS);
    triParDenombrement(T4S,T4S2,RES2,M,tailleTableauS);
    liberationPair(T4S2,tailleTableauS);

    Liste3 S1 = NULL;
    for(unsigned long long om = 0; om < M; om++){
        
        S1 = join(S1,T1S,T2S,RES1,tailleTableauS,M,om);

        //CHANGEMENT : TRIER LISTE S1 -> optimisation
        
        SOL = solver(SOL,S1,T3S,T4S,RES2,tailleTableauS,TargetSum,M,om);
    }

    //Libération de la mémoire
    liberationPair(RES1,tailleTableauS);
    liberationPair(RES2,tailleTableauS);

    if(S1!=NULL){
        while(S1->suivant!=NULL){
            mpz_clear(S1->valeur.word);
            S1 = S1->suivant;
        }
        mpz_clear(S1->valeur.word);
    }
    free(S1);

    return SOL;
}