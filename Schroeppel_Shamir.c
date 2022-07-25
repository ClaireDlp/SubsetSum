#include "word.h"

//Fonction qui permet de créer les différents tableaux utilisé dans l'algorithme de Schroeppel-Shamir algotithm
void CreationT(word* TS, word* tabGray, word* ai, unsigned long long place){

    //Si WORD_SIZE n'est pas divisible par 4
    int indice = WORD_SIZE/4;
    if(WORD_SIZE%4!=0){
        indice++; //On augmente de 1 indice afin de permettre l'utilisation du programme sur une dimension non divisible par 4
    }

    graytab tab; // n, nprec, bitchangement et pos ou neg
    tab.n = 0;
    tab.nPrec = 0;
    tab.bitChangement = 0;
    tab.signe = 0;

    mpz_set_ui(tabGray[0],0);
    //mpz_set_ui(tabGray[i],0);

    for(unsigned long long i = 0;i<(1ULL<<indice);++i){

        if(tab.n!=0){

            //On cherche l'actuel code de gray associé à l'entier i
            tab = gray(i,tab);

            //Si WORD_SIZE n'est pas divisible par 4, on ajoute à T4S des entiers 0 comme éléments de ai
            //Si tous les éléments ai ont été parcouru
            if(tab.bitChangement-1+place >= WORD_SIZE && place == indice*3){
                mpz_set(TS[i],TS[i-1]);

                //tabGray[i] = n;
                mpz_set_ui(tabGray[i],tab.n);

                tab.nPrec = tab.n;
            }
            else{
                //Condition permettant de savoir s'il faut ajouter ou soustraire 
                if(tab.signe==0){
                    //TS[i]= TS[i-1] +ai[bitChangement-1+place];
                    mpz_add(TS[i],TS[i-1],ai[tab.bitChangement-1+place]);
                    //tabGray[i] = n;
                    mpz_set_ui(tabGray[i],tab.n);
                }
                else{
                    mpz_sub(TS[i],TS[i-1],ai[tab.bitChangement-1+place]);
                    //tabGray[i] = n;
                    mpz_set_ui(tabGray[i],tab.n);
                }
                tab.nPrec = tab.n;
            }
        }
        else{
            mpz_set_ui(TS[i],0);// i = 0
            //mpz_set_ui(tabGray[i],0);// i = 0
            mpz_set_ui(tabGray[0],0);
            tab.n=1;
        }
    }
}

//Fonction qui permet de concaténer 4 valeur word et affiche une solution de l'algorithme de Schroeppel-Shamir
//CHANGEMENT : valeur à retourner
ListeSolConca concatenation(word w1, word w2, word w3, word w4, unsigned long long taille, ListeSolConca Solution){
    word res, ww1, ww2, ww3;
    mpz_inits(res,ww1,ww2,ww3,NULL);

    mpz_mul_2exp(ww1,w1,taille*3);
    mpz_mul_2exp(ww2,w2,taille*2);
    mpz_mul_2exp(ww3,w3,taille);


    mpz_ior(res,ww1,ww2);
    mpz_ior(res,res,ww3);
    mpz_ior(res,res,w4);

    //Ajout de la valeur concaténé dans une ListeSolConca, à terme, toutes les solutions y seront stockées
    Solution = AjouterListeSolConca(res,Solution);
    gmp_printf(" solution : %Zd",res);

    //Libération de l'espace libre
    mpz_clears(res,ww1,ww2,ww3,NULL);

    return Solution;
}


//Schroeppel-Shamir algorithm
ListeSolConca Schroeppel_Shamir(word* ai, word s, word* T1S, word* T2S, word* T3S, word* T4S, word* tabGray, ListeSolConca Solution){
    
    //Si WORD_SIZE n'est pas divisible par 4
    int indice = WORD_SIZE/4;
    if(WORD_SIZE%4!=0){
        indice++; //On augmente de 1 indice afin de permettre l'utilisation du programme sur une dimension non divisible par 4
    }
    //CHANGEMENT : on peut faire une fonction qui construit tabGray, au lieu de le faire à chaque appel de CreationT
    CreationT(T1S,tabGray,ai,0);
    printf("\n");
    for(int k = 0; k < (1ULL<<indice); k++){
        gmp_printf("%Zd ",T1S[k]);
    }
    CreationT(T2S,tabGray,ai,indice);
    printf("\n");
    for(int k = 0; k < (1ULL<<indice); k++){
        gmp_printf("%Zd ",T2S[k]);
    }
    CreationT(T3S,tabGray,ai,indice*2);
    printf("\n");
    for(int k = 0; k < (1ULL<<indice); k++){
        gmp_printf("%Zd ",T3S[k]);
    }
    CreationT(T4S,tabGray,ai,indice*3);
    printf("\n");
    for(int k = 0; k < (1ULL<<indice); k++){
        gmp_printf("%Zd ",T4S[k]);
    }



    ListeSol SOL = NULL;
    SOL = Modular_merge(T1S, T2S, T3S, T4S, s, SOL);

    //On affiche les solutions
    if(SOL!=NULL){
        system("clear");
        afficherA(ai);
        gmp_printf("\nLES SOLUTIONS POUR UN TARGET DE %Zd, SONT",s);
        while(SOL->suivant!=NULL){
            gmp_printf("\n %Zd %Zd %Zd %Zd",tabGray[SOL->valeur.i],tabGray[SOL->valeur.j],tabGray[SOL->valeur.k],tabGray[SOL->valeur.l]);
            Solution = concatenation(tabGray[SOL->valeur.i],tabGray[SOL->valeur.j],tabGray[SOL->valeur.k],tabGray[SOL->valeur.l],WORD_SIZE/4,Solution);
            SOL = SOL->suivant;
        }
        gmp_printf("\n %Zd %Zd %Zd %Zd",tabGray[SOL->valeur.i],tabGray[SOL->valeur.j],tabGray[SOL->valeur.k],tabGray[SOL->valeur.l]);
        Solution = concatenation(tabGray[SOL->valeur.i],tabGray[SOL->valeur.j],tabGray[SOL->valeur.k],tabGray[SOL->valeur.l],WORD_SIZE/4,Solution);

    }
    else{
        Solution = NULL;
        system("clear");
        afficherA(ai);
        gmp_printf("\nPAS DE SOLUTIONS POUR UN TARGET DE %Zd",s);
    }
    free(SOL);
    return Solution;
}