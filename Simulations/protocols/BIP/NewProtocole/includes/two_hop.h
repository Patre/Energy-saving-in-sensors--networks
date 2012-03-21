#ifndef TWO_HOP_RBH
#define TWO_HOP_RBH

#include <include/modelutils.h>

void get_two_hop(call_t *c, double eps);
int init_two_hop(call_t *c, void *args);
int rx_two_hop(call_t *c, packet_t *packet);
void rx_two_hop_reponse(call_t *c, packet_t *packet);

#endif //TWO_HOP_RBH

