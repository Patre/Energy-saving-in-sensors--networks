#ifndef BIP_DEC_RBH
#define BIP_DEC_RBH

#include <include/modelutils.h>
#include <listNodes.h>
#include <list_de_listNodes.h>
#include <list_paquet.h>
#include <arbre.h>
#include <graphe.h>

#define DBG(x...)  printf(x);
#define SHOW_GRAPH(x...)  { FILE *topo; topo=fopen("graphRBOP","a+"); fprintf(topo,x); fclose(topo);}
/* ************************************************** */
/* ************************************************** */
/*data des noeud dans l'application BIP*/




struct nodedata {
	int overhead;
	
	double range;
	//packet REcus
	list_PACKET *paquets;
	//Integration a la version chloé
	listeNodes* oneHopNeighbourhood;
	//PROTOCOLE
	list *RNG;
	//ajouter par rabah
	int nbr_evenement;
	
	
};

/* ************************************************** */
/* ************************************************** */
/* VARIABLE de L'APPLICATION LBIP */
struct protocoleData {
        int debug;
};

/* ************************************************** */
/* ************************************************** */

/*Paquet de BIP*/

struct _packet_bip
{
    int     type;  
    int     src;
    position_t src_pos;
    int     seq;                //la sequence de paquet
    int     redirected_by;      //le noeud qui a redirigé le paquet

    /**************************************************/
    list    *destinations;            //les noeuds de destination de packet
	
	
};
typedef struct _packet_bip packet_PROTOCOLE;

/* ************************************************** */
/* ************************************************** */
//PROPAGATION

struct propagation_data
{
        double range;
};

//MAC

struct mac_data {
    int debug;
    double range;        // Communication range (m)
    double bandwidth;    // Data bandwidth (KB/s)
};

struct macnodedata {
    void *buffer;
    double range;
#ifdef ONE_PACKET_AT_A_TIME
    int scheduler;
#endif
};

void tx( call_t *c , packet_t * packet );

#endif //PROTOCOLE DECLARATION
