#ifndef BIP_IMPL_RBH
#define BIP_IMPL_RBH

#include <include.h>
#include <time_wsnet.h>
#include "Declaration.h"

/*LES FONCTION DE CE FICHIER*/
int PROTOCOLE_appelle(call_t *c, packet_t * packetUP);
int PROTOCOLE_reception(call_t *c, packet_t *packetRecu);
double get_range_Tr(call_t *c);
void set_range_Tr(call_t *c,double range);

#endif //PROTOCOLE_RBH
