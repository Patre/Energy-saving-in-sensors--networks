/**
 *  \file   measure.h
 *  \brief  Measure declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __measure__
#define __measure__

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
extern void *with_entities;


/* ************************************************** */
/* ************************************************** */
typedef struct _measure {
    measureid_t id;
    char *name;
    entity_t *entity;
} measure_t;


/* ************************************************** */
/* ************************************************** */
typedef struct _measure_array {
    int size;
    measure_t *elts;
} measure_array_t;

extern measure_array_t measures;


/* ************************************************** */
/* ************************************************** */
int get_measureid_by_name(char *name);
measure_t *get_measure_by_name(char *name);
measure_t *get_measure_by_id(measureid_t id);


/* ************************************************** */
/* ************************************************** */
int measure_init(void);
int measure_bootstrap(void);
void measure_clean(void);


/* ************************************************** */
/* ************************************************** */
void print_measure(measure_t *measure);


#endif //__measure__
