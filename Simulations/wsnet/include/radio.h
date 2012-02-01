/**
 *  \file   radio.h
 *  \brief  Radio declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __radio_public__
#define __radio_public__

#include <include/types.h>


/**
 * \brief Carrier Sense mechanism.
 * \param c should be {radio id, node id, -1}.
 * \return The signal strength of the currently received signal. MIN_DBM if no current signal.
 **/
double radio_get_cs(call_t *c);


/**
 * \brief Clear Channel Assesment mechanism.
 * \param c should be {radio id, node id, -1}.
 * \return The noise strength on the radio interface. MIN_DBM if no noise.
 **/
double radio_get_noise(call_t *c);


/**
 * \brief Get the radio tx power.
 * \param c should be {radio id, node id, -1}.
 * \return The current transmission power in dBm.
 **/
double radio_get_power(call_t *c);


/**
 * \brief Set the radio tx power.
 * \param c should be {radio id, node id, -1}.
 * \param power the new transmission power in dBm.
 **/
void radio_set_power(call_t *c, double power);


/**
 * \brief Get the radio channel.
 * \param c should be {radio id, node id, -1}.
 * \return The current radio channel.
 **/
int radio_get_channel(call_t *c);


/**
 * \brief Set the radio channel.
 * \param c should be {radio id, node id, -1}.
 * \param channel the new radio channel.
 **/
void radio_set_channel(call_t *c, int channel);


/**
 * \brief Get the radio modulation.
 * \param c should be {radio id, node id, -1}.
 * \return The current modulation entity id.
 **/
entityid_t radio_get_modulation(call_t *c);


/**
 * \brief Set the radio modulation.
 * \param c should be {radio id, node id, -1}.
 * \param modulation the new modulation entity id.
 **/
void radio_set_modulation(call_t *c, entityid_t modulation);


/**
 * \brief Get the radio bandwidth.
 * \param c should be {radio id, node id, -1}.
 * \return The current bandwidth, i.e. the time to transmit a bit in ns.
 **/
uint64_t radio_get_Tb(call_t *c);


/**
 * \brief Notifies the radio with a new signal.
 * \param c should be {radio id, node id, antenna id}.
 * \param packet the new signal.
 **/
void radio_cs(call_t *c, packet_t *packet);


/**
 * \brief Set the radio sensibility.
 * \param c should be {radio id, node id, -1}.
 * \param sensibility the new radio sensibility in dBm.
 **/
void radio_set_sensibility(call_t *c, double sensibility);


/**
 * \brief Get the radio sensibility.
 * \param c should be {radio id, node id, -1}.
 * \return The current radio sensibility in dBm.
 **/
double radio_get_sensibility(call_t *c);


/**
 * \brief Set the radio in sleep mode.
 * \param c should be {radio id, node id, -1}.
 **/
void radio_sleep(call_t *c);


/**
 * \brief Set the radio in active mode.
 * \param c should be {radio id, node id, -1}.
 **/
void radio_wakeup(call_t *c);


/* edit by Christophe Savigny <christophe.savigny@insa-lyon.fr> */
/**
 * \brief get the number of bit per symbol for modulation associated
 * \param c should be {radio id, node id, -1}.
 * \return the current number of bit per symbol
 **/
int radio_get_modulation_bit_per_symbol(call_t *c);

/**
 * \brief Get the radio bandwidth.
 * \param c should be {radio id, node id, -1}.
 * \return The current bandwidth, i.e. the time to transmit a symbol in ns.
**/
uint64_t radio_get_Ts(call_t *c);

/**
 * \brief Set the radio bandwidth.
 * Ts must be a bit_per_symbol's multiple
 * \param c should be {radio id, node id, -1}.
 * \param Ts the new bandwidth, i.e. the time to transmit a symbol in ns.
**/
void radio_set_Ts(call_t *c, uint64_t Ts);


/* end of edition */


#endif //__radio_public__
