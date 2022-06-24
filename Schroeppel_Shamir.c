#include "word.h"

//Fonction qui permet de créer les différents tableaux utilisé dans l'algorithme de Schroeppel-Shamir algotithm
void CreationT(word* TS, word* tabGray, word* ai, unsigned long long place){

    graytab tab; // n, nprec, bitchangement et pos ou neg
    tab.n = 0;
    tab.nPrec = 0;
    tab.bitChangement = 0;
    tab.signe = 0;

    mpz_set_ui(tabGray[0],0);
    //mpz_set_ui(tabGray[i],0);

    for(unsigned long long i = 0;i<(1ULL<<(WORD_SIZE/4));++i){
        if(tab.n!=0){

            // //On cherche l'information de quel bit a changé entre l'actuel représentation de gray, et l'ancienne (boucle précédente)
            // bitChangement = n ^ nprec;

            // //On cherche à connaître le bit associé à la valeur à soustraire ou ajouter
            // bitChangement = emplacementBit(bitChangement);

            //On cherche l'actuel code de gray associé à l'entier i
            tab = gray(i,tab);

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
    
    CreationT(T1S,tabGray,ai,0);
    CreationT(T2S,tabGray,ai,WORD_SIZE/4);
    CreationT(T3S,tabGray,ai,WORD_SIZE/2);
    CreationT(T4S,tabGray,ai,3*(WORD_SIZE/4));

    ListeSol SOL = NULL;
    SOL = Modular_merge(T1S, T2S, T3S, T4S, s, SOL);

    //On affiche les solutions, à partir de leur code de gray, concaténation à effectuer
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