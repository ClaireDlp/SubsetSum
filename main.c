#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

//valeur de la taille de n
#define WORD_SIZE 32

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
    Liste3 lst = malloc(sizeof(triple)+sizeof(void*)); //revoir
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
    ListeSol lst = malloc(sizeof(void*)+sizeof(solution));
    lst->valeur = s;
    lst->suivant = L;
    printf("\nReussite");
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


void triParDenombrement(pair* T, pair* RES, unsigned long long M, unsigned long long tailleT){
    //  unsigned long long C[M];
    unsigned long long* C = malloc(sizeof(unsigned long long)*M);
    //pair RES[tailleT];

    for (unsigned long long i = 0; i < tailleT; ++i){
        mpz_init(RES[i].word);
    }

    for (unsigned long long i = 0; i < M; ++i){
        //mpz_init(C[i].word);
        C[i] = 0;
    }
    for (unsigned long long i = 0; i < tailleT; ++i){
        //mpz_add_ui(C[mpz_get_ui(T[i].word)].word),C[mpz_get_ui(T[i].word)].word),1);
        // mpz_get_ui
        C[mpz_get_ui(T[i].word)] = C[mpz_get_ui(T[i].word)] + 1;
    }
    for (unsigned long long i = 1; i < M; ++i){
        C[i] = C[i] + C[i-1];
    }
    for (unsigned long long i = 0; i < tailleT; ++i){
        //mpz_add_ui(C[mpz_get_ui(T[i].word)].word),C[mpz_get_ui(T[i].word)].word),1);
        // mpz_get_ui
        mpz_set(RES[C[mpz_get_ui(T[i].word)]-1].word,T[i].word);
        RES[C[mpz_get_ui(T[i].word)]-1].indexe = T[i].indexe;
        C[mpz_get_ui(T[i].word)] = C[mpz_get_ui(T[i].word)] -1;
    }
    printf("\n-------------------------TRI FAIT");
    for (unsigned long long i = 0; i < tailleT; i++){
        gmp_printf("\n RES[i].word : %Zd   RES[i].indexe : %llu",RES[i].word,RES[i].indexe);
    } 
}

//Algo3(T1S,T2S,T3S,T4S,10);
ListeSol Algo3(word* T1S, word* T2S, word* T3S, word* T4S, word TargetSum){
    // word M;
    // //M = (1<<WORD_SIZE);
    // mpz_init_set_ui(M,1);
    // mpz_mul_2exp(M,M,WORD_SIZE);
    // ////
    // word tailleTableauS;
    // //tailleTableauS = (1<<(WORD_SIZE/4));
    // mpz_init_set_ui(tailleTableauS,1);
    // mpz_mul_2exp(tailleTableauS,tailleTableauS,WORD_SIZE/4);

    unsigned long long M = (1ULL<<WORD_SIZE);
    unsigned long long tailleTableauS = (1<<(WORD_SIZE/4));
    printf("\n M : %llu  tailleTableauS : %llu  ",M, tailleTableauS);

    pair T2S2[tailleTableauS];
    for (unsigned long long i = 0; i < tailleTableauS; ++i){
        mpz_init(T2S2[i].word);
        // T2S2[i].word = T2S[i] % M;
        mpz_mod_ui(T2S2[i].word,T2S[i],M);
        T2S2[i].indexe = i;
    }

    pair T4S2[tailleTableauS];
    for (unsigned long long i = 0; i < tailleTableauS; i++){
        mpz_init(T4S2[i].word);
        // T4S2[i].word = T4S[i] % M;
        mpz_mod_ui(T4S2[i].word,T4S[i],M);
        T4S2[i].indexe = i; 
    }

    printf("\n-------------------------%llu",M);
    for (unsigned long long i = 0; i < tailleTableauS; i++){
        gmp_printf("\n T2S2[i].word : %Zd   T2S[i] : %Zd",T2S2[i].word,T2S[i]);
        printf("  T2S2[i].indexe : %llu",T2S2[i].indexe);
        gmp_printf("\n T4S2[i].word : %Zd   T4S[i] : %Zd",T4S2[i].word,T4S[i]);
        printf("  T4S2[i].indexe : %llu",T4S2[i].indexe);
    }
    //fonctionne


    pair* RES1 = malloc(sizeof(pair)*tailleTableauS);
    pair* RES2 = malloc(sizeof(pair)*tailleTableauS);
    //TRIE : Répartition uniforme des éléments (a revoir fonction aléatoire)
    triParDenombrement(T2S2,RES1,M,tailleTableauS);
    triParDenombrement(T4S2,RES2,M,tailleTableauS);


    //SOL
    ListeSol SOL = NULL;

    printf("\n--------------------------------ENTREE BOUCLE om");

    Liste3 S1 = NULL;
    for(unsigned long long om = 0; om < M; om++){
        S1 = NULL;

        for(unsigned long long i = 1; i < tailleTableauS; i++){
            word ol;
            mpz_init_set(ol,T1S[i]);

            word tmp_w;
            mpz_init_set_ui(tmp_w,om);
            mpz_sub(tmp_w,tmp_w,ol);
            mpz_mod_ui(tmp_w,tmp_w,M);
            unsigned long long ot = mpz_get_ui(tmp_w);

            for (unsigned long long j = 0; j < tailleTableauS; j++)
            {
                if(mpz_get_ui(RES1[j].word)==ot){
                    triple t;
                    t.i = i; //a chnager la notation
                    t.j = RES1[j].indexe;
                    mpz_init(t.word);
                    mpz_add(t.word,T2S[j]/*RES1[j].word*/,ol); //Revoir les valeurs
                    S1 = AjouterListe3(t,S1);
                }
            }
        }
        //on affiche
        Liste3 parcours = S1;
        if(parcours!=NULL){
            while(parcours->suivant!=NULL){
                parcours = parcours->suivant;
            }
            //SI = NULL
            printf("\n%llu",parcours->valeur.j);
        }



        // -------------------------- 

        //TRIER LISTE S1 A FAIRE ICI !!!!!!!!!!!!!!!!!!!

        // --------------------------

        for(unsigned long long i = 1; i < tailleTableauS; i++){
            word ol;
            mpz_init_set(ol,T3S[i]);

            word tmp_w;
            mpz_init_set_ui(tmp_w,om);
            mpz_sub(tmp_w,tmp_w,ol);
            mpz_sub(tmp_w,TargetSum,tmp_w);
            mpz_mod_ui(tmp_w,tmp_w,M);
            unsigned long long ot = mpz_get_ui(tmp_w);
            

            for (unsigned long long j = 0; j < tailleTableauS; j++)
            {
                if(mpz_get_ui(RES2[j].word)==ot){
                    word Tprime;
                    word T;
                    mpz_init(T);
                    mpz_sub(T,TargetSum,T3S[j]);
                    mpz_sub(T,T,T4S[j]);
                    mpz_init_set(Tprime,T);
                    printf("\n TPRIME : %lu",mpz_get_ui(Tprime));
                    Liste3 parcoursS1 = S1;
                    if(parcoursS1!=NULL){ //Ameliorer synthaxe...
                        if(parcoursS1->suivant==NULL){
                            if(mpz_get_ui(parcoursS1->valeur.word)==mpz_get_ui(Tprime)){ //A optimiser (Ne pas transformer en ull...)
                                solution s;
                                s.i = parcoursS1->valeur.i;
                                s.j = parcoursS1->valeur.j;
                                s.k = i;
                                s.l = RES2[j].indexe;
                                SOL = AjouterListeSol(s,SOL);
                            }
                        }
                        else{
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
                            
                            if(parcoursS1->suivant==NULL){
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
            }
        }
    }
    return SOL;
}

void Algo1(word* ai, word s, word* T, word* T1S, word* T2S, word* T3S, word* T4S, word* T1x, word* T2x, word* T3x, word* T4x){
    CreationT(T1S,T1x,ai,0);
    CreationT(T2S,T2x,ai,WORD_SIZE/4);
    CreationT(T3S,T3x,ai,WORD_SIZE/2);
    CreationT(T4S,T4x,ai,3*(WORD_SIZE/4));
    ListeSol SOL = Algo3(T1S, T2S, T3S, T4S, s);
    //FAIRE LA CONCATENATION
    if(SOL!=NULL){
        while(SOL!=NULL){
            gmp_printf("\n %Zd %Zd %Zd %Zd",T1x[SOL->valeur.i],T2x[SOL->valeur.j],T3x[SOL->valeur.k],T4x[SOL->valeur.l]);
            SOL = SOL->suivant;
        }
        gmp_printf("\n %Zd %Zd %Zd %Zd",T1x[SOL->valeur.i],T2x[SOL->valeur.j],T3x[SOL->valeur.k],T4x[SOL->valeur.l]);
    }
    else{
        printf("\nPAS DE SOLUTIONS");
    }
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
        //generationManuel();  // + chercher dans fichier et saisir le résultat dedans
    }


    //---------------------ALORITHME 1----------------------------
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


    word* T = malloc(sizeof(word)*tailleTableauS*4);
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

    word* T1x = malloc(sizeof(word)*tailleTableaux);
    word* T2x = malloc(sizeof(word)*tailleTableaux);
    word* T3x = malloc(sizeof(word)*tailleTableaux);
    word* T4x = malloc(sizeof(word)*tailleTableaux);
    for(int i=0;i<tailleTableaux;++i){
        mpz_init(T1x[i]);
        mpz_init(T2x[i]);
        mpz_init(T3x[i]);
        mpz_init(T4x[i]);
    }

    word s; //TARGETSUM
    mpz_init(s);
    mpz_set_ui(s,10);

    Algo1(ai,s,T,T1S,T2S,T3S,T4S,T1x,T2x,T3x,T4x);

    //Dans une fonction
    for(int i=0;i<WORD_SIZE;++i){
        mpz_clear(ai[i]);
    }

    //FAIRE LES FREE, METTRE EN MALLOC, CHANGER M EN GMP
    // + chercher dans fichier et saisir le résultat dedans
}
