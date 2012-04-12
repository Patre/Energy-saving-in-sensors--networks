#ifndef INIT_RBOP_H
#define INIT_RBOP_H

#include <time_wsnet.h>
#include <conversion.h>
#include <listNodes.h>
#include "Declaration.h"

/*LES FONCTION DE CE FICHIER*/
void get_RBOP_init(call_t *c, double eps);
void init_rbop(call_t *c, void *args);
void get_RNG_tree(int node,list **a,list *N1);
void get_PROTOCOLE_init(call_t *c, double eps);
//RANGE
double get_range_Tr(call_t *c);
void set_range_Tr(call_t *c,double range);






#endif // INIT_PROTOCOLE_H

