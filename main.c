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
typedef long unsigned word;


//Fonction qui permet de selectionner le mode de génération des éléments du vecteur a
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

//Fonction qui permet d'afficher les éléments du vecteur a
void afficherA(word* ai){
    printf("\na = (");
    for(int i = 0; i<WORD_SIZE-1; ++i){
        printf("%lu,",ai[i]);
    }
    printf("%lu)",ai[WORD_SIZE-1]);
}

//Fonction qui permet de générer de façon aléatoire les éléments du vecteur a
void generationAleaA(word* ai){
    srand(time(NULL));
    //int n = choixDeN();
    for(word i = 0; i<WORD_SIZE; ++i){
        //%(max-min+1) + min;
        ai[i] = rand()%(word)(1ULL<<WORD_SIZE);
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


// //Fonction qui retourne le code de gray d'un indice nb donné
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
int nombreDe0gauche(int n){
    int i = 0;
    int temp = 0;
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
int gray(int n, int grayprec){
    int Nb0 = nombreDe0gauche(n);
    int temp = grayprec >> (Nb0-1);
    if(temp % 2 == 0){
        return grayprec+=(1<<(Nb0-1));
    }
    else{
        return grayprec-=(1<<(Nb0-1));
    }
}

//-------------------------------------------------------------------------


//Fonction qui renvoie l'emplacement du bit dans la variable "bit"
//La variable "bit" contiendra uniquement une valeur composé dans sa forme binaire d'un seul bit à 1
//Renvoie 3 si bit = (0000100)2
word emplacementBit(word bit){
    int r=0;
    while(bit!=0){
        bit = bit/2;
        r++;
    }
    return r;
}

void CreationT(word* TS, word* TX, word* ai, word place){
    word n = 0, nprec = 0, bitChangement = 0;
    TX[0]=0;

    word indice = 0;
    for(word i = 0;i<(1<<(WORD_SIZE/4));++i){
        if(n!=0){
            //On cherche l'actuel code de gray associé à l'entier i
            n = gray(i,nprec);
            //On cherche l'information de quel bit a changé entre l'actuel représentation de gray, et l'ancienne (boucle précédente)
            bitChangement = n ^ nprec;

            //On cherche à connaître le bit associé à la valeur à soustraire ou ajouter
            bitChangement = emplacementBit(bitChangement);

            //Condition permettant de savoir s'il faut ajouter ou soustraire 
            if(nprec < n){
                TS[indice]= TS[indice-1] + ai[bitChangement-1+place];
                TX[indice] = n;
                printf("\n n : %lu  nprec: %lu BIT : +%lu ",n,nprec,bitChangement);
            }
            else{
                TS[indice]= TS[indice-1] - ai[bitChangement-1+place];
                TX[indice] = n;
                printf("\n n : %lu  nprec: %lu BIT : -%lu",n,nprec,bitChangement);
            }
            nprec = n;
        }
        else{
            TS[indice] = 0; // indice = 0
            TX[indice] = 0;
            n=1;
        }
        indice++;
    }
    //affichage
    for(word i = 0;i<(1<<(WORD_SIZE/4));++i){
        printf("\nTS[%lu]=%lu TX[%lu]=%lu",i,TS[i],i,TX[i]);
    }
}

void Algo1(word* ai, word s, word* T, word* T1S, word* T2S, word* T3S, word* T4S, word* T1x, word* T2x, word* T3x, word* T4x){
    CreationT(T1S,T1x,ai,0);
    CreationT(T2S,T2x,ai,WORD_SIZE/4);
    CreationT(T3S,T3x,ai,WORD_SIZE/2);
    CreationT(T4S,T4x,ai,3*(WORD_SIZE/4));
}

int main(){
    //On crée le tableau qui contiendra les éléments du vecteur a
    word ai[WORD_SIZE];

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
    word* xi = NULL;
    
    word tailleTableauS = (1<<(WORD_SIZE/4));

    word tailleTableaux = (tailleTableauS*(WORD_SIZE/4));

    word T[tailleTableauS*4];

    word T1S[tailleTableauS];
    word T2S[tailleTableauS];
    word T3S[tailleTableauS];
    word T4S[tailleTableauS];

    word T1x[tailleTableaux];
    word T2x[tailleTableaux];
    word T3x[tailleTableaux];
    word T4x[tailleTableaux];

    Algo1(ai,10,T,T1S,T2S,T3S,T4S,T1x,T2x,T3x,T4x);

}
