#include "word.h"

//Fonction qui renvoie 1 si une solution donnée est vraie, 0 sinon
//En paramètre : le tableau des élèments pouvant être sommé, la somme cherché, la solution proposé par le programme
int verificationSol(word* ai, word s, word Solution){

    gmp_printf("\nSol : %Zd",Solution);
    
    unsigned long long i1, i2, i3, i4;

    //On cherche à partir de la solution donné, les indices correspondant à une valeur binaire, qui associe les sommation effectué pour donner la solution.
    //C-à-d
    //Si a = (2,3,4,5,6,7,8,9) et Solution = (136) base 10 <=> (10001000) base 2. (pour somme cible = 10)
    //Alors i1 = 2, i2 = 0, i3 = 2 et i4 = 0


    word tmp, mask;
    mpz_init(mask);

    mpz_init_set(tmp,Solution);
    mpz_fdiv_q_2exp(tmp,tmp,(WORD_SIZE/4)*3);
    i1 = mpz_get_ui(tmp);

    mpz_set(tmp,Solution);
    mpz_fdiv_q_2exp(tmp,tmp,(WORD_SIZE/4)*2);

    mpz_set_ui(mask,1);
    mpz_mul_2exp(mask,mask,WORD_SIZE/4);
    mpz_sub_ui(mask,mask,1);
    mpz_and(tmp,tmp,mask);
    i2 = mpz_get_ui(tmp);


    mpz_set(tmp,Solution);
    mpz_fdiv_q_2exp(tmp,tmp,(WORD_SIZE/4));

    mpz_set_ui(mask,1);
    mpz_mul_2exp(mask,mask,WORD_SIZE/4);
    mpz_sub_ui(mask,mask,1);
    mpz_and(tmp,tmp,mask);
    i3 = mpz_get_ui(tmp);


    mpz_set(tmp,Solution);

    mpz_set_ui(mask,1);
    mpz_mul_2exp(mask,mask,WORD_SIZE/4);
    mpz_sub_ui(mask,mask,1);
    mpz_and(tmp,tmp,mask);
    i4 = mpz_get_ui(tmp);

    word rep;
    mpz_init(rep);
    int mask2 = 1, tmp2, N = WORD_SIZE/4;

    //CHANGEMENT : Reduire à l'aide d'une fonction
    for (int i = 0; i < 4; ++i){
        if(i==0){
            mask2 = 1;
            for(int j = 0; j < N; ++j){
                tmp2 = i1 & mask2;
                //Si <> de 0
                if(tmp2){
                    mpz_add(rep,rep,ai[j]);
                }
                mask2 = mask2 <<1;
            }
            gmp_printf("\ni : 0 = %Zd",rep);
        }
        else{
            if(i==1){
                mask2 = 1;
                for(int j = N; j < N*2; ++j){
                    tmp2 = i2 & mask2;
                    //Si <> de 0
                    if(tmp2){
                        mpz_add(rep,rep,ai[j]);
                    }
                    mask2 = mask2 <<1;
                }
                gmp_printf("\ni : 1 = %Zd",rep);
            }
            else{
                if(i==2){
                    mask2 = 1;
                    for(int j = N*2; j < N*3; ++j){
                        tmp2 = i3 & mask2;
                        //Si <> de 0
                        if(tmp2){
                            mpz_add(rep,rep,ai[j]);
                        }
                        mask2 = mask2 <<1;
                    }
                    gmp_printf("\ni : 3 = %Zd",rep);
                }
                else{
                    mask2 = 1;
                    for(int j = N*3; j < WORD_SIZE; ++j){
                        tmp2 = i4 & mask2;
                        //Si <> de 0
                        if(tmp2){
                            mpz_add(rep,rep,ai[j]);
                        }
                        mask2 = mask2 <<1;
                    }
                    gmp_printf("\ni : 4 = %Zd",rep);
                }
            }
        }
    }

    if(mpz_cmp(s,rep)==0){
        mpz_clears(mask,tmp,rep,NULL);
        return 1; //La solution est correct
    }
    else{
        mpz_clears(mask,tmp,rep,NULL);
        return 0; //La solution n'est pas correct
    }
}