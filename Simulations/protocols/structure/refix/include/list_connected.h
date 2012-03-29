/* Liste chainee */

#ifndef LIST_CONNECTED_H
#define LIST_CONNECTED_H

#include <stdlib.h>
#include <stdio.h>

#define Nullptr(type) (type *)0

struct _list_con
{
    int node1;
    int node2;
    double poids;
    struct _list_con *suiv;
};

typedef struct _list_con listC;

void list_con_insert(listC **liste_connected,int node1,int node2,double poids);
int list_con_recherche(listC *liste_connected,int node1,int node2);
void list_con_affiche(listC *liste_connected);
int  list_con_delete(listC **liste_connected, int node1,int node2);
void list_set_poids(listC *liste_connected,int node1,int node2,double poids);

#endif // LIST_CONNECTED_H
