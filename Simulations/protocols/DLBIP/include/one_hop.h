#ifndef ONE_HOP_RBH
#define ONE_HOP_RBH

#include <include/modelutils.h>

#include <listNodes.h>
#include <time_wsnet.h>

#include "BIP_tree_utils.h"
#include "two_hop.h"
#include "structures.h"


typedef struct packet_hello {
	int       type;
    nodeid_t src;
    position_t src_pos;
} packet_hello;

void init_one_hop(call_t *c, double eps);
int broadcast_hello(call_t *c, void *args);
int rx_hello(call_t *c, packet_t *packet);
void print_one_hop_neighbourhood(call_t *c);

#endif //ONE_HOP_RBH
