/**
 *  \file   noise.c
 *  \brief  Monitor noise level around each node
 *  \author Guillaume Chelius and Elyes Ben Hamida & Eric Fleury
 *  \date   2007
 **/
#include <stdio.h>
#include <string.h>
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Monitor noise level",
    "Eric Fleury & Elyes Ben Hamida & Guillaume Chelius",
    "0.1",
    MODELTYPE_MONITOR, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct entitydata {
    void *callbacks;
    char directory[FILENAME_MAX];
    char map_prefix[FILENAME_MAX];
    uint64_t map_period;
    int dBmUnit;
    double mWscale;
};

struct monitor_callback {
    callback_t callback;
    call_t c;
    void *arg;
};


/* ************************************************** */
/* ************************************************** */
int log_noisemap(call_t *c);
int map_callback(call_t *c, void *args);


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;

    /* default values */
    entitydata->callbacks = das_create();
    strcpy(entitydata->map_prefix, "noisemap");
    strcpy(entitydata->directory,"./");
    entitydata->map_period = 1000000000;
    entitydata->dBmUnit = 0;
    entitydata->mWscale = 100;
    
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        int i = 0;
        if (!strcmp(param->key, "directory")) {
            strcpy(entitydata->directory, param->value);
            goto parse_key_ok;
        } 
        if (!strcmp(param->key, "prefix")) {
            strcpy(entitydata->map_prefix, param->value);
            goto parse_key_ok;
        } 
        
        if (!strcmp(param->key, "map-period")) {
            if (get_param_time(param->value, &(entitydata->map_period))) {
                goto error;
            }
        goto parse_key_ok;
        }
        if (!strcmp(param->key, "mW-scale")) {
            if (get_param_double(param->value, &(entitydata->mWscale))) {
                goto error;
            }
        goto parse_key_ok;
        }
        if (!strcmp(param->key, "in-dBm")) {
            if (get_param_integer(param->value, &(entitydata->dBmUnit))) {
                goto error;
            }
        goto parse_key_ok;
        }
        (void) fprintf(stderr, 
                       "WARNING: token %s is not recognized\n",
                       param->key);
parse_key_ok:
        i++;
    }
    
    set_entity_private_data(c, entitydata);
    return 0;

 error:
    free(entitydata);
    return -1;
}

int destroy(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);
    struct monitor_callback *callback;
    
    while ((callback = (struct monitor_callback *) das_pop(entitydata->callbacks)) != NULL) {
        free(callback);
    }
    das_destroy(entitydata->callbacks);
    free(entitydata);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);

    if (entitydata->map_period > 0) {
        scheduler_add_callback(entitydata->map_period, c, map_callback, NULL);
    }
    
    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void monitor_death(call_t *c) {
    return;
}


/* ************************************************** */
/* ************************************************** */
void monitor_event(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);
    struct monitor_callback *callback;
    
    /* log energy map */
    log_noisemap(c);
    
    /* call callbacks */
    das_init_traverse(entitydata->callbacks);
    while ((callback = (struct monitor_callback *) das_traverse(entitydata->callbacks)) != NULL) {
        callback->callback(&(callback->c), callback->arg);
    }    
}


/* ************************************************** */
/* ************************************************** */
void register_callback(call_t *c, callback_t callback, void *arg) {
    struct entitydata *entitydata = get_entity_private_data(c);
    struct monitor_callback *call = malloc(sizeof(struct monitor_callback));
    call->callback = callback;
    call->c.entity = c->from;
    call->c.node = c->node;
    call->c.from = -1;
    call->arg = arg;
    das_insert(entitydata->callbacks, (void *) call);
    return;
}


/* ************************************************** */
/* ************************************************** */
int map_callback(call_t *c, void *args) {
    struct entitydata *entitydata = get_entity_private_data(c);
    log_noisemap(c);
    scheduler_add_callback(get_time() + entitydata->map_period, c, map_callback, NULL);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int log_noisemap(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);
    int i = get_node_count();
    FILE *file = NULL;
    char file_map[100];
    double time = get_time() * 0.000001;
    
    /* create file */
    sprintf(file_map, "%s/%s.%.0lf.data", entitydata->directory, entitydata->map_prefix, time);    
    if ((file = fopen(file_map, "w+")) == NULL) {
        fprintf(stderr, "My monitor: can not open file %s in monitor_event()\n", file_map);
        return -1;
    }
    
    /* log energy map */
    while (i--) {
        call_t c0 = {-1, i, -1};
        array_t *radios = get_radio_entities(&c0);
        position_t *position = get_node_position(c0.node);
        c0.entity = radios->elts[0];

        if (entitydata->dBmUnit) {
            fprintf(file, "%d %lf %lf %lf %lf\n", c0.node, position->x, position->y, position->z, radio_get_noise(&c0));
        } else {
            fprintf(file, "%d %lf %lf %lf %lf\n", c0.node, position->x, position->y, position->z, dBm2mW(radio_get_noise(&c0)) / entitydata->mWscale);
        }
    }

    /* log virtual nodes for the 4 area corners */
        if (entitydata->dBmUnit) {
            fprintf(file, "%d %lf %lf %lf %lf\n", -1, 0.0, 0.0, 0.0, MIN_DBM);
            fprintf(file, "%d %lf %lf %lf %lf\n", -1, get_topology_area()->x, 0.0, 0.0, MIN_DBM);
            fprintf(file, "%d %lf %lf %lf %lf\n", -1, 0.0, get_topology_area()->y, 0.0, MIN_DBM);
            fprintf(file, "%d %lf %lf %lf %lf\n", -1, get_topology_area()->x, get_topology_area()->y, 0.0, MIN_DBM);
        } else {
            fprintf(file, "%d %lf %lf %lf %lf\n", -1, 0.0, 0.0, 0.0, 0.0);
            fprintf(file, "%d %lf %lf %lf %lf\n", -1, get_topology_area()->x, 0.0, 0.0, 0.0);
            fprintf(file, "%d %lf %lf %lf %lf\n", -1, 0.0, get_topology_area()->y, 0.0, 0.0);
            fprintf(file, "%d %lf %lf %lf %lf\n", -1, get_topology_area()->x, get_topology_area()->y, 0.0, 0.0);
        }

    fclose(file);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
monitor_methods_t methods = {monitor_death, 
                             monitor_event, 
                             register_callback};
