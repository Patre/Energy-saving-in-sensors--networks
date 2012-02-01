/**
 *  \file   mobility.h
 *  \brief  Mobility declarations
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#ifndef __mobility__
#define __mobility__

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
int mobility_init(void);
int mobility_bootstrap(void);
void mobility_clean(void);


/* ************************************************** */
/* ************************************************** */
void mobility_update(uint64_t clock);
void mobility_event(uint64_t clock);


#endif //__mobility__
