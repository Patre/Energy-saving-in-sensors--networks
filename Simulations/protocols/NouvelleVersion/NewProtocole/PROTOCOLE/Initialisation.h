#ifndef INIT_LBIP_H
#define INIT_LBIP_H

#include "Declaration.h"
#include "AddedFunction.h"

/*LES FONCTION DE CE FICHIER*/
void get_PROTOCOLE_init(call_t *c, double eps);
void init_bip_tree(call_t *c, void *args);


/* ************************************************** */
/* ************************************************** */
//INIT LBIP
void get_PROTOCOLE_init(call_t *c, double eps)
{
    int count=get_node_count()*2;
    uint64_t fin_two_hop=count*time_seconds_to_nanos(eps)+c->node*time_seconds_to_nanos(eps);
    uint64_t at=fin_two_hop;
    scheduler_add_callback(at, c, init_bip_tree, NULL);
}


/* ************************************************** */
/* ************************************************** */
//initailisation de l'arbre de LBIP
void init_bip_tree(call_t *c, void *args)
{

}

#endif // INIT_PROTOCOLE_H
