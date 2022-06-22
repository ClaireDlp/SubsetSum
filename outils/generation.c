#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

//Code qui génére un fichier texte contenant
//  -Une dimension = nombre d'éléments pouvant être sommé <=> nombre d'éléments dans le vecteur a
//  -Les éléments
//  -Une somme t, pouvant être obtenu par sommation des éléments

//La dimension doit être > 2
//Un minimum de deux éléments doit être sommé pour obtenir t

int main(int argc, char **argv){

    FILE *Texte = NULL;
    
    int numeroInt;
    char destination[255] = "generation", numeroChar[255]="";


    if(argc<=1){
        printf("\nUn paramètre est attendu : \n\t  un entier correspondant à la taille n <=> nombre d'élément pouvant contribuer à la sommation <=> dimension");
        return 1;
    }

    //On choisit le nom du fichier
        printf("Numéro de la génération (correspondant au nom de fichier) : ");
        scanf("%d",&numeroInt);
        sprintf(numeroChar,"%d",numeroInt);
        strcat(destination,numeroChar);
        strcat(destination,".txt");

    //On ouvre le fichier
        Texte = fopen(destination, "w");
        if (Texte == NULL){
            printf("\nerreur");
            return EXIT_FAILURE;
        }

    //On écrit dans le fichier la dimension
        int dimensionInt;
        char texteDimension[255]="Dim: ";
        sscanf(argv[1],"%d",&dimensionInt);
        strcat(texteDimension,argv[1]);
        fputs(texteDimension,Texte);


    //On écrit dans le fichier les éléments de a générés
        char a[255]="\na: ";
        char tmp[255]="";
        mpz_t max,val;
        mpz_init(val);
        mpz_init_set_ui(max,1);
        mpz_mul_2exp(max,max,dimensionInt);
        gmp_randstate_t state;
        gmp_randinit_mt(state);
        gmp_randseed_ui(state, time(NULL));

        fputs(a,Texte);

        //Tableau qui permet de sauvegarder les éléments de a générés
        mpz_t* ai = malloc(sizeof(mpz_t)*dimensionInt);
        for (int i = 0; i < dimensionInt; i++)
        {
            mpz_init(ai[i]);
        }
        
        
        for(int i = 0; i < dimensionInt; ++i){
            mpz_urandomm(val,state,max);
            mpz_set(ai[i],val);
            //gmp_printf("\n val : %Zd",val);
            gmp_sprintf(tmp,"%Zd",val);
            fputs(tmp,Texte);
            if(i+1<dimensionInt){
                fputs(", ",Texte);
            }
        }


    //on génére un t de façon aléatoire, étant la sommation de plusieurs éléments a
        srand(time(NULL));


        //On choisit entre 2 et dimensionInt-1 éléments dans a, pour générer un target sum
        int num = ((rand() % ((dimensionInt-1)-1)) + 2);
        int objectif = 0, i = 0;
        mpz_t targetSum;
        mpz_init(targetSum);

        //Tableau qui permet de savoir si un élément a déjà été comptabilisé dans la sommation
        int* somme = calloc(dimensionInt,sizeof(int));


        while(objectif<num){
            int tmp = rand() % (dimensionInt); //Permet de rendre aléatoire le choix des éléments
            if(somme[tmp]==0){
                somme[tmp]=1;
                objectif++;
                mpz_add(targetSum,targetSum,ai[tmp]);
            }
        }

        //On écrit dans le fichier t
        fputs("\nt: ",Texte);
        gmp_sprintf(tmp,"%Zd",targetSum);
        fputs(tmp,Texte);


    //On free
        gmp_randclear(state);
        mpz_clears(max,targetSum,val,NULL);
        for (int i = 0; i < dimensionInt; i++)
        {
            mpz_clear(ai[i]);
        }
        free(ai);
        fclose(Texte);
}
