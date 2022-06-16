#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>



//lien doc: https://gmplib.org/gmp-man-6.2.1.pdf



//Initialiser un entier a à 0 avec n bits: mpz_init2(a, n)
//En fin de programme: libération de mémoire: mpz_clear(a)
//Affecter une valeur aléatoire à a: mpz_set(a,rand()%(word)(1<<WORD_SIZE))
//Ajouter deux entiers a= a1+a2: mpz_add (a, a1, a2)
//réduction modulaire a mod 1<<n: mpz_mod(a,a, 1<<n)

//Opérations binaires: https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling
//a= b&c : mpz_and (a, b, c)
//a =b^c mpz_xor (a, b, c)

//A re-tester:
//donne le nombre à gauche? mp_bitcnt_t mpz_scan1 (const mpz_t op, mp_bitcnt_t starting_bit)
//void mpz_setbit (mpz_t rop, mp_bitcnt_t bit_index): rop= 1<<(bit_index)? 
#define WORD_SIZE 32

int main(){

    mpz_t variable1;
    mpz_init(variable1);
    mpz_add_ui(variable1,variable1,5);
    gmp_printf("\n%Zd",variable1);
    mpz_fdiv_q_2exp(variable1,variable1,1);
    gmp_printf("\n%Zd",variable1);
    // mpz_mul_2exp(variable1,variable1,1);
    // gmp_printf("\n%Zd",variable1);
    // mpz_t tailleTableauS;
    // mpz_init(tailleTableauS);
    // mpz_set_ui(tailleTableauS,1);
    // mpz_mul_2exp(tailleTableauS,tailleTableauS,(WORD_SIZE/4));
    // gmp_printf("\n%Zd",tailleTableauS);



}
