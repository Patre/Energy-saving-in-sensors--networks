/**
 *  \file   teleport.c
 *  \brief  Teleport mobility
 *  \author Guillaume Chelius and Elyes Ben Hamida
 *  \date   2007
 **/
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Teleport mobility",
    "Elyes Ben Hamida",
    "0.1",
    MODELTYPE_MOBILITY, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */

struct nodedata {
    uint64_t pausetime;
    uint64_t lupdate;
    uint64_t nupdate;
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
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    param_t *param;
    uint64_t min_pausetime, max_pausetime;
    
    /* default values */
    get_node_position(c->node)->x = get_random_x_position();
    get_node_position(c->node)->y = get_random_y_position();
    get_node_position(c->node)->z = get_random_z_position();

    nodedata->pausetime = 2000000000;
    min_pausetime       = 0;
    max_pausetime       = 0;

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
        if (!strcmp(param->key, "pausetime")) {
            if (get_param_time(param->value, &(nodedata->pausetime))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "min-pausetime")) {
            if (get_param_time(param->value, &min_pausetime)) {
                goto error;
            }
        }
        if (!strcmp(param->key, "max-pausetime")) {
            if (get_param_time(param->value, &max_pausetime)) {
                goto error;
            }
        }
    }
    
    if (min_pausetime < max_pausetime ) {
        nodedata->pausetime = get_random_time_range(min_pausetime, max_pausetime);
    }
    
    set_node_private_data(c, nodedata);
    return 0;

 error:
    free(nodedata);
    return -1;
}

int unsetnode(call_t *c) {
    free(get_node_private_data(c));
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);

    PRINT_REPLAY("mobility %"PRId64" %d %lf %lf %lf\n", get_time(), c->node, 
                 get_node_position(c->node)->x, get_node_position(c->node)->y, 
                 get_node_position(c->node)->z);
    
    nodedata->lupdate = get_time();
    nodedata->nupdate = get_time() + nodedata->pausetime;
    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void update_position(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
        
    while  (nodedata->lupdate < get_time()) {
      
        if (nodedata->nupdate <= get_time()) {
            nodedata->lupdate = nodedata->nupdate;
            nodedata->nupdate += nodedata->pausetime;

            get_node_position(c->node)->x = get_random_x_position();
            get_node_position(c->node)->y = get_random_y_position();
            get_node_position(c->node)->z = get_random_z_position();
            PRINT_REPLAY("mobility %"PRId64" %d %lf %lf %lf\n", nodedata->lupdate, c->node, get_node_position(c->node)->x, get_node_position(c->node)->y, get_node_position(c->node)->z);
        } else {
            nodedata->lupdate = get_time();
            break;
        }
    }

    return;
}


/* ************************************************** */
/* ************************************************** */
mobility_methods_t methods = {update_position};
