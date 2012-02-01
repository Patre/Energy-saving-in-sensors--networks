/**
 *  \file   topology.c
 *  \brief  Topology support 
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include "internals.h"


/* ************************************************** */
/* ************************************************** */
static position_t g_area = {-1.0, -1.0, -1.0};
static double range = -1;


/* ************************************************** */
/* ************************************************** */
int topology_init(void) {
    return 0;
}

int topology_bootstrap(void) {
    return 0;
}


void topology_clean(void) {
    return;
}


/* ************************************************** */
/* ************************************************** */
position_t *get_topology_area(void) {
    return &g_area;
}

double get_topology_range(void) {
    if (range  == -1) {
        if ((range = propagation_range) == 0) {
            range = g_area.x / 5;
        }
    }
    return range;
}
