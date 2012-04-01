/**
 *  \file   data_d_source.c
 *  \brief  Source for data dissemination protocols
 *  \author Guillaume Chelius and Elyes Ben Hamida
 *  \date   2007
 **/
#include <include/modelutils.h>

#include "data_d_common.h"


/**************************************************************************/
/**************************************************************************/
#define IDLE_STATE 0
#define ADV_STATE  1


/**************************************************************************/
/**************************************************************************/
struct nodedata {
    int source;
    uint64_t d_start;
    uint64_t d_period;    
    int d_seq;    
    int metadata;    
    
    int state;
    int s_seq;    

    int overhead;
};

struct entitydata {
    int source;
};


/**************************************************************************/
/**************************************************************************/
model_t model =  {
    "Source for data dissemination protocols",
    "Guillaume Chelius & Elyes Ben Hamida",
    "0.1",
    MODELTYPE_APPLICATION, 
    {NULL, 0}
};


/**************************************************************************/
/**************************************************************************/
int fsm(call_t *c, void * arg);


/**************************************************************************/
/**************************************************************************/
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    /* init source id */
    entitydata->source = 0;
    set_entity_private_data(c, entitydata);
    return 0;
}

int destroy(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);
    free(entitydata);
    return 0;
}


/**************************************************************************/
/**************************************************************************/
int setnode(call_t *c, void *params) {
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    struct entitydata *entitydata = get_entity_private_data(c);
    param_t *param;

    /* default values */
    nodedata->source = entitydata->source++;
    nodedata->d_start = 0;
    nodedata->d_period = 1000000000;	
    nodedata->d_seq = 0;	
    nodedata->metadata = 0;	
    nodedata->state = IDLE_STATE;	
    nodedata->s_seq = 0;	

    /* parse config file */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "data-start")) {
            if (get_param_time(param->value, &(nodedata->d_start))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "data-period")) {
            if (get_param_time(param->value, &(nodedata->d_period))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "metadata")) {
            if (get_param_integer(param->value, &(nodedata->metadata))) {
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
    struct nodedata *nodedata = get_node_private_data(c);
    free(nodedata);
    return 0;
}


/**************************************************************************/
/**************************************************************************/
int bootstrap(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    uint64_t schedule = get_time() + nodedata->d_start + get_random_double() * nodedata->d_period;
  
    nodedata->overhead = GET_HEADER_SIZE(&c0);

    /* scheduler first request */
    scheduler_add_callback(schedule, c, fsm, NULL);
    return 0;
}


/**************************************************************************/
/**************************************************************************/
int fsm(call_t *c, void * arg) {
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    packet_t *packet = packet_create(c, nodedata->overhead + sizeof(struct source_adv_p), -1);
    struct source_adv_p *adv = (struct source_adv_p *) (packet->data + nodedata->overhead);
    destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
    
    /* update interval variables */
    nodedata->state = ADV_STATE;
    nodedata->s_seq++;

   /* send av */
    if (SET_HEADER(&c0, packet, &dst) == -1) {
        packet_dealloc(packet);
        return -1;
    }
    adv->type = SOURCE_ADV_TYPE;
    adv->source = nodedata->source;
    adv->s_seq = nodedata->s_seq;
    TX(&c0, packet);

    /* schedule next request */
    scheduler_add_callback(get_time() + nodedata->d_period, c, fsm, NULL);
    return 0;
}


/**************************************************************************/
/**************************************************************************/
void rx_sensor_adv(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct sensor_adv_p *adv = (struct sensor_adv_p *) (packet->data + nodedata->overhead);
    struct source_data_p *data;
    packet_t *packet0;
    destination_t dst = {adv->sensor, {-1, -1, -1}};
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

    /* check source id */
    if (adv->source != nodedata->source) {
        /* not for us */
        packet_dealloc(packet);
        return;
    }
    
    /* check adv sequence */
    if (adv->s_seq != nodedata->s_seq) {
        /* old request */
        packet_dealloc(packet);
        return;
    }

    /* check state */
    if (nodedata->state != ADV_STATE) {
        /* data already sent */
        packet_dealloc(packet);
        return;
    }

    /* send data packet */
    packet0 = packet_create(c, nodedata->overhead + sizeof(struct source_data_p), -1);
    data = (struct source_data_p *) (packet0->data + nodedata->overhead);
    data->type = SOURCE_DATA_TYPE;
    data->source = nodedata->source;
    data->sensor = adv->sensor;
    data->metadata = nodedata->metadata;
    data->d_seq = nodedata->d_seq++;
    data->d_value = nodedata->d_seq;
    if (SET_HEADER(&c0, packet0, &dst) == -1) {
        packet_dealloc(packet);
        packet_dealloc(packet0);
        return;
    }
    TX(&c0, packet0);
    packet_dealloc(packet);

    /* return to idle state */
    nodedata->state = IDLE_STATE;

    return;
}


/**************************************************************************/
/**************************************************************************/
void rx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct data_d_header *header = (struct data_d_header *) (packet->data + nodedata->overhead);

    switch (header->type) {
    case SOURCE_ADV_TYPE:
        /* not for us */
        packet_dealloc(packet);
        break;
    case SENSOR_ADV_TYPE:
        /* for us */
        rx_sensor_adv(c, packet);
        break;
    case SOURCE_DATA_TYPE:
        /* not for us */
        packet_dealloc(packet);
        break;
    case SINK_ADV_TYPE:
        /* not for us */
        packet_dealloc(packet);
        break;
    case SENSOR_DATA_TYPE:
        /* not for us */
        packet_dealloc(packet);
        break;
    case HOME_ADV_TYPE:
        /* not for us */
        packet_dealloc(packet);
        break;
    default:
        /* not for us */
        packet_dealloc(packet);
        break;
    }

    return;
}


/**************************************************************************/
/**************************************************************************/
application_methods_t methods = {rx};
