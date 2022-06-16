#include "word.h"

//Fonctions de paramétrage avant le début des opérations du programme

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
        gmp_printf("%Zd,",ai[i]);
    }
    gmp_printf("%Zd)",ai[WORD_SIZE-1]);
}

//Fonction qui permet de générer de façon aléatoire les éléments du vecteur a
void generationAleaA(word* ai){
    srand(time(NULL));
    //int n = choixDeN();
    for(int i = 0; i<WORD_SIZE; ++i){
        //CHANGEMENT
        //%(max-min+1) + min;
        mpz_set_ui(ai[i],rand() % 10);//(1ULL<<WORD_SIZE));
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