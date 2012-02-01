/**
 *  \file   fire.c
 *  \brief  Simple fire model
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include <stdio.h>

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
static char *measures[] = {"temperature"};

model_t model =  {
    "fire module",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_ENVIRONMENT, 
    {measures, 1}
};


/* ************************************************** */
/* ************************************************** */
struct entitydata {
    uint64_t refresh;
    uint64_t start;
    double speed;
    position_t center;
    double heat;
    double death_radius;
    double heat_radius;
    int measure;
};


/* ************************************************** */
/* ************************************************** */
int callback(call_t *c, void *data);


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;

    /* default values */
    entitydata->refresh = 100000000;
    entitydata->start = 0;
    entitydata->speed = 10;
    entitydata->center.x = get_random_x_position();
    entitydata->center.y = get_random_y_position();
    entitydata->center.z = get_random_z_position();
    entitydata->heat = 10;
    entitydata->death_radius = 0;
    entitydata->heat_radius = 0;
    
   /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "start")) {
            if (get_param_time(param->value, &entitydata->start)) {
                goto error;
            }
        }
        if (!strcmp(param->key, "refresh")) {
            if (get_param_time(param->value, &entitydata->refresh)) {
                goto error;
            }
        }
        if (!strcmp(param->key, "speed")) {
            if (get_param_double(param->value, &(entitydata->speed))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "x")) {
            if (get_param_x_position(param->value, &(entitydata->center.x))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "y")) {
            if (get_param_y_position(param->value, &(entitydata->center.y))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "z")) {
            if (get_param_z_position(param->value, &(entitydata->center.z))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "heat")) {
            if (get_param_distance(param->value, &(entitydata->heat))) {
                goto error;
            }
        }
    }

    set_entity_private_data(c, entitydata);
    return 0;

 error:
    free(entitydata);
    return -1;
}

int destroy(call_t *c) {
    free(get_entity_private_data(c));
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);
    
    /* get measure id */
    entitydata->measure = get_measureid_by_name(measures[0]);
    
    /* scheduler fire starter */
    scheduler_add_callback(get_time() + entitydata->start, c, callback, NULL);

    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int callback(call_t *c, void *data) {
    struct entitydata *entitydata = get_entity_private_data(c);
    nodeid_t node;
    
    /* schedule refresh */
    scheduler_add_callback(get_time() + entitydata->refresh, c, callback, NULL);
    
    /* update fire and heat radius */
    entitydata->death_radius = (((double) (get_time() - entitydata->start)) 
                                / ((double) 1000000000)) * entitydata->speed;
    entitydata->heat_radius = entitydata->death_radius + entitydata->heat;
    
    /* log */
    PRINT_REPLAY("environement %"PRId64" fire disc %f %f %f %f\n", get_time(), 
                 entitydata->center.x, entitydata->center.y, entitydata->center.z, 
                 entitydata->death_radius);
    PRINT_REPLAY("environement %"PRId64" heat disc %f %f %f %f\n", get_time(), 
                 entitydata->center.x, entitydata->center.y, entitydata->center.z, 
                 entitydata->heat_radius);
    
    /* eventually kill node */
    for (node = 0; node < get_node_count(); node++) {
        if (distance(get_node_position(node), &(entitydata->center)) <= entitydata->death_radius) {
            node_kill(node);
        }
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
void read_measure(call_t *c, measureid_t measure, double *value) {
    struct entitydata *entitydata = get_entity_private_data(c);
    
    /* check measure id */
    if (measure != entitydata->measure) {
        fprintf(stderr, "environment: measure %d not supported by model %s (reand_measure())\n", 
                measure, "fire");
    }
    
    /* return heat boolean value */
    if (distance(get_node_position(c->node), &(entitydata->center)) <= entitydata->heat_radius) {
        *value = 1;
    } else {
        *value = 0;
    }

    return;
}


/* ************************************************** */
/* ************************************************** */
environment_methods_t methods = {read_measure};
