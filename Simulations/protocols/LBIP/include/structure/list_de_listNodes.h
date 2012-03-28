#ifndef LIST_DE_LISTNODES_H
#define LIST_DE_LISTNODES_H

#include "structure/listNodes.h"

#define Nullptr(type) (type *)0

struct _list_of_listNodes
{
    element values;
    listeNodes *peres;
    struct _list_of_listNodes *suiv;
};

typedef struct _list_of_listNodes list2N;

void list2N_insert(list2N **list_de_list,int nodeid, listeNodes *peres);
void list2N_insert_values(list2N **list_de_list,int nodeid,double x,double y,double z,listeNodes *peres);
void list2N_insert_element(list2N **list_de_list,element node,listeNodes *peres);

void list2N_affiche(list2N *list_de_list);
int list2N_taille(list2N *list_de_list);
void list2N_detruire(list2N **list_de_list);
int list2N_recherche(list2N *list_de_list,int node);
int list2N_recherche_pere(list2N *list_de_list,int node);
void list2N_copy(list2N **destination,list2N *source);
int  list2N_delete(list2N **list_de_list, int node);
int  list2N_delete_pere_from_fils(list2N *list_de_list,int fils, int pere);


#endif // LIST_DE_LISTNODES_H
