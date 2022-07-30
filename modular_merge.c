#include "word.h"

//Fonction qui inverse un nombre binaire
//Exemple 100 (4 base 10) -> 001 (1 base 10)
unsigned long long inverse(unsigned long long nb, int lg){
    unsigned long long mask = 1<<(lg-1), rep = 0, ok = 0;
    for (int i = 0; i < lg; ++i){
    	ok = mask & nb;
    	if(ok){
    		rep = rep + (1<<i);
    	}
    	mask = 1<<(lg-2-i);
    }
    return rep;
}

//Fonction de tri par denombrement
void triParDenombrement(word* TS, pair* T, pair* RES, unsigned long long M, unsigned long long tailleT){

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

    //Libération de l'espace libre
    liberation(TStrie,tailleT);
    free(C);
}

//Fonction qui permet de renvoyer l'indice de l'emplacement de la première apparition de ot dans le tableau RES
//CHANGEMENT Vérifier la véracité de l'algo -> possible qu'il ne prenne pas l'indice le plus petit
unsigned long long dichotomie(unsigned long long ot, pair* RES, unsigned long long tailleTableauS){
    unsigned long long milieu/* = ot*tailleTableauS/4*/, debut = 0, fin=tailleTableauS-1;
    //valeur impossible (max indice : tailleTableauS*tailleTableauS-1)
    unsigned long long res = tailleTableauS*tailleTableauS;
    unsigned long long val;

    while(debut < fin){
        milieu = (unsigned long long) (debut+fin)/2;
        val = mpz_get_ui(RES[milieu].word);
     
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
        //CHANGEMENT vérifier si on peut améliorer la syntaxe
        if(debut==fin){
            if(mpz_get_ui(RES[debut].word)==ot){
                res = debut;
            }
            break;
        }
    }
    return res;
}
//CHANGEMENT fusion de dicho2 et dicho
unsigned long long dichotomie2(unsigned long long ot, triple* RES, unsigned long long tailleTableauS){
    unsigned long long milieu/* = ot*tailleTableauS/4*/, debut = 0, fin=tailleTableauS-1;
    //valeur impossible (max indice : tailleTableauS*tailleTableauS-1)
    unsigned long long res = tailleTableauS*tailleTableauS;
    unsigned long long val;

    while(debut < fin){
        milieu = (unsigned long long) (debut+fin)/2;
        val = mpz_get_ui(RES[milieu].word);
     
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
    triple tmp1, tmp2;
    mpz_inits(tmp1.word,tmp2.word);
    for(unsigned long long i = 0; i < tailleTableauS; i++){
        mpz_set(ol,T1S[i]);
        mpz_set_ui(tmp_w,om);
        mpz_sub(tmp_w,tmp_w,ol);
        mpz_mod_ui(tmp_w,tmp_w,M);
        unsigned long long ot = mpz_get_ui(tmp_w);


        //retourne le premier indice recherché (càd dont la valeur de sa cellule = ot)
        unsigned long long j = dichotomie(ot,RES1,tailleTableauS);

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

                //On set S1 en le triant
                if(*tailleS1==0){
                    S1[(*tailleS1)] = t;
                }
                else{
                    for (unsigned long long a = 0; a < (*tailleS1); a++){
                        //Si (S1[a].word>t.word)
                        if(mpz_cmp(S1[a].word,t.word)>0){
                            tmp2 = t;
                            while(a<=(*tailleS1)){
                                tmp1 = S1[a];
                                S1[a] = tmp2;
                                tmp2 = tmp1;
                                a++;
                            }
                        }
                        else{
                            if(a+1 == (*tailleS1)){
                                S1[a+1] = t;
                            }
                        }
                    }
                }
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
    mpz_clears(ol,tmp_w,tmp1.word,tmp2.word,NULL);
}

ListeSol solver(ListeSol SOL,triple* S1, word* T3S, word* T4S, pair* RES2, unsigned long long tailleTableauS,word TargetSum, unsigned long long M, unsigned long long om, unsigned long long* tailleS1, word* tabGray){
    word ol, tmp_w, Tprime;
    mpz_inits(ol,tmp_w,Tprime,NULL);
    
    for(unsigned long long i = 0; i < tailleTableauS; i++){
        mpz_set(ol,T3S[i]);
        mpz_set_ui(tmp_w,om);
        // mpz_sub(tmp_w,tmp_w,ol);
        // mpz_sub(tmp_w,TargetSum,tmp_w);
        // mpz_mod_ui(tmp_w,tmp_w,M);
        // unsigned long long ot = mpz_get_ui(tmp_w);

        //CHANGEMENT vérifier si complexité de mpz_get_ui > 
        unsigned long long ot = (mpz_get_ui(TargetSum)-om-mpz_get_ui(ol)) % M;

        unsigned long long j = dichotomie(ot,RES2,tailleTableauS);

        //Si WORD_SIZE n'est pas divisible par 4, on gére T4S et les 0 à ne pas considérer
        int nbEltParTab, nbElt = tailleTableauS;
        unsigned long long tailleTableauS4;

        if(WORD_SIZE%4!=0){
            nbEltParTab = WORD_SIZE/4+1;
            nbElt = nbEltParTab*3; //Nombre d'éléments ai dans T1S, T2S et T3S
            nbElt = WORD_SIZE - nbElt; //Nombre d'éléments ai dans T4S
            tailleTableauS4 = (1ULL<<nbElt);
        }
        else{
            nbElt = nbEltParTab = WORD_SIZE/4;
            tailleTableauS4 = tailleTableauS;
        }

        while (j < (tailleTableauS)){
            if(mpz_get_ui(RES2[j].word)==ot){
                if(RES2[j].indexe < tailleTableauS4){ //Condition utile dans le cas où WORD_SIZE n'est pas divisible par 4, permet d'éviter l'apparition de solutions impossible
                    // word Tprime;
                    // word T;
                    // mpz_init(T);
                    // mpz_sub(T,TargetSum,ol); //CHANGEMENT  : T3S[j]
                    // mpz_sub(T,T,T4S[j]);
                    // mpz_init_set(Tprime,T);

                    //CHANGEMENT vérifier si complexité de mpz_get_ui > 
                    unsigned long long T = mpz_get_ui(TargetSum)-mpz_get_ui(ol)-mpz_get_ui(T4S[j]);
                    mpz_set_ui(Tprime,T);

                    //retoune le premier indice recherché (càd dont la valeur de sa cellule = ot)
                    unsigned long long k = dichotomie2(T,S1,tailleTableauS);

                    while (k < (*tailleS1)){
                        if(mpz_get_ui(S1[k].word)==mpz_get_ui(Tprime)){
                            solution s;
                            s.i = inverse(mpz_get_ui(tabGray[S1[k].i]),nbEltParTab);
                            s.j = inverse(mpz_get_ui(tabGray[S1[k].j]),nbEltParTab);
                            s.k = inverse(mpz_get_ui(tabGray[i]),nbEltParTab);
                            s.l = inverse(mpz_get_ui(tabGray[RES2[j].indexe]),nbElt);
                            SOL = AjouterListeSol(s,SOL);
                        } 
                        else{
                            break;
                        } 
                        k++;
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

//Fonction qui permet d'afficher une idée approximative de l'avancement du processus en cours
int chargement(unsigned long long pos, unsigned long long max, int* anc) {
    int p = floor((pos*100)/max);
    if(p > *anc){
        system("clear");
        printf("\nETAT : %d %%",p);
    }
    return p;
}

//Algorithme 3 Modular 4-Way merge routine
ListeSol Modular_merge(word* T1S, word* T2S, word* T3S, word* T4S, word TargetSum, ListeSol SOL, word* tabGray){

    unsigned long long M = (1ULL<<(WORD_SIZE/4));

    //Si WORD_SIZE n'est pas divisible par 4
    if(WORD_SIZE%4!=0){
        M = (1ULL<<(WORD_SIZE/4+1)); //Permet l'utilisation du programme sur une dimension non divisible par 4
    }
    //tailleTableauS <=> M
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
    //TRIE : Répartition uniforme des éléments
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

    //Variable affichage du pourcentage de chargement du programme
    int anc = 1;

    for(unsigned long long om = 0; om < M; om++){
        tailleS1 = 0;
        S1 = join(S1,T1S,T2S,RES1,tailleTableauS,M,om,&tailleS1,&nbReallocS1);
        if (S1 == NULL){
            printf("\nErreur");
            return NULL;
        }
        SOL = solver(SOL,S1,T3S,T4S,RES2,tailleTableauS,TargetSum,M,om,&tailleS1,tabGray);
        anc = chargement(om,M,&anc);
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
