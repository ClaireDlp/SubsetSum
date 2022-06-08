#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

// valeur de la taille de n
#define WORD_SIZE 8

typedef short bool;
#define true 1
#define false 0

//stocker des chaînes de n bits.


//typedef long unsigned word;
typedef mpz_t word;


typedef struct{
    unsigned long long n;
    unsigned long long nPrec;
    unsigned long long bitChangement;
    unsigned long long signe;
} graytab;


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



// mpz_init_set_ui   Initialisation et declaration
// 




// //Fonction qui permet de selectionner le mode de génération des éléments du vecteur a
bool choixDeA(){
    short rep = 1;
    printf("Que souhaitez vous pour a :\n\t1: le générer aléatoirement\n\t2: le générer manuellement\n");
    scanf("%hd",&rep);
    if(rep == true){
        return true;
    }
    else{
        return false;
    }
}

// //Fonction qui permet d'afficher les éléments du vecteur a
void afficherA(word* ai){
    printf("\na = (");
    for(int i = 0; i<WORD_SIZE-1; ++i){
        gmp_printf("%Zd,",ai[i]);
    }
    gmp_printf("%Zd)",ai[WORD_SIZE-1]);
}

// //Fonction qui permet de générer de façon aléatoire les éléments du vecteur a
void generationAleaA(word* ai){
    srand(time(NULL));
    //int n = choixDeN();
    for(int i = 0; i<WORD_SIZE; ++i){
        //A REVOIR
        mpz_set_ui(ai[i],rand() % (1<<WORD_SIZE));
    }
    afficherA(ai);
}


//Fonction qui permet de chosir la taille de n
// int choixDeN(){
//     int n = 8;
//     printf("\nCombien d'éléments dans a souhaitez vous ? Attention la valeur saisie doit être une puissance de 2 et >= 8\n");
//     scanf("%d",&n);
//     if(round(log2(n)) != log2(n) && n >=8){ //8 ou 4 ? 
//         return choixDeN();
//     }
//     return n;
// }


//--------------------------------GRAY-------------------------------------

// //Fonction qui permet de chercher le nombre de bit à 1 dans un entier
// int nombreDe1(int n, int cpt){
//     if(n==0){
//         return cpt;
//     }
//     else{
//         int i = floor(log2(n));
//         n = n - (1<<i);
//         return nombreDe1(n,cpt+1);
//     }
// }

// //Fonction qui inverse inverse le bit situé à gauche du bit 1 le plus à droite dans un entier
// word inverse(word n){
//     word nb = n;
//     while(round(log2(nb)) - log2(nb) != 0){
//         int i = floor(log2(nb));
//         nb = nb - (1<<i);
//     }
//     word ntest = n;
//     if((ntest|(nb*2))==n+nb*2){
//         return n+nb*2;
//     }
//     return n-nb*2;
// }


// //Fonction qui retourne le code de gray d'un i nb donné
// word gray(word nb){
//     int nb1 = nombreDe1(nb,0);
//     if(nb1 % 2 == 0){
//         if(nb % 2 == 0){
//             nb++;
//         }
//         else{
//             nb--;
//         }
//     }
//     else{
//         nb = inverse(nb);
//     }
//     return nb;
// }

//Fonction qui permet de chercher le nombre de bit à 0 à gauche du dernier bit à 1, et retourne ce résultat + 1
unsigned long long nombreDe0gauche(unsigned long long n){
    unsigned long long i = 0;
    unsigned long long temp = 0;
    if(n!=0){
        while(temp==0){
            temp = n & (1<<i);
            i++;
        }
    }
    else{
        i++;
    }
    return i;
}

//Fonction qui renvoie le nombre de gray correspondant à partir, d'un nombre entier correspondant au code de gray associé, et le code de gray correcpondant à l'entier associé n-1
graytab gray(unsigned long long n, graytab tab){
    unsigned long long Nb0 = nombreDe0gauche(n);
    unsigned long long temp = tab.nPrec >> (Nb0-1);
    if(temp % 2 == 0){
        tab.n = tab.nPrec+(1<<(Nb0-1));
        tab.signe = 0;
    }
    else{
        tab.n = tab.nPrec-(1<<(Nb0-1));
        tab.signe = 1;
    }
    tab.bitChangement = Nb0;
    return tab;
}

//-------------------------------------------------------------------------


// //Fonction qui renvoie l'emplacement du bit dans la variable "bit"
// //La variable "bit" contiendra uniquement une valeur composé dans sa forme binaire d'un seul bit à 1
// //Renvoie 3 si bit = (0000100)2
// unsigned long long emplacementBit(unsigned long long bit){
//     unsigned long long r=0;
//     while(bit!=0){
//         bit = bit / 2;
//         r++;
//     }
//     return r;
// }


void CreationT(word* TS, word* TX, word* ai, unsigned long long place){

    graytab tab; // n, nprec et bitchangement et pos ou neg
    tab.n = 0;
    tab.nPrec = 0;
    tab.bitChangement = 0;
    tab.signe = 0;

    mpz_set_ui(TX[0],0);
    //mpz_set_ui(TX[i],0);

    for(unsigned long long i = 0;i<(1<<(WORD_SIZE/4));++i){
        if(tab.n!=0){
            //On cherche l'actuel code de gray associé à l'entier i
            tab = gray(i,tab); //Claire: Mettre à jour le tableau dans la fonction Gray_tableau

            // //On cherche l'information de quel bit a changé entre l'actuel représentation de gray, et l'ancienne (boucle précédente)
            // bitChangement = n ^ nprec;

            // //On cherche à connaître le bit associé à la valeur à soustraire ou ajouter
            // bitChangement = emplacementBit(bitChangement);

            //Condition permettant de savoir s'il faut ajouter ou soustraire 
            if(tab.signe==0){
                //TS[i]= TS[i-1] +ai[bitChangement-1+place];
                mpz_add(TS[i],TS[i-1],ai[tab.bitChangement-1+place]);
                //TX[i] = n;
                mpz_set_ui(TX[i],tab.n);

                printf("\n n : %llu  nprec: %llu BIT : +%llu ",tab.n,tab.nPrec,tab.bitChangement);
            }
            else{
                mpz_sub(TS[i],TS[i-1],ai[tab.bitChangement-1+place]);
                //TX[i] = n;
                mpz_set_ui(TX[i],tab.n);
                printf("\n n : %llu  nprec: %llu BIT : -%llu",tab.n,tab.nPrec,tab.bitChangement);
            }
            tab.nPrec = tab.n;
        }
        else{
            mpz_set_ui(TS[i],0);// i = 0
            //mpz_set_ui(TX[i],0);// i = 0
            mpz_set_ui(TX[0],0);
            tab.n=1;
        }
    }
    //affichage
    for(unsigned long long i = 0;i<(1<<(WORD_SIZE/4));++i){
        gmp_printf("\nTS[%llu]=%Zd TX[%llu]=%Zd",i,TS[i],i,TX[i]);
        //gmp_printf("\n%Zd",TS[i]);s
    }
}

void Algo1(word* ai, word s, word* T, word* T1S, word* T2S, word* T3S, word* T4S, word* T1x, word* T2x, word* T3x, word* T4x){
    CreationT(T1S,T1x,ai,0);
    CreationT(T2S,T2x,ai,WORD_SIZE/4);
    CreationT(T3S,T3x,ai,WORD_SIZE/2);
    CreationT(T4S,T4x,ai,3*(WORD_SIZE/4));
}


int main(){

    //lien doc: https://gmplib.org/gmp-man-6.2.1.pdf

    //Initialiser un entier a à 0 avec n bits: mpz_init2(a, n)
    //En fin de programme: libération de mémoire: mpz_clear(a)
    //Affecter une valeur aléatoire à a: mpz_set(a,rand()%(word)(1<<WORD_SIZE))
    //Ajouter deux entiers a= a1+a2: mpz_add (a, a1, a2)
    //réduction modulaire a mod 1<<n: mpz_mod(a,a, 1<<n)

    //Opérations binaires: https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling
    //a= b&c : mpz_and (a, b, c)
    //a =b^c mpz_xor (a, b, c)
    



    //On crée le tableau qui contiendra les éléments du vecteur a
    word ai[WORD_SIZE];
    for(int i=0;i<WORD_SIZE;++i){
        mpz_init(ai[i]);
    }


    //On génére les éléments du vecteur a
    bool choixDefDeA = true;

    choixDefDeA = choixDeA();
    if(choixDefDeA){
        generationAleaA(ai);
    }
    else{
        //generationManuel();
    }


    //ALORITHME 1
    //word* xi = NULL; ????????
    
    unsigned long long tailleTableauS = (1<<(WORD_SIZE/4));
    // word tailleTableauS;
    // mpz_init(tailleTableauS);
    // mpz_set_ui(tailleTableauS,1);
    // mpz_mul_2exp(tailleTableauS,tailleTableauS,WORD_SIZE/4);


    unsigned long long tailleTableaux = (tailleTableauS*(WORD_SIZE/4));
    // word tailleTableaux;
    // mpz_init(tailleTableaux);
    // mpz_mul_ui(tailleTableaux,tailleTableauS,WORD_SIZE/4);


    word T[tailleTableauS*4];
    for(int i=0;i<tailleTableauS*4;++i){
        mpz_init(T[i]); //DE même pour l'incrémaentation des ints...
    }
    // word tailleTableauS4;
    // mpz_init(tailleTableauS4);
    // mpz_mul_ui(tailleTableauS4,tailleTableauS,4);
    // word T[tailleTableauS4];
    // word i;
    // mpz_init(i);
    // for(mpz_set_ui(i,0);mpz_cmp(i,tailleTableauS4);mpz_add_ui(i,i,1)){
    //     mpz_init(T[i]);
    // }


    word T1S[tailleTableauS];
    word T2S[tailleTableauS];
    word T3S[tailleTableauS];
    word T4S[tailleTableauS];
    for(int i=0;i<tailleTableauS;++i){
        mpz_init(T1S[i]);
        mpz_init(T2S[i]);
        mpz_init(T3S[i]);
        mpz_init(T4S[i]);
    }

    word T1x[tailleTableaux];
    word T2x[tailleTableaux];
    word T3x[tailleTableaux];
    word T4x[tailleTableaux];
    for(int i=0;i<tailleTableaux;++i){
        mpz_init(T1x[i]);
        mpz_init(T2x[i]);
        mpz_init(T3x[i]);
        mpz_init(T4x[i]);
    }


    word s;
    mpz_init(s);
    mpz_set_ui(s,10);

    Algo1(ai,s,T,T1S,T2S,T3S,T4S,T1x,T2x,T3x,T4x);

    //Dans une fonction
    for(int i=0;i<WORD_SIZE;++i){
        mpz_clear(ai[i]);
    }
}
