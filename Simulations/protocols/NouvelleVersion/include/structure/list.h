#ifndef  _LIST_RBH
#define  _LIST_RBH
#include "structure/list_connected.h"

#define Nullptr(type) (type *)0

struct _list
{
    int val;
    struct _list *suiv;
};

typedef struct _list list;

void list_insert(list **liste,int val);
void list_affiche(list *liste);
int list_taille(list *liste);
int list_get(list *liste,int index);
void list_detruire(list **liste);
int list_recherche(list *liste,int val);
void list_copy(list **destination,list *source);
int  list_delete(list **liste, int val);
void list_intersection(list **liste1,list *liste_a_enlever_de_la_premiere);
void list_to_listC(listC **liste_connected,list *liste,int node);
void listC_to_list(list **liste,listC *liste_connected);
void list_union(list **liste1,list *liste2);

#endif //_LIST_RBH
