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


arbre* computeBIPtree(call_t *c, graphe* g);

/**
 * \brief Return the tx antenna gain towards the destination direction.
 * \param c should be {antenna id, node id, -1}.
 * \param position the destination position.
 * \return The antenna gain in dB.
 **/
double   calcul_energie(position_t A, position_t B,double alpha,double c, double* distance);

double getDistanceFromCout(double cout, double alpha, double c);

double getCoutFromDistance(double distance, double alpha, double c);

double setRangeToFarestNeighbour(call_t *c, graphe* g, arbre* bipTree);

int getNearestNeighbour(call_t *c);

void setRelayNodes(call_t *c, graphe* g, arbre* bipTree, listeNodes** askedToRedirect, listeNodes** needsToBeCovered);

graphe* purgeGraphe(call_t* c, int farestNode, int fromNode);

#endif // INIT_PROTOCOLE_H


