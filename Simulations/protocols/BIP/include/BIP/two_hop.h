#ifndef TWO_HOP_RBH
#define TWO_HOP_RBH

#include <include/modelutils.h>

#include "structure/listNodes.h"
#include "structure/time_wsnet.h"

#include "BIP/bip.h"


typedef struct packet_hello2 {
	int       type;
    nodeid_t src;
    position_t src_pos;
	listeNodes* oneHopNeighbourhood;
} packet_hello2;

void init_two_hop(call_t *c, double eps);
int broadcast_hello2(call_t *c, void *args);
int rx_two_hop(call_t *c, packet_t *packet);

#endif //TWO_HOP_RBH

