/**
 *  \file   noise.h
 *  \brief  Noise management declarations
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#ifndef __noise__
#define __noise__

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
extern entity_t *noise_entity;
extern entity_t *interference_entity;


/* ************************************************** */
/* ************************************************** */
int noise_init(void);
int noise_bootstrap(void);
void noise_clean(void);


/* ************************************************** */
/* ************************************************** */
void noise_packet_cs(call_t *c, packet_t *packet);
void noise_packet_rx(call_t *c, packet_t *packet);
double medium_get_noise(call_t *c, int channel);


#endif //__noise__
