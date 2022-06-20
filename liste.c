#include "word.h"

// Liste3 AjouterListe3(triple t, Liste3 L){
//     Liste3 lst = malloc(sizeof(struct cellule));
//     lst->valeur = t;
//     lst->suivant = L;
//     return lst;
// }

ListeSol AjouterListeSol(solution s, ListeSol L){
    ListeSol lst = malloc(sizeof(struct cellule2));
    lst->valeur = s;
    lst->suivant = L;
    return lst;
}
ListeSolConca AjouterListeSolConca(word w, ListeSolConca L){
    ListeSolConca lst = malloc(sizeof(struct cellule3));
    mpz_init_set(lst->valeur,w);
    lst->suivant = L;
    return lst;
}