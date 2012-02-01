/**
 *  \file   types.h
 *  \brief  Type declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida & Quentin Lampin
 *  \date   2007
 **/
#ifndef __types__
#define __types__

#include <stdlib.h>
#include <inttypes.h>
#include <math.h>


/** \typedef array_t
 * \brief An array of integers containing its size.
 **/
/** \struct _array 
 * \brief An array of integers containing its size. Should use type array_t.
 **/
typedef struct _array {
    int size;  /**< array size **/
    int *elts; /**< array elements **/
} array_t;


/** \typedef nodeid_t 
 * \brief A node identifier. 
 **/
/** \struct _nodeid
 * \brief A node identifier. Should use type nodeid_t.
 **/
typedef int nodeid_t;


/** \typedef entityid_t 
 * \brief An entity identifier. 
 **/
/** \struct _entityid 
 * \brief An entity identifier. Should use type entityid_t.
 **/
typedef int entityid_t;


/** \typedef measureid_t
 * \brief A measure identifier. 
 **/
/** \struct _measureid
 * \brief A measure identifier. Should use type measureid_t.
 **/
typedef int measureid_t;


/** \typedef packetid_t 
 * \brief A packet identifier. 
 **/
/** \struct _packetid
 * \brief A packet identifier. Should use type packetid_t.
 **/
typedef int packetid_t;


/** \typedef position_t
 * \brief A position in the 3D space. 
 **/
/** \struct _position
 * \brief A position in the 3D space. Should use type position_t.
 **/
typedef struct _position {
    double x; /**< x position **/
    double y; /**< y position **/ 
    double z; /**< z position **/ 
} position_t;


/** \typedef angle_t
 * \brief An angle in the 3D space.
 **/
/** \struct _angle
 * \brief An angle in the 3D space. Should use type angle_t.
 **/
typedef struct _angle {
    double xy; /**< angle on the xy plane **/ 
    double z;  /**< angle between the xy plane and the z axis**/ 
} angle_t;


/** \typedef destination_t
 * \brief A packet destination. May be a node address or a geographical position.
 **/
/** \struct _destination
 * \brief A packet destination. May be a node address or a geographical position. Should use type destination_t.
 **/
typedef struct _destination {
    nodeid_t id;         /**< the destination node id **/
    position_t position; /**< the destination position **/
} destination_t;


/** \typedef call_t
 * \brief A parameter that identifies who we are calling and who has called us. Kind of a self pointer.
 **/
/** \struct _call
 * \brief A parameter that identifies who we are calling and who has called us. Kind of a self pointer. Should use type call_t.
 **/
typedef struct _call {
    entityid_t entity; /**< the called entity id **/
    nodeid_t node;     /**< the called node id **/
    entityid_t from;   /**< the entity that made the call **/
} call_t;


/** \typedef packet_t
 * \brief A radio packet.
 **/
/** \struct _packet
 * \brief A radio packet. Should use type packet_t.
 **/
typedef struct _packet {
    packetid_t id;         /**< the packet id **/
    int size;              /**< size of the packet data **/
    int real_size;         /**< real size of the packet data **/
    int type;              /**< type of the packet (for multistandard nodes) **/

    uint64_t clock0;       /**< packet rx start **/
    uint64_t clock1;       /**< packet rx end **/
    uint64_t duration;     /**< packet tx/rx duration **/

    nodeid_t node;         /**< node that has created the packet **/
    entityid_t antenna;    /**< antenna that has emitted the packet **/

    /* edit by Loic Lemaitre */
    int    worldsens_mod;  /**< worldsens modulation id (wsim radio modulation id) **/
    double worldsens_freq; /**< worldsens radio frequency **/
    /* end of edition */

    double txdBm;          /**< tx power in dBm **/
    int channel;          
    entityid_t modulation; /**< modulation entity **/
    uint64_t Tb;           /**< radio bandwidth: time to send a bit **/

    double PER;            /**< packet error rate **/
    double rxdBm;          /**< rx power in dBm **/
    double rxmW;           /**< rx power in mW **/
    double *noise_mW;      /**< packet noise in mW**/
    double *ber;           /**< packet ber **/

    char *data;            /**< packet data **/
} packet_t;


/* edit by Quentin Lampin <quentin.lampin@orange-ftgroup.com> */
/** \typedef ioctl_message_t
 * \brief An ioctl message.
 **/
/** \struct _io_ctl_message
 * \brief An ioctl message. Should use type ioctl_message_t.
 **/
typedef struct _ioctl_message{
    /* message body type */
    int type;
    /* message body size */
    int size;
    /* message body real size */
    int real_size;
    /* message body */
    void *body;
} ioctl_message_t;
/* end of edition */


/** \typedef param_t
 * \brief A parameter for the "init" and "setnode" entity functions.
 **/
/** \struct _param
 * \brief A parameter for the "init" and "setnode" entity functions. Should use type param_t.
 **/
typedef struct _param {
    char *key;   /**< the parameter key **/
    char *value; /**< the parameter value **/
} param_t;


/** \typedef callback_t 
 * \brief Prototype of a callback function. 
 **/
typedef int (* callback_t) (call_t *c, void *arg);


#endif //__types__
