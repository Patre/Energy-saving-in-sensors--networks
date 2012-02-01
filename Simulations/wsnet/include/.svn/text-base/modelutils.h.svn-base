/**
 *  \file   modelutils.h
 *  \brief  Utility function declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida & Quentin Lampin
 *  \date   2007
 **/
#ifndef __modelutils__
#define __modelutils__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <limits.h>

#include <include/options.h>
#include <include/log.h>
#include <include/mem_fs.h>
#include <include/das.h>
#include <include/sodas.h>
#include <include/spadas.h>
#include <include/hadas.h>
#include <include/timer.h>
#include <include/types.h>
#include <include/models.h>
#include <include/rng.h>
#include <include/probabilistic_distribution.h>
#include <include/medium.h>
#include <include/radio.h>
#include <include/antenna.h>
#include <include/battery.h>
#include <include/entity.h>
#include <include/packet.h>
#include <include/node.h>
#include <include/param.h>
#include <include/measure.h>
#include <include/scheduler.h>
#include <include/monitor.h>
#include <include/ioctl_message.h>
#include <include/modulation.h>

/** \def BROADCAST_ADDR 
 * \brief Broadcast address.
 **/ 
#define BROADCAST_ADDR  -1 


/**
 * \brief Stop the simulation
 **/
void end_simulation(void);


/**
 * \brief Return the current simulation time.
 * \return The simulation time.
 **/
uint64_t get_time(void);


/**
 * \brief Return the number of simulated nodes.
 * \return The number of ndoes.
 **/
int get_node_count(void);


/**
 * \brief Return the size of the network area.
 * \return The network area.
 **/
position_t *get_topology_area(void);


void TX(call_t *c, packet_t *packet);
void RX(call_t *c, packet_t *packet);
int IOCTL(call_t *c, int option, void *in, void **out);
int SET_HEADER(call_t *c, packet_t *packet, destination_t *dst);
int GET_HEADER_SIZE(call_t *c);

/* edit by Quentin Lampin <quentin.lampin@orange-ftgroup.com> */
int GET_HEADER_REAL_SIZE(call_t *c);
/* end of edition */

#endif //__modelutils__
