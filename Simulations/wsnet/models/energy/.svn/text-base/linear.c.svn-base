/**
 *  \file   linear.c
 *  \brief  linearly decreasing battery
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Linearly decreasing battery",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_ENERGY, 
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
struct nodedata {
    double energy;
    double initial;
    double tx;
    double rx;
    double idle;
};


/* ************************************************** */
/* ************************************************** */
int setnode(call_t *c, void *params) {
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    param_t *param;

    /* default values */
    nodedata->energy  = 1000000;
    nodedata->initial = 1000000;
    nodedata->tx      = 1;
    nodedata->rx      = 1;
    nodedata->idle    = nodedata->rx;

   /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "energy")) {
            if (get_param_double(param->value, &(nodedata->energy))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "tx")) {
            if (get_param_double(param->value, &(nodedata->tx))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "rx")) {
            if (get_param_double(param->value, &(nodedata->rx))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "idle")) {
            if (get_param_double(param->value, &(nodedata->idle))) {
                goto error;
            }
        }
    }
    
    nodedata->initial = nodedata->energy;
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
    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void consume_tx(call_t *c, uint64_t duration, double txdBm) {
    struct nodedata *nodedata = get_node_private_data(c);
    nodedata->energy -= duration * nodedata->tx; 
    if (nodedata->energy <= 0) {
        nodedata->energy = 0;
        node_kill(c->node);
    }
    return;
}

void consume_rx(call_t *c, uint64_t duration) {
    struct nodedata *nodedata = get_node_private_data(c);
    nodedata->energy -= duration * nodedata->rx; 
    if (nodedata->energy <= 0) {
        nodedata->energy = 0;
        node_kill(c->node);
    }
    return;
}

void consume_idle(call_t *c, uint64_t duration) {
    struct nodedata *nodedata = get_node_private_data(c);
    nodedata->energy -= duration * nodedata->idle; 
    if (nodedata->energy <= 0) {
        nodedata->energy = 0;
        node_kill(c->node);
    }
    return;
}

void consume(call_t *c, double energy) {
    struct nodedata *nodedata = get_node_private_data(c);
    nodedata->energy -= energy; 
    if (nodedata->energy <= 0) {
        nodedata->energy = 0;
        node_kill(c->node);
    }
    return;
}

double energy_consumed(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    return nodedata->initial - nodedata->energy;
}

double energy_remaining(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    return nodedata->energy;
}

double energy_status(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    double status = nodedata->energy / nodedata->initial;
    if (nodedata->energy <= 0) {
        return 0;
    } else if ((status >= 0) && (status <= 1)) {
        return status;
    } else {
        return 0;
    }
}


/* ************************************************** */
/* ************************************************** */
energy_methods_t methods = {consume_tx, 
                            consume_rx,
                            consume_idle,
                            consume,
                            energy_consumed,
                            energy_remaining,
                            energy_status};
