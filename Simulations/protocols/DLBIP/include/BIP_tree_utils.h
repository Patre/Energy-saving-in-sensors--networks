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



arbre* computeBIPtree(call_t *c, graphe* g, listeNodes* askedToRedirect, listeNodes* needsToBeCovered, int debug);

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

double getRange(call_t *c);

int getNearestNeighbour(call_t *c, graphe* g);

void setRelayNodes(call_t *c, graphe* g, arbre* bipTree, listeNodes** askedToRedirect, listeNodes** needsToBeCovered, int node);

graphe* purgeGraphe(call_t* c, int farestNode, int fromNode, int predNode);

void forward(call_t* c, packet_t *packet);

void set_range_Tr(call_t *c, double range);

#endif // INIT_PROTOCOLE_H


