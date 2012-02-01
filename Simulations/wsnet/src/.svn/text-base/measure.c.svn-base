/**
 *  \file   measure.c
 *  \brief  Measure module
 *  \author Guillaume Chelius
 *  \date   2007
 */
#include "internals.h"


/* ************************************************** */
/* ************************************************** */
measure_array_t measures = {0, NULL};
void *with_entities = NULL;


/* ************************************************** */
/* ************************************************** */
measureid_t get_measureid_by_name(char *name) {
    int i = measures.size;
    while (i--)
	if (!strcmp(name, (measures.elts + i)->name))
	    break;
    return i;
}

measure_t *get_measure_by_name(char *name) {
    measureid_t id = get_measureid_by_name(name);
    return (id < 0) ? NULL : get_measure_by_id(id);
}

measure_t *get_measure_by_id(measureid_t id) {
    return measures.elts + id;
}


/* ************************************************** */
/* ************************************************** */
int measure_init(void) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int measure_bootstrap(void) {
    entity_t *entity;

    /* bootstrap monitor */
    das_init_traverse(with_entities);
    while ((entity = (entity_t *) das_traverse(with_entities)) != NULL) {
        
        if (entity->model->type != MODELTYPE_ENVIRONMENT) {
            fprintf(stderr, "entity %s is not an environment model\n", entity->name);
            return -1;
        }
        
        if (entity->bootstrap) {
            call_t c = {entity->id, -1, -1};
            entity->bootstrap(&c);
        }
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
void READ_MEASURE(call_t *c, measureid_t id, double *value) {
    measure_t *measure = get_measure_by_id(id);
    call_t c1 = {measure->entity->id, c->node, c->entity};
    measure->entity->methods->environment.read_measure(&c1, id, value);    
}


/* ************************************************** */
/* ************************************************** */
void measure_clean(void) {
    das_destroy(with_entities);
    if (measures.size) {
        free(measures.elts);
    }
}


/* ************************************************** */
/* ************************************************** */
void print_measure(measure_t *measure) {
    fprintf(stderr, "\nMeasure '%s' (%d)\n", measure->name, measure->id);
    fprintf(stderr, "    exported by : %s\n", measure->entity->name);
}
