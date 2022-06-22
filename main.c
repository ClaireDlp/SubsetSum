//|___________________________________________________________________________________|//
//|Informations :                                                                     |// 
//|                                                                                   |// 
//|Schroeppel-Shamir algorithm fonctionnel mais non optimale,                         |//
//|en cours de réalisation                                                            |// 
//|                                                                                   |// 
//|Source : Papier de Nick Howgrave-Graham et Antoine Joux, New generic algorithms    |//
//|for hard knapsacks                                                                 |//
//|___________________________________________________________________________________|//

/* TODO:             
-> commenter
-> changer M en mpz (voir si M en llu est opti)
-> chercher dans un fichier et saisir automatiquement le résultat dedans
-> regarder si mpz_clear recommandé en fin de prog
-> voir les "CHANGEMENT"
-> optimisation appel de Creationtab avec tabGray
*/

/*TODO Claire:
-> TS2 à remplacer par T2S2 pour avoir des pairs sur les tableaux qu'on trie
*/

#include "word.h"


//valeur de la taille de n
int WORD_SIZE = 8;

//Valeur à trouver par sommation
int TARGET = 10;


int main(){

    //On crée le tableau qui contiendra les éléments du vecteur a
    word* ai = malloc(sizeof(word)*WORD_SIZE);
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
        generationManuelle(ai);
    }
    
    //On définit et déclare les variables utilent au bon déroulement du programme

    //tailleTableauS correspond au nombre de cellule présente dans un tableau contenant une partie de toutes les sommations possible des valeurs du vecteur a
    // (2^(WORD_SIZE/4))*4 cellules
    unsigned long long tailleTableauS = (1ULL<<(WORD_SIZE/4));

    //tableaux contenant à eux 4 toutes les sommations possible des valeurs du vecteur a
    word* T1S = malloc(sizeof(word)*tailleTableauS);
    word* T2S = malloc(sizeof(word)*tailleTableauS);
    word* T3S = malloc(sizeof(word)*tailleTableauS);
    word* T4S = malloc(sizeof(word)*tailleTableauS);

    //Tableau contenant les valeurs du code de gray
    word* tabGray = malloc(sizeof(word)*tailleTableauS);

    //Initialisation
    for(int i=0;i<tailleTableauS;++i){
        mpz_init(T1S[i]);
        mpz_init(T2S[i]);
        mpz_init(T3S[i]);
        mpz_init(T4S[i]);
        mpz_init(tabGray[i]);
    }

    ListeSolConca Solution = NULL;

    word s; //TARGETSUM
    mpz_init_set_ui(s,TARGET);

    Solution = Schroeppel_Shamir(ai,s,T1S,T2S,T3S,T4S,tabGray,Solution);
    if(Solution == NULL){
        return 1;
    }

    //Verification de sol (while pour effectuer toutes les vérif)

    //verificationSol(ai,s,Solution->valeur);


    //Libération de la mémoire
    liberer(ai,T1S,T2S,T3S,T4S,tabGray,Solution,tailleTableauS);
    mpz_clear(s);

}
