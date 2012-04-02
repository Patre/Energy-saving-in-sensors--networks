/**
 *  \file   modulation.h
 *  \brief  Modulation management declarations
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#ifndef __modulation__
#define __modulation__

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
extern void *modulation_entities;


/* ************************************************** */
/* ************************************************** */
int modulation_init(void);
int modulation_bootstrap(void);
void modulation_clean(void);


/* ************************************************** */
/* ************************************************** */
double do_modulate(entityid_t modulation, double rxmW, double noise);
void modulation_errors(packet_t *packet);

#endif //__modulation__
