#include <stdio.h>
#include <math.h>

#define NB_CODE_GRAY 64

//Pour obtenir dans l'ordre les valeurs du code de gray nous allons partir d'un principe décrit sur ce lien Wikipédia : https://fr.wikipedia.org/wiki/Code_de_Gray qui nous dit :
// "Une méthode de calcul permettant de passer d'un nombre de Gray au suivant, et qui présente l'avantage de ne pas nécessiter de connaître l'ensemble des nombres précédents est la suivante :
// si le nombre de 1 est pair, il faut inverser le dernier chiffre.
// si le nombre de 1 est impair, il faut inverser le chiffre situé à gauche du 1 le plus à droite."

//Fonction qui permet de chercher le nombre de bit à 1 dans un entier
int nombreDe1(int n, int cpt){
    if(n==0){
        return cpt;
    }
    else{
        int i = floor(log2(n));
        n = n - (1<<i);
        return nombreDe1(n,cpt+1);
    }
}

//Fonction qui inverse inverse le bit situé à gauche du bit 1 le plus à droite dans un entier
int inverse(int n){
    int nb = n;
    while(round(log2(nb)) - log2(nb) != 0){
        int i = floor(log2(nb));
        nb = nb - (1<<i);
    }
    int ntest = n;
    if((ntest|(nb*2))==n+nb*2){
        return n+nb*2;
    }
    return n-nb*2;
}

int main(){
    int nb = 0;
    for(int i = 0; i<NB_CODE_GRAY;++i){
        int nb1 = nombreDe1(nb,0);
        printf("\n%d",nb); //Affichage du résultat
        if(nb1 % 2 == 0){
            if(nb % 2 == 0){
                nb++;
            }
            else{
                nb--;
            }
        }
        else{
            nb = inverse(nb);
        }
    }
}
