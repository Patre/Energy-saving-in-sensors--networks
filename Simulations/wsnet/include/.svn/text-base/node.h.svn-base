/**
 *  \file   node.h
 *  \brief  Node declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __node_public__
#define __node_public__

#include <include/types.h>


/** 
 * \brief Return the distance between two points.
 * \param position0 first point.
 * \param position1 second point.
 * \return The distance between point0 and point1.
 **/
double distance(position_t *position0, position_t *position1);


/** 
 * \brief Return a node's position. For read-only, the position should not be modified.
 * \param node the node id.
 * \return The node's position.
 **/
position_t *get_node_position(nodeid_t node);


/** 
 * \brief Kill a node during the simulation.
 * \param id the node id.
 **/
void node_kill(nodeid_t id);


/** 
 * \brief Check wether a node is alive.
 * \param id the node id.
 * \return 1 if the node is alive, 0 otherwise.
 **/
int is_node_alive(nodeid_t id);



#endif //__node_public__
