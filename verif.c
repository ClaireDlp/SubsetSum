#include "word.h"

//Fonction qui renvoie 1 si une solution donnée est vraie, 0 sinon
//En paramètre : le tableau des éléments pouvant être sommé, la somme cherché, la solution proposé par le programme
int verificationSol(word* ai, word s, word Solution, word* tabGray){

    gmp_printf("\nSol : %Zd",Solution);
    
    unsigned long long i1, i2, i3, i4, nbEltParTab, nbElt;

    //On cherche à partir de la solution donné, les indices correspondant à une valeur binaire, qui associe les sommation effectué pour donner la solution.
    //C-à-d
    //Si a = (2,3,4,5,6,7,8,9) et Solution = (136) base 10 <=> (10001000) base 2. (pour somme cible = 8)
    //Alors i1 = 2, i2 = 0, i3 = 2 et i4 = 0


    //Si WORD_SIZE n'est pas divisible par 4
    if(WORD_SIZE%4!=0){
        nbEltParTab = WORD_SIZE/4+1;
        nbElt = nbEltParTab*3; //Nombre d'éléments ai dans T1S, T2S et T3S
        nbElt = WORD_SIZE - nbElt; //Nombre d'éléments ai dans T4S
    }
    else{
        nbElt = nbEltParTab = WORD_SIZE/4;
    }





    word tmp, mask;
    mpz_init(mask);

    mpz_init_set(tmp,Solution);
    mpz_fdiv_q_2exp(tmp,tmp,nbEltParTab*2+nbElt);
    i1 = mpz_get_ui(tmp);

    mpz_set(tmp,Solution);
    mpz_fdiv_q_2exp(tmp,tmp,nbEltParTab+nbElt);

    mpz_set_ui(mask,(1<<nbEltParTab)-1);
    //mpz_mul_2exp(mask,mask,nbEltParTab);
    //mpz_sub_ui(mask,mask,1);
    mpz_and(tmp,tmp,mask);
    i2 = mpz_get_ui(tmp);


    mpz_set(tmp,Solution);
    mpz_fdiv_q_2exp(tmp,tmp,nbElt);

    mpz_set_ui(mask,(1<<nbEltParTab)-1);
    //mpz_mul_2exp(mask,mask,WORD_SIZE/4);
    //mpz_sub_ui(mask,mask,1);
    mpz_and(tmp,tmp,mask);
    i3 = mpz_get_ui(tmp);


    mpz_set(tmp,Solution);

    mpz_set_ui(mask,(1<<nbElt)-1);
    //mpz_mul_2exp(mask,mask,WORD_SIZE/4);
    //mpz_sub_ui(mask,mask,1);
    mpz_and(tmp,tmp,mask);
    i4 = mpz_get_ui(tmp);

    word rep;
    mpz_init(rep);
    // mpz_add(rep,rep,ai[i1]);
    // mpz_add(rep,rep,ai[i2]);
    // mpz_add(rep,rep,ai[i3]);
    // mpz_add(rep,rep,ai[i4]);

    gmp_printf("\n%llu %llu %llu %llu",i1,i2,i3,i4);


    int mask2 = 1, tmp2;

    //CHANGEMENT : Reduire à l'aide d'une fonction
    for (int i = 0; i < 4; ++i){
        if(i==0){
            mask2 = 1;
            for(int j = nbEltParTab-1; j >= 0; --j){
                tmp2 = i1 & mask2;
                //Si <> de 0
                if(tmp2){
                    mpz_add(rep,rep,ai[j]);
                    printf("\n val : %d",j);
                }
                mask2 = mask2 <<1;
            }
            gmp_printf("\ni : 0 = %Zd",rep);
        }
        else{
            if(i==1){
                mask2 = 1;
                for(int j = nbEltParTab*2-1; j >= nbEltParTab; --j){
                    tmp2 = i2 & mask2;
                    //Si <> de 0
                    if(tmp2){
                        mpz_add(rep,rep,ai[j]);
                        printf("\n val : %d",j);
                    }
                    mask2 = mask2 <<1;
                }
                gmp_printf("\ni : 1 = %Zd",rep);
            }
            else{
                if(i==2){
                    mask2 = 1;
                    for(int j = nbEltParTab*3-1; j >= nbEltParTab*2; --j){
                        tmp2 = i3 & mask2;
                        //Si <> de 0
                        if(tmp2){
                            mpz_add(rep,rep,ai[j]);
                            printf("\n val : %d",j);
                        }
                        mask2 = mask2 <<1;
                    }
                    gmp_printf("\ni : 3 = %Zd",rep);
                }
                else{
                    mask2 = 1;
                    for(int j = WORD_SIZE-1; j >= nbEltParTab*3; --j){
                        tmp2 = i4 & mask2;
                        //Si <> de 0
                        if(tmp2){
                            mpz_add(rep,rep,ai[j]);
                            printf("\n val : %d",j);
                        }
                        mask2 = mask2 <<1;
                    }
                    gmp_printf("\ni : 4 = %Zd",rep);
                }
            }
        }
    }

    if(mpz_cmp(s,rep)==0){
        gmp_printf("\nok  %Zd",rep);
        mpz_clears(mask,tmp,rep,NULL);
        return 1; //La solution est correct
    }
    else{
        gmp_printf("\npas ok  %Zd",rep);
        mpz_clears(mask,tmp,rep,NULL);
        return 0; //La solution n'est pas correct
    }
}