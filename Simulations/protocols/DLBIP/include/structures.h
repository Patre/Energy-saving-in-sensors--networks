#ifndef BIP_DEC_RBH
#define BIP_DEC_RBH

#include <include/modelutils.h>

#include <listNodes.h>
#include <list_de_listNodes.h>
#include <list_paquet.h>
#include <arbre.h>
#include <graphe.h>

#define DBG(x...)  printf(x);
#define SHOW_GRAPH(x...)  { FILE *topo; topo=fopen("graphDLBIP","a+"); fprintf(topo,x); fclose(topo);}


void tx( call_t *c , packet_t * packet );

struct nodedata {
	int overhead;
	listeNodes* oneHopNeighbourhood;
	listeNodes* twoHopNeighbourhood;
	graphe* g2hop;
	int nbr_evenement; // stats
	int* lastIDs;
	double* energiesRem;
	arbre* BIP_tree;
};


struct protocoleData {
        int debug;
	double    alpha; //alpha de modele d'energie
	double    c; //le C de modele d'energie
	double    eps; //la duree avant le lancement du premier evenement
};


typedef struct _packet_bip
{
    int     type;
    nodeid_t src;
	nodeid_t dst;
	nodeid_t pred;
	double energyRem;
	listeNodes* askedToRedirect;
	listeNodes* needsToBeCovered;
	int id;
} packet_PROTOCOLE;


#endif //PROTOCOLE DECLARATION
