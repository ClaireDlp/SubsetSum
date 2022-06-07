#include <stdio.h>
#include <math.h>

#define NB_CODE_GRAY 64

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

//Fonction qui renvoie le nombre de gray correspondant à partir, de nombre entier correspondant au code de gray, et le code de gray correcpondant à l'entier n-1
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

int main(){
    int ia = 0; 
    for(int i = 1; i<NB_CODE_GRAY;++i){
        ia = gray(i,ia);
        printf("\n%d",ia);
    }
}
