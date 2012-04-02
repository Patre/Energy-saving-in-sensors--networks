/**
 *  \file   modulation.h
 *  \brief  Modulation declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida & Christophe Savigny
 *  \date   2009
 **/
#ifndef __modulation_public__
#define __modulation_public__

#include <include/types.h>


/* edit by Christophe Savigny <christophe.savigny@insa-lyon.fr> */

/** 
 * \brief Returns the bit per sumbol rate for a given modulation scheme.
 * \param modulation : modulation id.
 * \return The bit per symbol.
 **/
int modulation_bit_per_symbol(entityid_t modulation);

/* end of edition */


#endif //__modulation_public__
