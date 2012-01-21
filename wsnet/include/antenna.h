/**
 *  \file   antenna.h
 *  \brief  Antenna declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __antenna_public__
#define __antenna_public__

#include <include/types.h>


/**
 * \brief Return the signal loss induced by an antenna circuit.
 * \param c should be {antenna id, node id, -1}.
 * \return Signal loss in dB.
 **/
double antenna_get_loss(call_t *c);


/**
 * \brief Return the antenna orientation.
 * \param c should be {antenna id, node id, -1}.
 * \return Current antenna orientation.
 **/
angle_t *antenna_get_angle(call_t *c);


/**
 * \brief Set the antenna orientation.
 * \param c should be {antenna id, node id, -1}.
 * \param angle new antenna orientation.
 **/
void antenna_set_angle(call_t *c, angle_t *angle);


/**
 * \brief Forward a received packet to the antenna.
 * \param c should be {antenna id, node id, -1}.
 * \param packet the received packet.
 **/
void antenna_rx(call_t *c, packet_t *packet);


/**
 * \brief Return the tx antenna gain towards the destination direction.
 * \param c should be {antenna id, node id, -1}.
 * \param position the destination position.
 * \return The antenna gain in dB.
 **/
double antenna_gain_tx(call_t *c, position_t *position);


/**
 * \brief Return the rx antenna gain towards the source direction.
 * \param c should be {antenna id, node id, -1}.
 * \param position the source position.
 * \return The antenna gain in dB.
 **/
double antenna_gain_rx(call_t *c, position_t *position);


#endif //__antenna_public__
