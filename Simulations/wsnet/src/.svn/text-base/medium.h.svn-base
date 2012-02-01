/**
 *  \file   medium.h
 *  \brief  Medium declarations
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#ifndef __medium__
#define __medium__

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
extern entity_t *propagation_entity;
extern entity_t *shadowing_entity;
extern entity_t *fading_entity;
extern double propagation_range;


/* ************************************************** */
/* ************************************************** */
int medium_init(void);
int medium_bootstrap(void);
void medium_clean(void);


/* ************************************************** */
/* ************************************************** */
void medium_tx_end(packet_t *packet, call_t *c);
void medium_cs(packet_t *packet, call_t *c);
void medium_rx(packet_t *packet, call_t *c);
void medium_compute_rxdBm(packet_t *packet, call_t *c);


#endif //__medium__
