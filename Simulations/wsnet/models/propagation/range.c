/**
 *  \file   range.c
 *  \brief  Range propagation model
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Range propagation",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_PROPAGATION, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct entitydata {
	double range;
};


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;
    
    /* default values */
    entitydata->range = 10;

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "range")) {
            if (get_param_distance(param->value, &(entitydata->range))) {
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
double propagation(call_t *c, packet_t *packet, nodeid_t src, nodeid_t dst, double rxdBm) {
    struct entitydata *entitydata = get_entity_private_data(c);
    double dist = distance(get_node_position(src), get_node_position(dst));
    
    if (dist > entitydata->range) {
        return MIN_DBM;
    } else {
        return rxdBm;
    }
}


/* ************************************************** */
/* ************************************************** */
propagation_methods_t methods = {propagation};
