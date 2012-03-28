#ifndef ONE_HOP_RBH
#define ONE_HOP_RBH

#include <include/modelutils.h>

#include "structure/listNodes.h"
#include "structure/time_wsnet.h"

#include "BIP/bip.h"
#include "BIP/two_hop.h"


typedef struct packet_hello {
	int       type;
    nodeid_t src;
    position_t src_pos;
} packet_hello;

void init_one_hop(call_t *c, double eps);
int broadcast_hello(call_t *c, void *args);
int rx_hello(call_t *c, packet_t *packet);

#endif //ONE_HOP_RBH
