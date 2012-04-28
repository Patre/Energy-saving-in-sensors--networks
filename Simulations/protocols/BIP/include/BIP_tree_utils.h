#ifndef INIT_LBIP_H
#define INIT_LBIP_H

#include <float.h>
#include <math.h>

#include <listNodes.h>
#include <arbre.h>
#include <list_de_listNodes.h>
#include <conversion.h>
#include <graphe.h>
#include <time_wsnet.h>
#include <heap.h>
#include <list.h>

#include "structures.h"

struct macnodedata {
	void *buffer;
	double range;
#ifdef ONE_PACKET_AT_A_TIME
	int scheduler;
#endif
};

void init_graphe(call_t *c);

void init_bip_tree(call_t *c, int noeudRacine);

arbre* computeBIPtree(call_t *c, graphe* g, int noeudRacine, int debug);

/**
 * \brief Return the tx antenna gain towards the destination direction.
 * \param c should be {antenna id, node id, -1}.
 * \param position the destination position.
 * \return The antenna gain in dB.
 **/
double calcul_energie(position_t A, position_t B,double alpha,double c, double* distance);

double getCoutFromDistance(double distance, double alpha, double c);

double setRangeToFarestNeighbour(call_t *c, graphe* g, arbre* bipTree);

int getNearestNeighbour(call_t *c, graphe* g, int noeudRacine);

double get_range_Tr(call_t *c);

void set_range_Tr(call_t *c,double range);

#endif // INIT_PROTOCOLE_H


