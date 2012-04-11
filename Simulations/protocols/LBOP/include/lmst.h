#ifndef LMST_INIT
#define LMST_INIT

#include <include/modelutils.h>

#include <listNodes.h>
#include <list.h>
#include <time_wsnet.h>
#include "Declaration.h"



/* Paquet LMST */
typedef struct _packet_lmst {
    int         type;
    int         src;
    list        *MST;
} packet_lmst;



void get_lmst(call_t *c, double eps);
int broadcast_lmst(call_t *c);
int rx_lmst(call_t *c, packet_t *packet);

#endif //LMST
