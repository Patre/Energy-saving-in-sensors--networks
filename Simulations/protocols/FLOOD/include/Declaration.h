#ifndef PROTOCOLE_DEC_RBH
#define PROTOCOLE_DEC_RBH

#include <include/modelutils.h>
#include <listNodes.h>
#include <list_de_listNodes.h>
#include <list_paquet.h>
#include <arbre.h>
#include <conversion.h>
#include <graphe.h>

#define SHOW_GRAPH(x...)  { FILE *topo; topo=fopen("graphFLOOD","a+"); fprintf(topo,x); fclose(topo);}

//HELLO one HOP
#define HELLO       100             //le paquet hello pour calculer one Hop

//FLOOD
#define FLOOD   150                 //le paquet FLOOD

/* ************************************************** */
/* ************************************************** */
/*data des noeud dans l'application BIP*/




struct nodedata {
	int overhead;
	//packet REcus
        list_PACKET *paquets;
        //ajouter par rabah
        listeNodes *oneHopNeighbourhood;

        int nbr_evenement;
};

/* ************************************************** */
/* ************************************************** */
/* VARIABLE de L'APPLICATION LBIP */
struct protocoleData {
        int       debug;
};
/* ************************************************** */
/* ************************************************** */

/*Paquet de BIP*/

struct _packet_flood
{
    int     type;  
    int     src;
    int     seq;                //la sequence de paquet
    int     redirected_by;      //le noeud qui a redirigé le paquet
};
typedef struct _packet_flood packet_PROTOCOLE;

/* ************************************************** */
/* ************************************************** */
//PROPAGATION
struct propagation_data
{
        double range;
};


struct mac_data {
	void *buffer;
	double range;
#ifdef ONE_PACKET_AT_A_TIME
	int scheduler;
#endif
};

#endif //PROTOCOLE DECLARATION
