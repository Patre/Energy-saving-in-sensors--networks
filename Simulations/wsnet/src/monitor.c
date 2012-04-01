/**
 *  \file   monitor.c
 *  \brief  Monitor module
 *  \author Guillaume Chelius
 *  \date   2007
 */
#include "internals.h"


/* ************************************************** */
/* ************************************************** */
entity_t *monitor_entity = NULL;


/* ************************************************** */
/* ************************************************** */
int monitor_init(void) {
    return 0;
}

void monitor_clean(void) {
}


/* ************************************************** */
/* ************************************************** */
int monitor_bootstrap(void) {
    call_t c = {-1, -1, -1};

    /* bootstrap monitor entity */
    if (monitor_entity) {
        if (monitor_entity->model->type != MODELTYPE_MONITOR) {
            fprintf(stderr, "entity %s is not a monitor model\n", monitor_entity->name);
            return -1;
        }
        c.entity = monitor_entity->id;
        if (monitor_entity->bootstrap) {
            monitor_entity->bootstrap(&c);
        }   
    }
    
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void monitor_simulation(void) {
    if (monitor_entity) {
        call_t c = {monitor_entity->id, -1, -1};
        monitor_entity->methods->monitor.monitor_event(&c);
    }
}

void monitor_register_callback(callback_t callback, call_t *c0, void *arg) {
    if (monitor_entity) {
        call_t c = {monitor_entity->id, c0->node, c0->entity};
        monitor_entity->methods->monitor.monitor_register_callback(&c, callback, arg);
    }
}
