#ifndef INIT_LBIP_H
#define INIT_LBIP_H

#include <listNodes.h>
#include <arbre.h>
#include <list_de_listNodes.h>
#include <conversion.h>
#include <graphe.h>
#include <time_wsnet.h>
#include <math.h>

#include "structures.h"


void computeBIPtree2Hop(call_t *c);

void setRelayNodes(listeNodes** askedToRedirect, listeNodes** needsToBeCovered);

/**
 * \brief Return the tx antenna gain towards the destination direction.
 * \param c should be {antenna id, node id, -1}.
 * \param position the destination position.
 * \return The antenna gain in dB.
 **/
double   calcul_energie(position_t A, position_t B,double alpha,double c, double* distance);

/*void prim_tree(int node,arbre **a,listC *l,list *g);
void prim_tree_lbip(int node,arbre **a, list *N1,list2 *N2,double alpha,double c);
void init_lbip_tree(call_t *c, void *args);*/

#endif // INIT_PROTOCOLE_H


