/**
 *  \file   entity.h
 *  \brief  Entity declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida & Quentin Lampin
 *  \date   2007
 **/
#ifndef __entity__
#define __entity__

#include <glib.h>
#include <gmodule.h>

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
typedef struct _generic_methods {
    void (*rx) (call_t *c, packet_t *packet);
    void (*tx) (call_t *c, packet_t *packet);
    int (*set_header) (call_t *c, packet_t *packet, void *dst);
    int (*get_header_size) (call_t *c);
    /* edit by Quentin Lampin <quentin.lampin@orange-ftgroup.com> */
    int (*get_header_real_size) (call_t *c);
    /* end of edition */
} generic_methods_t;

typedef union _methods {
    generic_methods_t generic;
    environment_methods_t environment;
    mobility_methods_t mobility;
    antenna_methods_t antenna;
    radio_methods_t radio;
    mac_methods_t mac;
    routing_methods_t routing;
    application_methods_t application;
    energy_methods_t energy;
    monitor_methods_t monitor;
    propagation_methods_t propagation;
    interferences_methods_t interferences;
    modulation_methods_t modulation;
    noise_methods_t noise;
} methods_t;

typedef struct _library {
    gchar *name;
    GModule *module;
    gchar *file;
} library_t;

typedef struct _entity {
    entityid_t id;
    char *name;
    
    library_t library;
    
    model_t *model;
    methods_t *methods;
    
    int (*init) (call_t *c, void *params);
    int (*destroy) (call_t *c);
    int (*bootstrap) (call_t *c);
    int (*setnode) (call_t *c, void *params);
    int (*unsetnode) (call_t *c);
    int (*ioctl) (call_t *c, int option, void *in, void **out);
    
    void *private;
    
    array_t bundles;
} entity_t;


/* ************************************************** */
/* ************************************************** */
typedef struct _entity_array {
    int size;
    entity_t *elts;
} entity_array_t;

extern entity_array_t entities;


/* ************************************************** */
/* ************************************************** */
int get_entityid_by_name(char *name);
entity_t *get_entity_by_name(char *name);
entity_t *get_entity_by_id(entityid_t id);


/* ************************************************** */
/* ************************************************** */
int entity_init(void);
int entity_bootstrap(void);
void entity_clean(void);


/* ************************************************** */
/* ************************************************** */
void print_entity(entity_t *entity);


#endif //__entity__
