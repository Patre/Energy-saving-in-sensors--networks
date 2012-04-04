#ifndef  _LISTNODES_RBH
#define  _LISTNODES_RBH

#include "include.h"

#include <stdio.h>
#include <stdlib.h>

#define Nullptr(type) (type *)0

struct _listeNodes
{
    element values;
    struct _listeNodes *suiv;
};

typedef struct _listeNodes listeNodes;

void listeNodes_insert(listeNodes **listeNodese,int val);
void listeNodes_insert_values(listeNodes **liste,int node,double x,double y ,double z);
void listeNodes_insert_element(listeNodes **liste,element node);


void listeNodes_affiche(listeNodes *listeNodese);
int listeNodes_taille(listeNodes *listeNodes);
int listeNodes_get(listeNodes *listeNodese,int index);
void listeNodes_detruire(listeNodes **listeNodese);
int listeNodes_recherche(listeNodes *listeNodese,int val);
int listeNodes_get_index(listeNodes *l, int val);
void listeNodes_copy(listeNodes **destination,listeNodes *source);
int  listeNodes_delete(listeNodes **listeNodes, int val);
void listeNodes_intersection(listeNodes **listeNodese1,listeNodes *listeNodes_a_enlever_de_la_premiere);
void listeNodes_union(listeNodes **listeNodese1,listeNodes *listeNodese2);//*/



#endif //_listeNodesNODES_RBH
