/**
 *  \file   packet.h
 *  \brief  Packet declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida & Quentin Lampin
 *  \date   2007
 **/
#ifndef __packet_public__
#define __packet_public__

#include <include/types.h>


/** 
 * \brief Deallocate a packet.
 * \param packet the packet to dealloc.
 **/
void packet_dealloc(packet_t *packet);


/** 
 * \brief Duplicate a packet. Data and other information are copied.
 * \param packet the packet to clone.
 * \return The cloned packet.
 **/
packet_t *packet_clone(packet_t *packet);


/* edit by Quentin Lampin <quentin.lampin@orange-ftgroup.com> */

/**
 * \brief Allocate a packet with real size argument.
 * \param c should be {-1, node id, -1}.
 * \param size the data size of the packet (in bytes).
 * \param real_size real size in bits of the packet.
 * \return The newly allocated packet.
 **/

packet_t *packet_create(call_t *c, int size, int real_size);

/* end of edition */


/** 
 * \brief Allocate a packet - DEPRECATED, use packet_create instead.
 * \param c should be {-1, node id, -1}.
 * \param size the data size of the packet (in bytes).
 * \return The newly allocated packet.
 **/
packet_t *packet_alloc(call_t *c, int size);






#endif //__packet_public__
