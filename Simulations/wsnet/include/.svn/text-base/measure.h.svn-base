/**
 *  \file   measure.h
 *  \brief  Measure declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __measure_public__
#define __measure_public__

#include <include/types.h>


/**
 * \brief Return the measure id associated to a measure name.
 * \param name the measure name.
 * \return The measure id.
 **/
measureid_t get_measureid_by_name(char *name);


/**
 * \brief Return the measure id associated to a measure name.
 * \param c should be {caller entity id, caller node id, -1}.
 * \param measure the measure id.
 * \param value a pointer where to put the read value.
 **/
void READ_MEASURE(call_t *c, measureid_t measure, double *value);


#endif //__measure_public__
