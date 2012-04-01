/**
 *  \file   medium.h
 *  \brief  Medium declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __medium_public__
#define __medium_public__

#include <include/types.h>
#include <math.h>



/**
 * \def MIN_DBM 
 * \brief Signal strength of a null signal in dBm.
 **/
#define MIN_DBM -DBL_MAX


/**
 * \def MAX_SNR 
 * \brief Maximum SNR value, when there is no interference nor noise.
 **/
#define MAX_SNR DBL_MAX


/**
 * \brief Convert a dBm value into a mW value.
 * \param dBm the dBm value to convert.
 * \return The converted mW value.
 **/
static inline double dBm2mW(double dBm) {
    return (dBm == MIN_DBM) ? 0 : pow(10, dBm / 10);
}


/**
 * \brief Convert a mW value into a dBm value.
 * \param mW the mW value to convert.
 * \return The converted dBm value.
 **/
static inline double mW2dBm(double mW) {
    return (mW == 0) ? MIN_DBM : (log10(mW) * 10);
}


/**
 * \brief Transmit a packet in the radio medium.
 * \param c should be {radio id, node id, antenna id}.
 * \param packet the transmitted packet.
 **/
void MEDIA_TX(call_t *c, packet_t *packet);


/**
 * \brief Return the radio medium noise on a given channel.
 * \param c should be {radio id, node id, antenna id}.
 * \param channel the radio channel we are listening to.
 * \return the noise strength in dBm.
 **/
double MEDIA_GET_NOISE(call_t *c, int channel);


#endif //__medium_public__
