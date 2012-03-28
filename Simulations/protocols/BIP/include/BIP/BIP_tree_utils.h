#ifndef INIT_LBIP_H
#define INIT_LBIP_H

#include "structure/listNodes.h"
#include "structure/arbre.h"
#include "structure/list_de_listNodes.h"
#include "structure/conversion.h"
#include "structure/graphe.h"


#ifndef DEF_NODEDATA
#define DEF_NODEDATA
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
#endif


void computeBIPtree2Hop(call_t *c);
void computeGrapheFromNeighbours(call_t *c, graphe* g);

void setRelayNodes(listeNodes** askedToRedirect, listeNodes** needsToBeCovered);

//void init_bip_tree(call_t *c, void *args);

/**
 * \brief Return the tx antenna gain towards the destination direction.
 * \param c should be {antenna id, node id, -1}.
 * \param position the destination position.
 * \return The antenna gain in dB.
 **/
//double   calcul_energie(element A,element B,double alpha,double c);

/**
 * \brief Return the tx antenna gain towards the destination direction.
 * \param c should be {antenna id, node id, -1}.
 * \param position the destination position.
 * \return The antenna gain in dB.
 **/
//void prim_tree(list *nodesDemarrage, arbre **arbreResult, listC *liste_connexion, list *nodes_de_liste_connexion);

#endif // INIT_PROTOCOLE_H


