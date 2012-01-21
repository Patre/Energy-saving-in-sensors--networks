/**
 *  \file   xmlparser.h
 *  \brief  XML parser declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __xmlparser__
#define __xmlparser__

#include <string.h>
#include <assert.h>
#include <math.h>

#include <glib.h>
#include <gmodule.h>

#include <config.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/xmlschemas.h>

#include "internals.h"


/* ************************************************** */
/* ************************************************** */
#define DEFAULT_STRING     "0"


/* ************************************************** */
/* ************************************************** */
/* XML keywords (tag, entity, namespace, xpath) */

/* Namespace */
#define XML_NS_ID		"w"
#define XML_NS_URL		"http://worldsens.citi.insa-lyon.fr"

/* Elements */
#define XML_E_WITH          "with"
#define XML_E_WORLDSENS     "worldsens"
#define XML_E_SIMULATION    "simulation"
#define XML_E_ENTITY        "entity"
#define XML_E_INIT          "init"
#define XML_E_DEFAULT       "default"
#define XML_E_ENVIRONMENT   "environment"
#define XML_E_PROPAGATION   "propagation"
#define XML_E_INTERFERENCES "interferences"
#define XML_E_MONITORING    "monitoring"
#define XML_E_NOISE         "noise"
#define XML_E_MODULATION    "modulation"
#define XML_E_BUNDLE        "bundle"
#define XML_E_MOBILITY      "mobility"
#define XML_E_ENERGY        "energy"
#define XML_E_ANTENNA       "antenna"
#define XML_E_UP            "up"
#define XML_E_DOWN          "down"
#define XML_E_NODE          "node"
#define XML_E_FOR           "for"

/* Attrributes */
#define XML_A_DURATION    "duration"
#define XML_A_NODES       "nodes"
#define XML_A_X           "x"
#define XML_A_Y           "y"
#define XML_A_Z           "z"
#define XML_A_NAME        "name"
#define XML_A_LIBRARY     "library"
#define XML_A_ENTITY      "entity"
#define XML_A_RANGE       "range"
#define XML_A_DEFAULT     "default"
#define XML_A_WORLDSENS   "worldsens"
#define XML_A_BIRTH       "birth"
#define XML_A_ID          "id"
#define XML_A_AS          "as"

/* XPath */
#define XML_X_SIMULATION  "/" XML_NS_ID ":" XML_E_WORLDSENS	\
                          "/" XML_NS_ID ":" XML_E_SIMULATION
#define XML_X_ENTITY      "/" XML_NS_ID ":" XML_E_WORLDSENS	\
                          "/" XML_NS_ID ":" XML_E_ENTITY
#define XML_X_ENVIRONMENT "/" XML_NS_ID ":" XML_E_WORLDSENS	\
                          "/" XML_NS_ID ":" XML_E_ENVIRONMENT
#define XML_X_BUNDLE      "/" XML_NS_ID ":" XML_E_WORLDSENS	\
                          "/" XML_NS_ID ":" XML_E_BUNDLE
#define XML_X_NODE        "/" XML_NS_ID ":" XML_E_WORLDSENS	\
                          "/" XML_NS_ID ":" XML_E_NODE


/* ************************************************** */
/* ************************************************** */
void config_set_configfile(char *c);
void config_set_schemafile(char *s);
void config_set_sys_modulesdir(char *c);
int do_configuration(void);


/* ************************************************** */
/* ************************************************** */
typedef struct _dflt_param {
    entityid_t entityid;
    bundleid_t bundleid;
    nodeid_t nodeid;
    void * params;
} dflt_param_t;

extern void *dflt_params;

void *get_entity_params(nodeid_t node, entityid_t entity, bundleid_t bundle);


/* ************************************************** */
/* ************************************************** */
extern gchar **user_path_list;
extern gchar **sys_path_list;


#endif //__xmlparser__
