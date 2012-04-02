/**
 *  \file   bundle_config.h
 *  \brief  Bundle declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __bundle_config__
#define __bundle_config__

#include "xmlparser.h"


/* ************************************************** */
/* ************************************************** */
extern bundle_t *dflt_bundle;


/* ************************************************** */
/* ************************************************** */
int parse_bundle(xmlNodePtr nd, int id);
int parse_bundles(xmlNodeSetPtr nodeset);


#endif //__bundle_config__
