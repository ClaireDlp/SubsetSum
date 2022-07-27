//|___________________________________________________________________________________|//
//|Informations :                                                                     |// 
//|                                                                                   |// 
//|Schroeppel-Shamir algorithm fonctionnel, mais non optimale.                        |// 
//|                                                                                   |// 
//|En cours de réalisation                                                            |// 
//|                                                                                   |// 
//|Source : Papier de Nick Howgrave-Graham et Antoine Joux, New generic algorithms    |//
//|for hard knapsacks                                                                 |//
//|___________________________________________________________________________________|//

#include "word.h"

//valeur de la taille de n
int WORD_SIZE = 8;


int main(){
    //Valeur recherché par sommation des éléments a
    word TARGET;

    char tmp[255]; int dim;

    //Choix de la dimension
    printf("\nQuel est la dimension ? 0 pour ignorer\n");
    scanf("%d",&dim);
    if(dim!=0){
       WORD_SIZE = dim; 
    }
    //Valeur à trouver par sommation
    printf("\nQuel est le résultat de la sommation attendu ?\n");
    scanf("%s",tmp);
    mpz_init_set_str(TARGET,tmp,10);

    //On déclare le tableau qui contiendra les éléments du vecteur a
    word* ai = NULL; 

    //On génére les éléments du vecteur a
    bool choixDefDeA = true;

    choixDefDeA = choixDeA();
    if(choixDefDeA){
        generationAleaA(&ai);
    }
    else{
        generationManuelle(&ai);
    }

    
    //On définit et déclare les variables utilent au bon déroulement du programme

    //tailleTableauS correspond au nombre de cellule présente dans un tableau contenant une partie de toutes les sommations possible des valeurs du vecteur a
    // (2^(WORD_SIZE/4))*4 cellules
    unsigned long long tailleTableauS = (1ULL<<(WORD_SIZE/4));

    //Si WORD_SIZE n'est pas divisible par 4
    if(WORD_SIZE%4!=0){
        tailleTableauS = (1ULL<<(WORD_SIZE/4+1)); //On augmente de 1 tailleTableauS afin de permettre l'utilisation du programme sur une dimension non divisible par 4
    }

    //Tableaux contenant à eux 4 toutes les sommations possible des valeurs du vecteur a
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

    Solution = Schroeppel_Shamir(ai,TARGET,T1S,T2S,T3S,T4S,tabGray,Solution);
    if(Solution == NULL){
        return 1;
    }

    //Verification de sol (while pour effectuer toutes les vérif)
    //verificationSol(ai,TARGET,Solution->valeur,tabGray);

    //Libération de la mémoire
    liberer(ai,T1S,T2S,T3S,T4S,tabGray,Solution,tailleTableauS);
    mpz_clear(TARGET);


    /* TODO:             
    -> voir les "CHANGEMENT"
    -> optimisation appel de Creationtab avec tabGray
    -> TS2 à remplacer par T2S2 pour avoir des pairs sur les tableaux qu'on trie
    -> for each dans la fonction merge, avec une dichotomie
    -> regler le choix de dimension au debut du programme -> probleme à la lecture d'un fichier : faire un realloc
    */
}
