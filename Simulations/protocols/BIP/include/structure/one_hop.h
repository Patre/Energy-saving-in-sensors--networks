#ifndef ONE_HOP_RBH
#define ONE_HOP_RBH

#include <include/modelutils.h>
#include "structure/include.h"

void get_one_hop(call_t *c, double eps);
int init_one_hop(call_t *c, void *args);
int rx_one_hop(call_t *c, packet_t *packet); 
void rx_one_hop_reponse(call_t *c, packet_t *packet); 

#endif //ONE_HOP_RBH
