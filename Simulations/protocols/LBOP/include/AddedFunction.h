#ifndef ADDEDFUNCTION_H
#define ADDEDFUNCTION_H

#include <list.h>
#include <arbre.h>
#include <list_connected.h>
#include <heap.h>
#include "Declaration.h"

void prim_tree(list *nodesDemarrage, arbre **arbreResult, listC *liste_connexion, list *nodes_de_liste_connexion);

double   calcul_energie(position_t A, position_t B,double alpha,double c, double* distance);


/**
  * RANGE
  *
  **/

double get_range_Tr(call_t *c);
void set_range_Tr(call_t *c,double range);



#endif
