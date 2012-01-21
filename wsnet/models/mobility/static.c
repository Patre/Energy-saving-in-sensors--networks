/**
 *  \file   static.c
 *  \brief  no mobility
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "No mobility",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_MOBILITY, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    return 0;
}

int destroy(call_t *c) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int setnode(call_t *c, void *params) {
    param_t *param;

    /* default values */
    get_node_position(c->node)->x = get_random_x_position();
    get_node_position(c->node)->y = get_random_y_position();
    get_node_position(c->node)->z = get_random_z_position();
    
   /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "x")) {
            if (get_param_x_position(param->value, &(get_node_position(c->node)->x))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "y")) {
            if (get_param_y_position(param->value, &(get_node_position(c->node)->y))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "z")) {
            if (get_param_z_position(param->value, &(get_node_position(c->node)->z))) {
                goto error;
            }
        }
    }

    return 0;
    
 error:
    return -1;
}

int unsetnode(call_t *c) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(call_t *c) {
    PRINT_REPLAY("mobility %"PRId64" %d %lf %lf %lf\n", get_time(), c->node, 
                 get_node_position(c->node)->x, get_node_position(c->node)->y, 
                 get_node_position(c->node)->z);
    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void update_position(call_t *c) {
    return;
}


/* ************************************************** */
/* ************************************************** */
mobility_methods_t methods = {update_position};
