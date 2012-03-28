#ifndef ADDEDFUNCTION_H
#define ADDEDFUNCTION_H

#include "structure/list.h"
#include "structure/arbre.h"
#include "structure/list_connected.h"
#include "BIP/bip.h"

/**
 * \brief Return the tx antenna gain towards the destination direction.
 * \param c should be {antenna id, node id, -1}.
 * \param position the destination position.
 * \return The antenna gain in dB.
 **/
double   calcul_energie(element A,element B,double alpha,double c);

/**
 * \brief Return the tx antenna gain towards the destination direction.
 * \param c should be {antenna id, node id, -1}.
 * \param position the destination position.
 * \return The antenna gain in dB.
 **/
void prim_tree(list *nodesDemarrage, arbre **arbreResult, listC *liste_connexion, list *nodes_de_liste_connexion);

#endif
