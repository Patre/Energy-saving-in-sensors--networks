/**
 *  \file   packet.h
 *  \brief  Packet management declarations
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#ifndef __packet__
#define __packet__

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
int packet_init(void);
int packet_bootstrap(void);
void packet_clean(void);


/* ************************************************** */
/* ************************************************** */
packet_t *packet_rxclone(packet_t *packet);


#endif //__packet__
