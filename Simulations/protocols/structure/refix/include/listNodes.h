#ifndef  _LISTNODES_RBH
#define  _LISTNODES_RBH

#include "list_connected.h"
#include "list.h"
#include "include.h"

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
void listeNodes_copy(listeNodes **destination,listeNodes *source);
int  listeNodes_delete(listeNodes **listeNodes, int val);
void listeNodes_intersection(listeNodes **listeNodese1,listeNodes *listeNodes_a_enlever_de_la_premiere);
void listeNodes_to_listC(listC **liste_connected,listeNodes *listeNodese,int node);
void listC_to_listeNodes(listeNodes **listeNodese,listC *liste_connected);
void listeNodes_union(listeNodes **listeNodese1,listeNodes *listeNodese2);//*/


void listeNodes_to_list(list **liste, listeNodes *list_nodes);

#endif //_listeNodesNODES_RBH
