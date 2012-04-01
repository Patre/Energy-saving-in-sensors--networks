/**
 *  \file   isotropic.c
 *  \brief  Isotropic antenna
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Isotropic antenna",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_ANTENNA, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct nodedata {
    double noise;
    angle_t angle;
    double gain_tx;
    double gain_rx;
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

    /* default values */
    nodedata->noise = 0;
    nodedata->gain_tx = 0;
    nodedata->gain_rx = 0;
    nodedata->angle.xy = get_random_double_range(0, 2 * M_PI);
    nodedata->angle.z = get_random_double_range(0, 2 * M_PI);
    
   /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "gain-tx")) {
            if (get_param_double(param->value, &(nodedata->gain_tx))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "gain-rx")) {
            if (get_param_double(param->value, &(nodedata->gain_rx))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "loss")) {
            if (get_param_double(param->value, &(nodedata->noise))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "angle-xy")) {
            if (get_param_double_range(param->value, &(nodedata->angle.xy), 0, 2*M_PI)) {
                goto error;
            }
        }
        if (!strcmp(param->key, "angle-z")) {
            if (get_param_double_range(param->value, &(nodedata->angle.z), 0, 2*M_PI)) {
                goto error;
            }
        }
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
    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
double get_loss(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    return nodedata->noise;
}

angle_t *get_angle(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    return &(nodedata->angle);
}

void set_angle(call_t *c, angle_t *angle) {
    struct nodedata *nodedata = get_node_private_data(c);
    nodedata->angle.xy = angle->xy;
    nodedata->angle.z = angle->z;
    return;
}


/* ************************************************** */
/* ************************************************** */
void cs(call_t *c, packet_t *packet) {
    array_t *up = get_entity_bindings_up(c);
    int i = up->size;

    while (i--) {
        call_t c_up = {up->elts[i], c->node, c->entity};
        radio_cs(&c_up, packet);
    }
    
    return;
}

void rx(call_t *c, packet_t *packet) {
    array_t *up = get_entity_bindings_up(c);
    int i = up->size;
    
    while (i--) {
        call_t c_up = {up->elts[i], c->node, c->entity};
        packet_t *packet_up;
        
        if (i > 0) {
            packet_up = packet_clone(packet);           
        } else {
            packet_up = packet;
        }
        RX(&c_up, packet_up);
    }
    
    return;
}


/* ************************************************** */
/* ************************************************** */
double  gain_tx(call_t *c, position_t *pos) {    
    struct nodedata *nodedata = get_node_private_data(c);
    return nodedata->gain_tx;
}

double  gain_rx(call_t *c, position_t *pos) {    
    struct nodedata *nodedata = get_node_private_data(c);
    return nodedata->gain_rx;
}


/* ************************************************** */
/* ************************************************** */
antenna_methods_t methods = {rx, 
                             cs,
                             get_loss, 
                             get_angle, 
                             set_angle, 
                             gain_tx, 
                             gain_rx};
