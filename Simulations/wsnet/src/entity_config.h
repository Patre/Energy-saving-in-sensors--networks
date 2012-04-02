/**
 *  \file   entity_config.h
 *  \brief  Entity declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __entity_config__
#define __entity_config__

#include "xmlparser.h"


/* ************************************************** */
/* ************************************************** */
int parse_entity(xmlNodePtr nd1, int id);
int parse_entities(xmlNodeSetPtr nodeset);


#endif //__entity_config__
