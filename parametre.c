#include "word.h"

#include <string.h>

//Fonctions de paramétrage avant le début des opérations du programme

//Fonction qui permet de selectionner le mode de génération des éléments du vecteur a
bool choixDeA(){
    short rep = 1;
    printf("Que souhaitez vous pour a :\n\t1: le générer aléatoirement\n\t2: le générer manuellement (par l'intermédiaire d'un fichier .txt)\n");
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

    word max;
    mpz_init_set_ui(max,1);
    mpz_mul_2exp(max,max,WORD_SIZE);

    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));

    for(int i = 0; i<WORD_SIZE; ++i){
        mpz_urandomm(ai[i],state,max);
    }

    afficherA(ai);
    gmp_randclear(state);
    mpz_clear(max);


    // srand(time(NULL));
    // //int n = choixDeN();
    // for(int i = 0; i<WORD_SIZE; ++i){
    //     //CHANGEMENT
    //     //%(max-min+1) + min;
    //     mpz_set_ui(ai[i],rand() % (1ULL<<WORD_SIZE));
    // }
}


//Fonction qui permet de choisir le fichier de lecture
int choixFichier(FILE* Texte, char* destination){
    int numeroInt;
    char numeroChar[255]="";
    printf("\nNuméro de la génération (correspondant au nom de fichier) : ");
    scanf("%d",&numeroInt);
    sprintf(numeroChar,"%d",numeroInt);
    strcpy(destination,"generation");
    strcat(destination,numeroChar);
    strcat(destination,".txt");
    Texte = fopen(destination, "r");
    if (Texte == NULL){
        printf("\nErreur fichier introuvable");
        choixFichier(Texte,destination);
    }
    else{
        printf("\nFichier trouvé");
        fclose(Texte);
    }
    return 1;
}

void generationManuelle(word* ai){
    //LECTURE DU FICHIER
        FILE *Texte = NULL;
        //char* dimension = malloc(sizeof(char)*255);
        char tmp[2500] = "";


        char dimensionChar[255];
        int dimensionInt;

        char aiChar[2500];

        char destination[255];
        char* token;

        //CHANGEMENT : voir comment vérifier la présence d'un fichier au lieu d'ouvrir
        int retour = choixFichier(Texte,destination);
        Texte = fopen(destination, "r");

        if(retour == 1){
            //On lit la ligne correspondant à la dimension
            int i = 0, j = 5;
            fgets(tmp,255,Texte);
            while(tmp[j]!='\n'){
                dimensionChar[i] = tmp[j];
                j++;
                i++;
            }
            dimensionChar[i] = '\0';
            sprintf(dimensionChar,"%d",WORD_SIZE);

            
            int n = 0, flag =0, erreur=0; //saut virgule
            i = 0, j = 3;
            mpz_t elt;
            mpz_init(elt);
            fgets(tmp,2500,Texte);
            while(1){
                if(tmp[j]>=48 && tmp[j] <=57){
                    aiChar[i] = tmp[j];
                    j++;
                    i++;
                }
                else{
                    if(tmp[j]=='\n'){
                        flag =1;
                    }
                    aiChar[i] = '\0';


                    erreur = mpz_set_str(ai[n],aiChar,10);
                    if(erreur == -1){
                        printf("\nerreur");
                        break;
                    }
                    n++;
                    j = j+2; //on passe , et espace
                    for (int k = 0; k < i; k++) //on vide les cellules remplies
                    {
                        aiChar[k]= 0;
                    }
                    i = 0;
                }
                if(flag){
                    break;
                }
            }
            mpz_clear(elt);
            fclose(Texte);
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