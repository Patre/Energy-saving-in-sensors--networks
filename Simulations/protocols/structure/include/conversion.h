#ifndef DEF_CONVERSION
#define DEF_CONVERSION
#include "list.h"
#include "listNodes.h"
#include "list_connected.h"
#include "list_de_list.h"
#include "list_de_listNodes.h"
#include "arbre.h"

void list_to_listC(listC **liste_connected,list *liste,int node);
void listC_to_list(list **liste,listC *liste_connected);

void arbre_to_list(list **liste,arbre *arbreV);
void arbre_to_list_sauf(list **liste,arbre *arbreV,int val);
void arbre_to_list_fils(list **liste,arbre *arbreV);


void list2_to_list(list **liste,list2 *list_de_list);
void list2_to_listC(listC **liste_connected,list2 *list_de_list);


void list2N_to_list(list **liste,list2N *list_de_list);
void list2N_to_listC(listC **liste_connected,list2N *list_de_list);


void listeNodes_to_listC(listC **liste_connected,listeNodes *listeNodese,int node);
void listC_to_listeNodes(listeNodes **listeNodese,listC *liste_connected);
void listeNodes_to_list(list **liste, listeNodes *list_nodes);

#endif


