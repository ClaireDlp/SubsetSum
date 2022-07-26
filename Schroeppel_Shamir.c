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

//Fonction qui permet de concaténer 4 valeur entière et affiche une solution de l'algorithme de Schroeppel-Shamir
//CHANGEMENT : valeur à retourner
ListeSolConca concatenation(unsigned long long w1, unsigned long long w2, unsigned long long w3, unsigned long long w4, unsigned long long taille, ListeSolConca Solution){
    unsigned long long res, ww1, ww2, ww3;
    //word res, ww1, ww2, ww3;
    //mpz_inits(res,ww1,ww2,ww3,NULL);

    int decalage = 0, tmp;
    if(WORD_SIZE%4!=0){
        tmp= WORD_SIZE/4+1;
        tmp = tmp*3; //Nombre d'éléments ai dans T1S, T2S et T3S
        decalage = tmp/3 - (WORD_SIZE - tmp); //Nombre de 0 dans T4S non inclus dans ai
    }

    ww1= w1<<(taille*3-decalage);
    ww2= w2<<(taille*2-decalage);
    ww3= w3<<(taille-decalage);
    //mpz_mul_2exp(ww1,w1,taille*3-decalage);
    //mpz_mul_2exp(ww2,w2,taille*2-decalage);
    //mpz_mul_2exp(ww3,w3,taille-decalage);


    res = ww1 | ww2;
    res = res | ww3;
    res = res | w4;
    //mpz_ior(res,ww1,ww2);
    //mpz_ior(res,res,ww3);
    //mpz_ior(res,res,w4);

    word res2;
    mpz_init_set_ui(res2,res);

    //Ajout de la valeur concaténé dans une ListeSolConca, à terme, toutes les solutions y seront stockées
    Solution = AjouterListeSolConca(res2,Solution);
    printf(" solution : %llu",res);

    //Libération de l'espace libre
    mpz_clear(res2);

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
    CreationT(T2S,tabGray,ai,indice);
    CreationT(T3S,tabGray,ai,indice*2);
    CreationT(T4S,tabGray,ai,indice*3);

    ListeSol SOL = NULL;
    SOL = Modular_merge(T1S, T2S, T3S, T4S, s, SOL, tabGray);

    //On affiche les solutions
    if(SOL!=NULL){
        system("clear");
        afficherA(ai);
        gmp_printf("\nLES SOLUTIONS POUR UN TARGET DE %Zd, SONT",s);
        while(SOL->suivant!=NULL){
            //gmp_printf("\n %Zd %Zd %Zd %Zd",SOL->valeur.i,SOL->valeur.j,SOL->valeur.k,SOL->valeur.l);
            gmp_printf("\n %llu %llu %llu %llu",SOL->valeur.i,SOL->valeur.j,SOL->valeur.k,SOL->valeur.l);

            Solution = concatenation(SOL->valeur.i,SOL->valeur.j,SOL->valeur.k,SOL->valeur.l,indice,Solution);
            SOL = SOL->suivant;
        }
        //gmp_printf("\n %Zd %Zd %Zd %Zd",SOL->valeur.i,SOL->valeur.j,SOL->valeur.k,SOL->valeur.l);
        gmp_printf("\n %llu %llu %llu %llu",SOL->valeur.i,SOL->valeur.j,SOL->valeur.k,SOL->valeur.l);

        Solution = concatenation(SOL->valeur.i,SOL->valeur.j,SOL->valeur.k,SOL->valeur.l,indice,Solution);
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