/**
 *  \file   nrj.c
 *  \brief  Monitor energy and kill events
 *  \author Guillaume Chelius and Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdio.h>
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Monitor energy and death events",
    "Elyes Ben Hamida & Guillaume Chelius",
    "0.1",
    MODELTYPE_MONITOR, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct entitydata {
    void *callbacks;
    int nbr_nodes;
    FILE *filenode;
    char directory[FILENAME_MAX];
    char prefix[FILENAME_MAX];
};

struct monitor_callback {
    callback_t callback;
    call_t c;
    void *arg;
};


/* ************************************************** */
/* ************************************************** */
void monitor_event(call_t *c);
int log_activenodes(call_t *c);


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    char filenode[FILENAME_MAX];
    param_t *param;

    /* default values */
    entitydata->callbacks = das_create();
    entitydata->nbr_nodes = get_node_count();
    strcpy(entitydata->directory,"./");
    strcpy(entitydata->prefix,"activenodes");
    
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        int i = 0;
        if (!strcmp(param->key, "directory")) {
            strcpy(entitydata->directory, param->value);
            goto parse_key_ok;
        } 
        if (!strcmp(param->key, "prefix")) {
            strcpy(entitydata->prefix, param->value);
            goto parse_key_ok;
        } 
        (void) fprintf(stderr,
                       "WARNING: token %s is not recognized\n",
                       param->key);
parse_key_ok:
        i++;
    }
    
    /* open node activity file */
    sprintf(filenode, 
            "%s/%s.data", 
            entitydata->directory, 
            entitydata->prefix);
    if ((entitydata->filenode = fopen(filenode, "w")) == NULL) {
        fprintf(stderr, "NRJ monitor: can not open file %s !\n", filenode);
        goto error;
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
    long time = (long) (get_time() * 0.000001);
    
    fprintf(entitydata->filenode, "%ld %d\n", time, entitydata->nbr_nodes);

    while ((callback = (struct monitor_callback *) das_pop(entitydata->callbacks)) != NULL) {
        free(callback);
    }
    das_destroy(entitydata->callbacks);
    if (entitydata->filenode) {
        fclose(entitydata->filenode);
    }
    free(entitydata);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(call_t *c) {
    /* log initial active nodes number */
    log_activenodes(c);

    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void monitor_death(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);
    
    /* log node death */
    entitydata->nbr_nodes--;
    
    /* log active nodes number */
    log_activenodes(c);
    
    /* monitor */
    monitor_event(c);
}


/* ************************************************** */
/* ************************************************** */
void monitor_event(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);
    struct monitor_callback *callback;
    
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
    call->arg = arg;
    das_insert(entitydata->callbacks, (void *) call);
    return;
}


/* ************************************************** */
/* ************************************************** */
int log_activenodes(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);
    long time = (long) (get_time() * 0.000001);
    fprintf(entitydata->filenode, "%ld %d\n", time, entitydata->nbr_nodes);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
monitor_methods_t methods = {monitor_death, 
                             monitor_event, 
                             register_callback};
