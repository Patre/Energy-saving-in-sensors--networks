/**
 *  \file   battery.h
 *  \brief  Battery declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __battery_public__
#define __battery_public__

#include <include/types.h>


/**
 * \brief Consume energy associated to a transmission.
 * \param c should be {-1, node id, -1}.
 * \param duration the transmission duration.
 * \param txdBm the transmission power in dBm.
 **/
void battery_consume_tx(call_t *c, uint64_t duration, double txdBm);


/**
 * \brief Consume energy associated to a reception.
 * \param c should be {-1, node id, -1}.
 * \param duration the reception duration.
 **/
void battery_consume_rx(call_t *c, uint64_t duration);


/**
 * \brief Consume energy associated to idle time.
 * \param c should be {-1, node id, -1}.
 * \param duration the idle duration.
 **/
void battery_consume_idle(call_t *c, uint64_t duration);


/**
 * \brief Consume energy.
 * \param c should be {-1, node id, -1}.
 * \param the amount of consumed energy.
 **/
void battery_consume(call_t *c, double energy);


/**
 * \brief Return the consummed energy.
 * \param c should be {-1, node id, -1}.
 * \return Volume of consumed energy.
 **/
double battery_consumed(call_t *c);


/**
 * \brief Return the remaining energy.
 * \param c should be {-1, node id, -1}.
 * \return Volume of remaining energy.
 **/
double battery_remaining(call_t *c);


/**
 * \brief Returns the percentage of energy charge.
 * \param c should be {-1, node id, -1}.
 * \return Percentage of energy charge.
 **/
double battery_status(call_t *c);


#endif //__battery_public__
