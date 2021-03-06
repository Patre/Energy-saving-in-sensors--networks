#ifndef ARBRE_RBH
#define ARBRE_RBH

#include "list.h"

#include <stdio.h>
#include <stdlib.h>

struct _arbre
{
    int node;
    int size;
    struct _arbre **fils;
};
typedef struct _arbre arbre;

/*******************/
/*FONCTION DANS Ce FICHIER*/
void arbre_add_pere(arbre **arbreV, int node);
void arbre_add_fils(arbre *arbreV,int pere,int fils);
void arbre_affiche(arbre *arbreV);
void arbre_detruire(arbre **arbreV);
void arbre_delete_fils(arbre *arbreV,int node);
void arbre_moins_list(arbre *arbreV,list *liste);

void arbre_copy(arbre **dest,arbre *source);
void arbre_copy_fils(arbre *arbreV, arbre *arbreFils,int pere);

int arbre_recherche(arbre *arbreV,int node);
void arbre_get_fils(list **liste, arbre *arbreV,int pere);
int arbre_is_leaf(arbre *a, int node);
#endif // ARBRE_RBH
