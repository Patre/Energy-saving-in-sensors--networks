#ifndef BIP_IMPL_RBH
#define BIP_IMPL_RBH

#include "structure/include.h"
#include "BIP/Initialisation.h"


/*LES FONCTION DE CE FICHIER*/
int PROTOCOLE_appelle(call_t *c, packet_t * packetUP);
int PROTOCOLE_reception(call_t *c, packet_t *packetRecu);

#endif //PROTOCOLE_RBH
