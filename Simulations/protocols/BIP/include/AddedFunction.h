#ifndef ADDEDFUNCTION_H
#define ADDEDFUNCTION_H

#include <list.h>
#include <arbre.h>
#include <list_connected.h>
#include <heap.h>
#include "Declaration.h"

arbre * computeBIPtree(call_t *c, graphe* g);
double   calcul_energie(position_t A, position_t B,double alpha,double c, double* distance);
double getDistanceFromCout(double cout, double alpha, double c);
double getCoutFromDistance(double distance, double alpha, double c);
double setRangeToFarestNeighbour(call_t *c, graphe* g, arbre* bipTree);
int getNearestNeighbour(call_t *c);
void setRelayNodes(call_t *c, graphe* g, arbre* bipTree, listeNodes** askedToRedirect, listeNodes** needsToBeCovered);
graphe* purgeGraphe(call_t* c, int farestNode, int fromNode);

/**
  * RANGE
  *
  **/
double get_range_Tr(call_t *c);
void set_range_Tr(call_t *c,double range);


#endif
