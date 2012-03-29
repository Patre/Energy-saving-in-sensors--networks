#ifndef BIP_DEC_RBH
#define BIP_DEC_RBH

#include <include/modelutils.h>

#include <listNodes.h>
#include <list_de_listNodes.h>
#include <list_paquet.h>
#include <arbre.h>


#define SHOW_GRAPH(x...)  { FILE *topo; topo=fopen("graphBIP","a+"); fprintf(topo,x); fclose(topo);}


struct nodedata {
	int overhead;
	listeNodes* oneHopNeighbourhood;
	listeNodes* twoHopNeighbourhood;
	arbre* BIP_tree;
	double radius;
	//list2N* NodesV1; // l'ensemble des connections du graphe
	//list_PACKET *paquets; // packet recus
	int nbr_evenement; // stats
};


struct protocoleData {
	double    alpha; //alpha de modele d'energie
	double    c; //le C de modele d'energie
	double    eps; //la duree avant le lancement du premier evenement
	//uint64_t  debut; //l'instant de debut de l'application (detection de premier evenement
	//uint64_t  periodEVE; //delta temps entre chaque evenement
	double range;
};


typedef struct _packet_bip
{
    int     type;
    nodeid_t src;
	nodeid_t dst;
	listeNodes* askedToRedirect;
	listeNodes* needsToBeCovered;
} packet_PROTOCOLE;


#endif //PROTOCOLE DECLARATION
