#ifndef LIST_DE_LIST_H
#define LIST_DE_LIST_H

#include <include/modelutils.h>
#include "structure/list.h"
#include "structure/list_connected.h"


#define Nullptr(type) (type *)0

struct _list_of_list
{
    int node;
    list *peres;
    struct _list_of_list *suiv;
};

typedef struct _list_of_list list2;

void list2_insert(list2 **list_de_list,int nodeid, list *peres);
void list2_affiche(list2 *list_de_list);
int list2_taille(list2 *list_de_list);
void list2_detruire(list2 **list_de_list);
int list2_recherche(list2 *list_de_list,int node);
int list2_recherche_pere(list2 *list_de_list,int node);
void list2_copy(list2 **destination,list2 *source);
int  list2_delete(list2 **list_de_list, int node);
int  list2_delete_pere_from_fils(list2 *list_de_list,int fils, int pere);
void list2_to_list(list **liste,list2 *list_de_list);
void list2_to_listC(listC **liste_connected,list2 *list_de_list);

#endif // LIST_DE_LIST_H
