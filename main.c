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
-> recherche plus efficace dans algo3
-> transformer S1 en tableau
-> commenter
-> changer M en mpz (voir si M en llu est opti)
-> chercher dans un fichier et saisir automatiquement le résultat dedans
-> regarder si mpz_clear recommandé en fin de prog
-> voir les "CHANGEMENT"
*/

/*TODO Claire:
-> TS2 à remplacer par T2S2 pour avoir des pairs sur les tableaux qu'on trie
-> Garder 1 seul TX qui correspond au Code de Gray (Nécessaire?)
*/

#include "word.h"

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
        //generationManuel();  //CHANGEMENT + chercher dans fichier et saisir le résultat dedans
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

    //Tableau contenant 
    word* T1x = malloc(sizeof(word)*tailleTableauS);
    word* T2x = malloc(sizeof(word)*tailleTableauS);
    word* T3x = malloc(sizeof(word)*tailleTableauS);
    word* T4x = malloc(sizeof(word)*tailleTableauS);

    //Initialisation
    for(int i=0;i<tailleTableauS;++i){
        mpz_init(T1S[i]);
        mpz_init(T2S[i]);
        mpz_init(T3S[i]);
        mpz_init(T4S[i]);
        mpz_init(T1x[i]);
        mpz_init(T2x[i]);
        mpz_init(T3x[i]);
        mpz_init(T4x[i]);
    }

    ListeSolConca Solution = NULL;

    word s; //TARGETSUM
    mpz_init_set_ui(s,TARGET);

    Solution = Schroeppel_Shamir(ai,s,T1S,T2S,T3S,T4S,T1x,T2x,T3x,T4x,Solution);


    //Verification de sol (while pour effectuer toutes les vérif)

   // verificationSol(ai,s,Solution->valeur);


    //Libération de la mémoire
    liberer(ai,T1S,T2S,T3S,T4S,T1x,T2x,T3x,T4x,Solution,tailleTableauS);
    mpz_clear(s);
}
