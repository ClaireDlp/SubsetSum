/*Fonction qui parcourt L entre start et end et qui pour chacune des 2^l 
 *valeurs possibles de x , compte le nombre d'éléments de L qui vaut x mod 2^l
 *
 *Le resultat est stoké dans le tableau w*/
void F2list_count_low(size_t *w, F2list *L, size_t l, size_t start, size_t end){
  F2vector low;
  size_t i;
  size_t N;

  assert(l <= (size_t)L->ncols);
  N = (0x1 << l);
  //assert(N <= (size_t)L->nrows);

  /*initialisation de w*/
  for(i = 0; i < N; i++){
    w[i] = 0;
  }

  /* on parcourt les entrées de L*/
  for(i = start; i < end; i++){
    low = F2list_entry_low(L, i, l);
    w[low]++;
    fprintf(stderr,"\r[count low = %llu] : %zu ...", low, w[low]);
    fflush(stderr);
  }
  return;
}

/*Fonction qui parcourt la liste L entre start et end, et permute les éléments de
 *sorte qu'ils soient ordonnés modulo 2^l.
 *le resultat est stocké dans la po_F2list T.*/
po_F2list* F2list_partial_ordering(F2list *L, size_t l, size_t start, size_t end, int keep_pinv){
  F2vector low;
  size_t i, N, NL = end - start;
  size_t *ind, *w;
  F2list *TL;
  po_F2list *T = po_F2list_init(NL, L->ncols, l, keep_pinv);

  ind = T->index;
  TL = T->L;
  N = (0x1 << l);

  //get workspace
  w = F2malloc(N * sizeof(size_t));

  //count low for each value
  fprintf(stderr,"[Partial ordering]: count low start...\n");
  F2list_count_low(w, L, l, start, end);
  

  //index table. //Peut être directement dans w.
  ind[0] = 0;
  for(i = 0; i < N; i++){
    ind[i+1] = ind[i] + w[i];
    w[i] = 0; // reset w.
  }
  fprintf(stderr,"\n");
  fprintf(stderr,"[Partial ordering]: re-order list...\n");

  //Partialy sort list L in TL.
  /*we look at each entry in L between*/
  for(i = start; i < end; i++){
    low = F2list_entry_low(L, i, l);
    mzd_copy_row(TL, ind[low]+w[low], L, i); //copy entry i of L in good position in TL
    if(keep_pinv){
      T->pinv[ind[low]+w[low]] = i; // row ind[low]+w[low] is row i of L.
    }
    w[low]++; // incrementation of the counter.
  }

  //free workspace.
  free(w);
  return T;
}
