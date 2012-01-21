/**
 *  \file   mobility.c
 *  \brief  Mobility module : event-driven
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdlib.h>
#include <stdio.h>

#include "internals.h"


/* ************************************************** */
/* ************************************************** */
#define MOBILITY_UPDATE 200000000


/* ************************************************** */
/* ************************************************** */
static uint64_t l_update = 0;


/* ************************************************** */
/* ************************************************** */
int mobility_init(void) {
    return 0;
}

void mobility_clean(void) {
    return;
}


/* ************************************************** */
/* ************************************************** */
int mobility_bootstrap(void) {
#ifdef MOB_PERIOD
    scheduler_add_mobility(l_update + MOBILITY_UPDATE);
#endif /* MOB_PERIOD */
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void mobility_update(uint64_t clock) {
#ifndef MOB_PERIOD
    if (clock - l_update < MOBILITY_UPDATE) {
        return;
    }
    
    nodes_update_mobility();
    l_update = clock;
#endif /* MOB_PERIOD */
}

void mobility_event(uint64_t clock) {
    if (l_update == clock) {
        return;
    }

    nodes_update_mobility();
    l_update = clock;
#ifdef MOB_PERIOD
    scheduler_add_mobility(l_update + MOBILITY_UPDATE);
#endif /* MOB_PERIOD */
}
