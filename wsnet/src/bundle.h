/**
 *  \file   bundle.h
 *  \brief  Bundle declarations
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#ifndef __bundle__
#define __bundle__

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
typedef int bundleid_t;


/* ************************************************** */
/* ************************************************** */
typedef struct _bundle {
    bundleid_t id;
    char *name;
    int worldsens;

    char *birth;

    entityid_t mobility;
    entityid_t energy;
    array_t entity;
    array_t antenna;
    array_t radio;
    array_t mac;
    array_t routing;
    array_t application;

    array_t *up;
    array_t *down;
} bundle_t;


/* ************************************************** */
/* ************************************************** */
typedef struct _bundle_array {
    int size;
    bundle_t *elts;
} bundle_array_t;

extern bundle_array_t bundles;


/* ************************************************** */
/* ************************************************** */
int get_bundleid_by_name(char *name);
bundle_t *get_bundle_by_name(char *name);
bundle_t *get_bundle_by_id(bundleid_t id);


/* ************************************************** */
/* ************************************************** */
int bundle_init(void);
int bundle_bootstrap(void);
void bundle_clean(void);


/* ************************************************** */
/* ************************************************** */
void print_bundle(bundle_t *bundle);


#endif //__bundle__
