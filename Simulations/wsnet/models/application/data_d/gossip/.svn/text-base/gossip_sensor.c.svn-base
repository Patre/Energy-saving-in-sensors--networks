/**
 *  \file   gossip_sensor.c
 *  \brief  Sensor for gossip data dissemination protocols
 *  \author Guillaume Chelius and Elyes Ben Hamida
 *  \date   2007
 **/
#include <include/modelutils.h>

#include "../data_d_common.h"


/**************************************************************************/
/**************************************************************************/
#define GOSSIP_DATA_TYPE 6

struct gossip_data_p {
    int type; 
    int source;
    int metadata;
    int d_seq;
    int d_value;
};


/**************************************************************************/
/**************************************************************************/
struct nodedata {
    int d_source[MAX_METADATA];
    int d_value[MAX_METADATA];
    int d_seq[MAX_METADATA];

    int s_seq[MAX_SOURCE];
    int r_seq[MAX_SINK];

    int overhead;
};

/**************************************************************************/
/**************************************************************************/
model_t model =  {
    "Sensor for gossip data dissemination protocol",
    "Guillaume Chelius & Elyes Ben Hamida",
    "0.1",
    MODELTYPE_APPLICATION, 
    {NULL, 0}
};


/**************************************************************************/
/**************************************************************************/
int init(call_t *c, void *params) {
    return 0;
}

int destroy(call_t *c) {
    return 0;
}


/**************************************************************************/
/**************************************************************************/
int setnode(call_t *c, void *params) {
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    int i = MAX_METADATA;
    int j = MAX_SOURCE;
    int k = MAX_SINK;

    /* init values */
    while (i--) {
        nodedata->d_source[i] = -1;
        nodedata->d_value[i] = -1;
        nodedata->d_seq[i] = -1;
    }
    while (j--) {
        nodedata->s_seq[j] = -1;        
    }
    while (k--) {
        nodedata->r_seq[k] = -1;        
    }

    set_node_private_data(c, nodedata);
    return 0;
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
  
    /* get overhead */
    nodedata->overhead = GET_HEADER_SIZE(&c0);

    return 0;
}


/**************************************************************************/
/**************************************************************************/
void rx_source_adv(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct source_adv_p *source = (struct source_adv_p *) (packet->data + nodedata->overhead);
    struct sensor_adv_p *sensor;
    packet_t *packet0;
    destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

    /* check adv sequence */
    if (source->s_seq <= nodedata->s_seq[source->source]) {
        /* old request */
        packet_dealloc(packet);
        return;
    }
    nodedata->s_seq[source->source] = source->s_seq;
    
    /* reply */
    packet0 = packet_create(c, nodedata->overhead + sizeof(struct sensor_adv_p), -1);
    sensor = (struct sensor_adv_p *) (packet0->data + nodedata->overhead);
    if (SET_HEADER(&c0, packet0, &dst) == -1) {
        packet_dealloc(packet);
        packet_dealloc(packet0);
        return;
    }
    sensor->type = SENSOR_ADV_TYPE;
    sensor->sensor = c->node;
    sensor->source = source->source;
    sensor->s_seq = source->s_seq;
    TX(&c0, packet0);
    packet_dealloc(packet);

    return;
}


/**************************************************************************/
/**************************************************************************/
void rx_sink_adv(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct sink_adv_p *sink = (struct sink_adv_p *) (packet->data + nodedata->overhead);
    struct sensor_data_p *data;
    packet_t *packet0;
    destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

    /* check request sequence */
    if (sink->r_seq <= nodedata->r_seq[sink->sink]) {
        /* old request */
        packet_dealloc(packet);
        return;
    }
    nodedata->r_seq[sink->sink] = sink->r_seq;
    
    /* check wether we have the data */
    if (sink->d_seq > nodedata->d_seq[sink->metadata]) {
        /* our data is not up to date */
        packet_dealloc(packet);
        return;
    }

    /* reply */
    packet0 = packet_create(c, nodedata->overhead + sizeof(struct sensor_data_p), -1);
    data = (struct sensor_data_p *) (packet0->data + nodedata->overhead);
    if (SET_HEADER(&c0, packet0, &dst) == -1) {
        packet_dealloc(packet);
        packet_dealloc(packet0);
        return;
    }
    data->type = SENSOR_DATA_TYPE;
    data->metadata = sink->metadata;
    data->sink = sink->sink;
    data->r_seq = sink->r_seq;
    data->source = nodedata->d_source[sink->metadata];
    data->d_seq = nodedata->d_seq[sink->metadata];
    data->d_value = nodedata->d_value[sink->metadata];
    TX(&c0, packet0);
    packet_dealloc(packet);

    return;
}


/**************************************************************************/
/**************************************************************************/
void rx_source_data(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct source_data_p *data = (struct source_data_p *) (packet->data + nodedata->overhead);
    struct gossip_data_p *gossip;
    packet_t *packet0;
    destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

    /* check sensor */
    if (data->sensor != c->node) {
        /* not for us */
        packet_dealloc(packet);
        return;
    }

    /* check data sequence */
    if (data->d_seq <= nodedata->d_seq[data->metadata]) {
        /* old data */
        packet_dealloc(packet);
        return;
    }
    nodedata->d_seq[data->metadata] = data->d_seq;
    nodedata->d_source[data->metadata] = data->source;
    nodedata->d_value[data->metadata] = data->d_value;
    
    /* start gossip */
    packet0 = packet_create(c, nodedata->overhead + sizeof(struct gossip_data_p), -1);
    gossip = (struct gossip_data_p *) (packet0->data + nodedata->overhead);
    if (SET_HEADER(&c0, packet0, &dst) == -1) {
        packet_dealloc(packet);
        packet_dealloc(packet0);
        return;
    }
    gossip->type = GOSSIP_DATA_TYPE;
    gossip->source = data->source;
    gossip->metadata = data->metadata;
    gossip->d_seq = data->d_seq;
    gossip->d_value = data->d_value;
    TX(&c0, packet0);
    packet_dealloc(packet);

    return;
}


/**************************************************************************/
/**************************************************************************/
void rx_gossip_data(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct gossip_data_p *data = (struct gossip_data_p *) (packet->data + nodedata->overhead);
    destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

    /* check data sequence */
    if (data->d_seq <= nodedata->d_seq[data->metadata]) {
        /* old data */
        packet_dealloc(packet);
        return;
    }
    nodedata->d_seq[data->metadata] = data->d_seq;
    nodedata->d_source[data->metadata] = data->source;
    nodedata->d_value[data->metadata] = data->d_value;
    
    /* forward gossip */
    if (SET_HEADER(&c0, packet, &dst) == -1) {
        packet_dealloc(packet);
        return;
    }
    TX(&c0, packet);    
}


/**************************************************************************/
/**************************************************************************/
void rx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct data_d_header *header = (struct data_d_header *) (packet->data + nodedata->overhead);

    switch (header->type) {
    case SOURCE_ADV_TYPE:
        /* for us */
        rx_source_adv(c, packet);
        break;
    case SENSOR_ADV_TYPE:
        /* not for us */
        packet_dealloc(packet);
        break;
    case SOURCE_DATA_TYPE:
        /* for us */
        rx_source_data(c, packet);
        break;
    case SINK_ADV_TYPE:
        /* for us */
        rx_sink_adv(c, packet);
        break;
    case SENSOR_DATA_TYPE:
       /* not for us */
        packet_dealloc(packet);
        break;
    case HOME_ADV_TYPE:
        /* not for us */
        packet_dealloc(packet);
        break;
    case GOSSIP_DATA_TYPE:
        /* for us */
        rx_gossip_data(c, packet);
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
