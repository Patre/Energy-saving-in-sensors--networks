/**
 *  \file   internals.h
 *  \brief  Internal declarations
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#ifndef __internals__
#define __internals__


/* ************************************************** */
/* ************************************************** */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>

#include <glib.h>
#include <gmodule.h>

#include <include/modelutils.h>
#include <config.h>

/* ************************************************** */
/* ************************************************** */
#include "entity.h"
#include "bundle.h"
#include "measure.h"
#include "node.h"
#include "topology.h"
#include "rng.h"
#include "packet.h"
#include "noise.h"
#include "modulation.h"
#include "mobility.h"
#include "medium.h"
#include "monitor.h"
#include "ioctl_message.h"
#include "probabilistic_distribution.h"

/* ************************************************** */
/* ************************************************** */
int scheduler_init(void);
void scheduler_clean(void);
int scheduler_bootstrap(void);
void do_observe(void);
void scheduler_set_end(uint64_t end);
uint64_t scheduler_get_end(void);
void scheduler_add_mobility(uint64_t clock);
void scheduler_add_rx_begin(uint64_t clock, call_t *c, packet_t *packet);
void scheduler_add_rx_end(uint64_t clock, call_t *c, packet_t *packet);
void scheduler_add_tx_end(uint64_t clock, call_t *c, packet_t *packet);
void do_end(void);
void scheduler_add_birth(uint64_t clock, nodeid_t id);


/* ************************************************** */
/* ************************************************** */
void worldsens_set_syncperiod(uint64_t period);
void worldsens_set_count(int count);
void worldsens_tx_kill(nodeid_t id);
int  worldsens_nodes_rx(call_t *c, packet_t *packet);


#endif //__internals__
