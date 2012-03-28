#ifndef BIP_DEC_RBH
#define BIP_DEC_RBH

#include <include/modelutils.h>

#include "structure/listNodes.h"
#include "structure/list_de_listNodes.h"
#include "structure/list_paquet.h"
#include "structure/arbre.h"

#define SHOW_GRAPH(x...)  { FILE *topo; topo=fopen("graphBIP","a+"); fprintf(topo,x); fclose(topo);}


struct nodedata {
	int overhead;
	listeNodes* oneHopNeighbourhood;
	listeNodes* twoHopNeighbourhood;
	arbre* BIP_tree; // l'arbre de bip original
	//list2N* NodesV1; // l'ensemble des connections du graphe
	//list_PACKET *paquets; // packet recus
	
	int nbr_evenement; // stats
};


struct protocoleData {
	double    alpha; //alpha de modele d'energie
	double    c; //le C de modele d'energie
	double    eps; //la duree de repos (ordonnanceur de l'initialisation)
	uint64_t  debut; //l'instant de debut de l'application (detection de premier evenement
	uint64_t  periodEVE; //delta temps entre chaque evenement
};


typedef struct _packet_bip
{
    int     type;               //pour le type
    nodeid_t src;
    position_t src_pos;
	nodeid_t dst;
    position_t dst_pos;
    int     seq;                //la sequence de paquet
    int     redirected_by;      //le noeud qui a redirigé le paquet
	
    /**************************************************/
    list    *destinations;            //les noeuds de destination de packet
    arbre   *pere_arbre;              //ARBRE du pere (redirected_by)
	
	
} packet_PROTOCOLE;


#endif //PROTOCOLE DECLARATION
