#ifndef LIST_PAQUET_LBIP_H
#define LIST_PAQUET_LBIP_H

#include <stdio.h>
#include <stdlib.h>

#define Nullptr(type) (type *)0

struct _packet_
{
    int     source;             //la source de paquet
    int     seq;                //la sequance de paquet
    int     redirected_by;      //le noeud qui a rediriger  le paquet
};
typedef struct _packet_ packet_LIST_element;


//LA structure d'une list de packet
struct _list_paquet
{
    packet_LIST_element packet;
    struct _list_paquet *suiv;
};
typedef struct _list_paquet list_PACKET;


void list_PACKET_insert(list_PACKET **list_packets,packet_LIST_element packet);
void list_PACKET_insert_tout(list_PACKET **list_packets,int source,int seq, int redirected_by);
void list_PACKET_affiche(list_PACKET *list_packets);
int list_PACKET_taille(list_PACKET *list_packets);
void list_PACKET_detruire(list_PACKET **list_packets);
int list_PACKET_recherche(list_PACKET *list_packets,packet_LIST_element val);
int list_PACKET_recherche_tout(list_PACKET *list_packets,int source,int seq);

#endif // LIST_PAQUET_LBIP_H
