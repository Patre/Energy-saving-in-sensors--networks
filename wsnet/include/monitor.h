/**
 *  \file   monitor.h
 *  \brief  Monitor declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __monitor_public__
#define __monitor_public__

#include <include/types.h>


/**
 * \brief Call the "monitor_event()" function of the monitoring entity.
 **/
void monitor_simulation(void);


/**
 * \brief Register a function that is called whenever the network is monitored.
 * \param callback the function that is called back.
 * \param c the call parameter given to the callback function.
 * \param arg a paramater that given to the callback function.
 **/
void monitor_register_callback(callback_t callback, call_t *c, void *arg);


#endif //__monitor_public__
