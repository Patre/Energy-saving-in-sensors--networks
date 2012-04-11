#ifndef TWO_HOP_RBH
#define TWO_HOP_RBH

#include <include/modelutils.h>

#include <listNodes.h>
#include <time_wsnet.h>
#include <arbre.h>

#include "BIP_tree_utils.h"
#include "structures.h"



typedef struct packet_hello2 {
	int       type;
    nodeid_t src;
    position_t src_pos;
	listeNodes* oneHopNeighbourhood;
} packet_hello2;

void init_two_hop(call_t *c, double eps);
int broadcast_hello2(call_t *c, void *args);
int rx_two_hop(call_t *c, packet_t *packet);
void print_two_hop_neighbourhood(call_t *c);

#endif //TWO_HOP_RBH

