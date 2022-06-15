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
-> fonction de test                 
-> compilation séparée + makefile
-> recherche plus efficace dans algo3
*/


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>


//valeur de la taille de n
#define WORD_SIZE 8

//Valeur à trouver par sommation
#define TARGET 10

//Activer l'affichage
#define DEBUG 0

typedef short bool;
#define true 1
#define false 0

//Représentation d'un mot
typedef mpz_t word;

//Définition d'un type permettant de retourner plusieurs informations à la suite de l'utilisation de la fonction gray
typedef struct{ 
    unsigned long long n; // Représente le code de gray actuel
    unsigned long long nPrec; // Représente le code de gray antérieur à l'appel de la fonction gray
    //CHANGEMENT : short suffit pour les deux variables qui précédent
    unsigned long long bitChangement; // Précise l'emplacement du bit modifié
    unsigned long long signe; // Précise si il y a eu un retrait ou ajout de bit à 1
} graytab;

//Définition du type pair : Utile pour stoker en pair un mot et son indexe dans son tableau
typedef struct
{
    word word;
    unsigned long long indexe;
}pair;

//Définition du type pair : Utile pour stoker en pair un mot, et un couple d'indexe représentant 
//l'emplacement dans leur deux tableaux respectives des deux valeurs somme du mot
typedef struct
{
    word word;
    unsigned long long i;
    unsigned long long j;
}triple;


typedef struct cellule{
    triple valeur;
    struct cellule* suivant;
} *Liste3;

Liste3 AjouterListe3(triple t, Liste3 L){
    Liste3 lst = malloc(sizeof(struct cellule));
    lst->valeur = t;
    lst->suivant = L;
    return lst;
}

typedef struct{
    unsigned long long i;
    unsigned long long j;
    unsigned long long k;
    unsigned long long l;
} solution;

typedef struct cellule2{
    solution valeur;
    struct cellule2* suivant;
} *ListeSol;

ListeSol AjouterListeSol(solution s, ListeSol L){
    ListeSol lst = malloc(sizeof(struct cellule2));
    lst->valeur = s;
    lst->suivant = L;
    return lst;
}


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

//Fonctions qui effectuent les opérations utilent au déroulement du programme

    //Fonctions permettant de connaître le code de gray de sa valeur parrallèle entière (Ancienne version : pas optimisé, avantage : pas besoin d'avoir l'information du code de gray précédent)

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
        //         nb = nb - (1<<i);q
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

    //Fonctions permettant de connaître le code de gray de sa valeur parrallèle entière (inconvénient : besoin de connaître la valeur du code de gray précédent)

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


//Regarder cette fonction pour optimiser nombreDe0gauche()

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



    //ERREUR : quitter le prog si erreur..
    void liberation(word* w,unsigned long long N){
        for (unsigned long long i = 0; i < N; ++i){
            mpz_clear(w[i]);
        }
        free(w);
    }

    //CHANGEMENT : Voir si on peut associer cette fonction à liberation()
    void liberationPair(pair* w,unsigned long long N){
        for (unsigned long long i = 0; i < N; ++i){
            mpz_clear(w[i].word);
        }
        free(w);
    }

    void liberer(word* ai,word* T1S,word* T2S,word* T3S,word* T4S,word* T1x,word* T2x,word* T3x,word* T4x,unsigned long long N){
        liberation(ai,N);
        liberation(T1S,N);
        liberation(T2S,N);
        liberation(T3S,N);
        liberation(T4S,N);
        liberation(T1x,N);
        liberation(T2x,N);
        liberation(T3x,N);
        liberation(T4x,N);
    }


    //Fonction qui permet de créer les différents tableaux utilisé dans l'algorithme de Schroeppel-Shamir algotithm
    void CreationT(word* TS, word* TX, word* ai, unsigned long long place){

        graytab tab; // n, nprec, bitchangement et pos ou neg
        tab.n = 0;
        tab.nPrec = 0;
        tab.bitChangement = 0;
        tab.signe = 0;

        mpz_set_ui(TX[0],0);
        //mpz_set_ui(TX[i],0);

        for(unsigned long long i = 0;i<(1ULL<<(WORD_SIZE/4));++i){
            if(tab.n!=0){

                // //On cherche l'information de quel bit a changé entre l'actuel représentation de gray, et l'ancienne (boucle précédente)
                // bitChangement = n ^ nprec;

                // //On cherche à connaître le bit associé à la valeur à soustraire ou ajouter
                // bitChangement = emplacementBit(bitChangement);

                //On cherche l'actuel code de gray associé à l'entier i
                tab = gray(i,tab);

                //Condition permettant de savoir s'il faut ajouter ou soustraire 
                if(tab.signe==0){
                    //TS[i]= TS[i-1] +ai[bitChangement-1+place];
                    mpz_add(TS[i],TS[i-1],ai[tab.bitChangement-1+place]);
                    //TX[i] = n;
                    mpz_set_ui(TX[i],tab.n);
                }
                else{
                    mpz_sub(TS[i],TS[i-1],ai[tab.bitChangement-1+place]);
                    //TX[i] = n;
                    mpz_set_ui(TX[i],tab.n);
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
        if(DEBUG){
            printf("\n CREATION DE T : ");
            for(unsigned long long i = 0;i<(1<<(WORD_SIZE/4));++i){
                gmp_printf("\nTS[%llu]=%Zd TX[%llu]=%Zd",i,TS[i],i,TX[i]);
                //gmp_printf("\n%Zd",TS[i]);s
            }
        }  
    }

    //Fonction de tri par denombrement
    void triParDenombrement(word* TS, pair* T, pair* RES, unsigned long long M, unsigned long long tailleT){
        
        //CHANGEMENT
        unsigned long long* C = malloc(sizeof(unsigned long long)*M);

        word* TStrie = malloc(sizeof(word)*tailleT);

        for (unsigned long long i = 0; i < tailleT; ++i){
            mpz_init(RES[i].word);
            mpz_init(TStrie[i]);
        }

        for (unsigned long long i = 0; i < M; ++i){
            C[i] = 0;
        }

        for (unsigned long long i = 0; i < tailleT; ++i){
            C[mpz_get_ui(T[i].word)] = C[mpz_get_ui(T[i].word)] + 1;
        }

        for (unsigned long long i = 1; i < M; ++i){
            C[i] = C[i] + C[i-1];
        }

        for (unsigned long long i = 0; i < tailleT; ++i){
            mpz_set(RES[C[mpz_get_ui(T[i].word)]-1].word,T[i].word);
            RES[C[mpz_get_ui(T[i].word)]-1].indexe = T[i].indexe;
            C[mpz_get_ui(T[i].word)] = C[mpz_get_ui(T[i].word)] -1;
        }


        //CHANGEMENT : OPTIMISER EN MEMOIRE : trie de TS
        for (unsigned long long i = 0; i < tailleT; ++i){
            mpz_set(TStrie[i],TS[RES[i].indexe]);
        }
        for (unsigned long long i = 0; i < tailleT; ++i){
            mpz_set(TS[i],TStrie[i]);
        }

        if(DEBUG){
            printf("\n-------------------------TRI FAIT");
            for (unsigned long long i = 0; i < tailleT; i++){
                gmp_printf("\n RES[i].word : %Zd   RES[i].indexe : %llu",RES[i].word,RES[i].indexe);
                gmp_printf("\n tstrie[i] : %Zd",TStrie[i]);
            }
        }

        //Libération de l'espace libre
        liberation(TStrie,tailleT);
        free(C);
    }


    //Fonction qui permet de concaténer 4 valeur word et affiche une solution de l'algorithme de Schroeppel-Shamir
    //CHANGEMENT : valeur à retourner
    void concatenation(word w1, word w2, word w3, word w4, unsigned long long taille){
        word res, ww1, ww2, ww3;
        mpz_inits(res,ww1,ww2,ww3,NULL);

        mpz_mul_2exp(ww1,w1,taille*3);
        mpz_mul_2exp(ww2,w2,taille*2);
        mpz_mul_2exp(ww3,w3,taille);


        mpz_ior(res,ww1,ww2);
        mpz_ior(res,res,ww3);
        mpz_ior(res,res,w4);
        gmp_printf(" solution : %Zd",res);

        //Libération de l'espace libre
        mpz_clears(res,ww1,ww2,ww3,NULL);
    }


    Liste3 join(Liste3 S1, word* T1S, word* T2S, pair* RES1, unsigned long long tailleTableauS, unsigned long long M, unsigned long long om){
        S1 = NULL;
        word ol, tmp_w;
        mpz_inits(ol,tmp_w,NULL);
        for(unsigned long long i = 0; i < tailleTableauS; i++){
            mpz_set(ol,T1S[i]);
            mpz_set_ui(tmp_w,om);
            mpz_sub(tmp_w,tmp_w,ol);
            mpz_mod_ui(tmp_w,tmp_w,M);
            unsigned long long ot = mpz_get_ui(tmp_w);
            for (unsigned long long j = 0; j < tailleTableauS; j++){ //Claire: remplacer par recherche plus efficace, la bonne valeur devrait être autour de RES1[ot]
                if(mpz_get_ui(RES1[j].word)==ot){
                    triple t;
                    t.i = i;
                    t.j = RES1[j].indexe;
                    mpz_init(t.word);
                    mpz_add(t.word,T2S[j],ol);
                    S1 = AjouterListe3(t,S1);
                }
            }
        }
        
        //affichage
        if(DEBUG){
            Liste3 parcours = S1;
            if(parcours!=NULL){
                printf("\nListe %llu",om);
                while(parcours->suivant!=NULL){
                    gmp_printf("\n%llu %llu %Zd",parcours->valeur.j, parcours->valeur.i, parcours->valeur.word);
                    parcours = parcours->suivant;
                }
                gmp_printf("\n%llu %llu %Zd",parcours->valeur.j, parcours->valeur.i, parcours->valeur.word);
            }
        }

        //Liération de l'espace libre
        mpz_clears(ol,tmp_w,NULL);
        
        return S1;
    }

    ListeSol solver(ListeSol SOL,Liste3 S1, word* T3S, word* T4S, pair* RES2, unsigned long long tailleTableauS,word TargetSum, unsigned long long M, unsigned long long om){
        word ol, tmp_w, Tprime;
        mpz_inits(ol,tmp_w,Tprime,NULL);
        
        for(unsigned long long i = 0; i < tailleTableauS; i++){
            mpz_set(ol,T3S[i]);
            mpz_set_ui(tmp_w,om);
            // mpz_sub(tmp_w,tmp_w,ol);
            // mpz_sub(tmp_w,TargetSum,tmp_w);
            // mpz_mod_ui(tmp_w,tmp_w,M);
            // unsigned long long ot = mpz_get_ui(tmp_w);

            //CHANGEMENT
            unsigned long long ot = (mpz_get_ui(TargetSum)-om-mpz_get_ui(ol)) % M;

            for (unsigned long long j = 0; j < tailleTableauS; j++)
            {
                if(mpz_get_ui(RES2[j].word)==ot){
                    // word Tprime;
                    // word T;
                    // mpz_init(T);
                    // mpz_sub(T,TargetSum,ol); //CHANGEMENT  : T3S[j]
                    // mpz_sub(T,T,T4S[j]);
                    // mpz_init_set(Tprime,T);

                    //CHANGEMENT
                    unsigned long long T = mpz_get_ui(TargetSum)-mpz_get_ui(ol)-mpz_get_ui(T4S[j]);
                    //mpz_init(Tprime);
                    mpz_set_ui(Tprime,T);


                    Liste3 parcoursS1 = S1;
                    if(parcoursS1!=NULL){
                        while(parcoursS1->suivant!=NULL){
                            if(mpz_get_ui(parcoursS1->valeur.word)==mpz_get_ui(Tprime)){ //A optimiser (Ne pas transformer en ull...)
                                solution s;
                                s.i = parcoursS1->valeur.i;
                                s.j = parcoursS1->valeur.j;
                                s.k = i;
                                s.l = RES2[j].indexe;
                                SOL = AjouterListeSol(s,SOL);
                            }
                            parcoursS1 = parcoursS1->suivant;
                        }
                        if(mpz_get_ui(parcoursS1->valeur.word)==mpz_get_ui(Tprime)){ //A optimiser (Ne pas transformer en ull...)
                            solution s;
                            s.i = parcoursS1->valeur.i;
                            s.j = parcoursS1->valeur.j;
                            s.k = i;
                            s.l = RES2[j].indexe;
                            SOL = AjouterListeSol(s,SOL);
                        }
                    }
                }
            }
        }
        mpz_clears(ol,tmp_w,Tprime,NULL);
        return SOL;
    }



    //Algorithme 3 Modular 4-Way merge routine
    ListeSol Algo3(word* T1S, word* T2S, word* T3S, word* T4S, word TargetSum, ListeSol SOL){

        //CHANGEMENT
        unsigned long long M = (1ULL<<(WORD_SIZE/4));
        unsigned long long tailleTableauS = (1ULL<<(WORD_SIZE/4));

        pair* T2S2 = malloc(sizeof(pair)*tailleTableauS);
        for (unsigned long long i = 0; i < tailleTableauS; ++i){
            mpz_init(T2S2[i].word);
            mpz_mod_ui(T2S2[i].word,T2S[i],M);
            T2S2[i].indexe = i;
        }

        pair* T4S2 = malloc(sizeof(pair)*tailleTableauS);
        for (unsigned long long i = 0; i < tailleTableauS; i++){
            mpz_init(T4S2[i].word);
            mpz_mod_ui(T4S2[i].word,T4S[i],M);
            T4S2[i].indexe = i; 
        }


        pair* RES1 = malloc(sizeof(pair)*tailleTableauS);
        pair* RES2 = malloc(sizeof(pair)*tailleTableauS);

        //CHANGEMENT : Regarder si on peut éviter de déclarer T2S2 et T4S2 pour gain de mémoire et calcul
        //TRIE : Répartition uniforme des éléments (a revoir fonction aléatoire)
        triParDenombrement(T2S,T2S2,RES1,M,tailleTableauS);
        liberationPair(T2S2,tailleTableauS);
        triParDenombrement(T4S,T4S2,RES2,M,tailleTableauS);
        liberationPair(T4S2,tailleTableauS);

        Liste3 S1 = NULL;
        for(unsigned long long om = 0; om < M; om++){
            
            S1 = join(S1,T1S,T2S,RES1,tailleTableauS,M,om);

            //CHANGEMENT : TRIER LISTE S1 -> optimisation
            
            SOL = solver(SOL,S1,T3S,T4S,RES2,tailleTableauS,TargetSum,M,om);
        }

        //Libération de la mémoire
        liberationPair(RES1,tailleTableauS);
        liberationPair(RES2,tailleTableauS);

        if(S1!=NULL){
            while(S1->suivant!=NULL){
                mpz_clear(S1->valeur.word);
                S1 = S1->suivant;
            }
            mpz_clear(S1->valeur.word);
        }
        free(S1);

        return SOL;
    }

    //Schroeppel-Shamir algorithm
    void Schroeppel_Shamir(word* ai, word s, word* T1S, word* T2S, word* T3S, word* T4S, word* T1x, word* T2x, word* T3x, word* T4x){
        
        CreationT(T1S,T1x,ai,0);
        CreationT(T2S,T2x,ai,WORD_SIZE/4);
        CreationT(T3S,T3x,ai,WORD_SIZE/2);
        CreationT(T4S,T4x,ai,3*(WORD_SIZE/4));

        ListeSol SOL = NULL;
        SOL = Algo3(T1S, T2S, T3S, T4S, s, SOL);

        //On affiche les solutions, à partir de leur code de gray, concaténation à effectuer
        //CHANGEMENT Faire afficher les résultats en code binaire classique
        if(SOL!=NULL){
            while(SOL->suivant!=NULL){
                gmp_printf("\n %Zd %Zd %Zd %Zd",T1x[SOL->valeur.i],T2x[SOL->valeur.j],T3x[SOL->valeur.k],T4x[SOL->valeur.l]);
                concatenation(T1x[SOL->valeur.i],T2x[SOL->valeur.j],T3x[SOL->valeur.k],T4x[SOL->valeur.l],WORD_SIZE/4);
                SOL = SOL->suivant;
            }
            gmp_printf("\n %Zd %Zd %Zd %Zd",T1x[SOL->valeur.i],T2x[SOL->valeur.j],T3x[SOL->valeur.k],T4x[SOL->valeur.l]);
            concatenation(T1x[SOL->valeur.i],T2x[SOL->valeur.j],T3x[SOL->valeur.k],T4x[SOL->valeur.l],WORD_SIZE/4);
        }
        else{
            printf("\nPAS DE SOLUTIONS");
        }
        free(SOL);
    }


int main(){

    //On crée le tableau qui contiendra les éléments du vecteur a
    //word ai[WORD_SIZE];
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
    

    unsigned long long tailleTableauS = (1ULL<<(WORD_SIZE/4));

    word* T1S = malloc(sizeof(word)*tailleTableauS);
    word* T2S = malloc(sizeof(word)*tailleTableauS);
    word* T3S = malloc(sizeof(word)*tailleTableauS);
    word* T4S = malloc(sizeof(word)*tailleTableauS);

    for(int i=0;i<tailleTableauS;++i){
        mpz_init(T1S[i]);
        mpz_init(T2S[i]);
        mpz_init(T3S[i]);
        mpz_init(T4S[i]);
    }

    word* T1x = malloc(sizeof(word)*tailleTableauS);
    word* T2x = malloc(sizeof(word)*tailleTableauS);
    word* T3x = malloc(sizeof(word)*tailleTableauS);
    word* T4x = malloc(sizeof(word)*tailleTableauS);
    for(int i=0;i<tailleTableauS;++i){
        mpz_init(T1x[i]);
        mpz_init(T2x[i]);
        mpz_init(T3x[i]);
        mpz_init(T4x[i]);
    }

    word s; //TARGETSUM
    mpz_init_set_ui(s,TARGET);

    Schroeppel_Shamir(ai,s,T1S,T2S,T3S,T4S,T1x,T2x,T3x,T4x);

    //Libération de la mémoire
    mpz_clear(s);
    liberer(ai,T1S,T2S,T3S,T4S,T1x,T2x,T3x,T4x,tailleTableauS);


    //FAIRE LES FREE, METTRE EN MALLOC, CHANGER M EN GMP (voir si M en llu est opti)
    // + chercher dans fichier et saisir le résultat dedans

    //S1 A TRANSFORMER EN TABLEAU   -> gain de temps

    //regarder si mpz_clear recommandé en fin de prog
}
