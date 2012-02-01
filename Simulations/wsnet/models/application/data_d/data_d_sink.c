/**
 *  \file   data_d_sink.c
 *  \brief  Sink for data dissemination protocols
 *  \author Guillaume Chelius and Elyes Ben Hamida
 *  \date   2007
 **/
#include <include/modelutils.h>

#include "data_d_common.h"


#define LOG_APPLICATION_SINK
#define LOG_APPLICATION_SINK_REQ

/**************************************************************************/
/**************************************************************************/
#define IDLE_STATE 0
#define ADV_STATE  1


/**************************************************************************/
/**************************************************************************/
struct nodedata {
    int sink;
    uint64_t r_start;
    uint64_t r_period;    
    int r_seq;    

    int state;
    int metadata;    
    int d_seq;  
    int d_value;  

    int overhead;
};

struct entitydata {
    int sink;

    /* stats */
    int init;
    int d_rx;
    int attempt;
    int success;
    uint64_t log_period;
    FILE *logfile;

    /* delay */
    double last_packet_time;
    int last_packet;
};


/**************************************************************************/
/**************************************************************************/
model_t model =  {
    "Sink for data dissemination protocols",
    "Guillaume Chelius & Elyes Ben Hamida",
    "0.1",
    MODELTYPE_APPLICATION, 
    {NULL, 0}
};


/**************************************************************************/
/**************************************************************************/
int fsm(call_t *c, void * arg);
int entity_log(call_t *c, void * arg);


/**************************************************************************/
/**************************************************************************/
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    char logfile[100] = "gossip-sink.log";
    char logfile_delay[100] = "";
    param_t *param;

    /* init stats */
    entitydata->sink = 0;
    entitydata->init = 0;
    entitydata->d_rx = 0;
    entitydata->attempt = -1;
    entitydata->success = 0;
    entitydata->log_period = 1000000000;
    entitydata->last_packet_time = -1;
    entitydata->last_packet = 0;


    /* get params */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "logfile")) {
            strcpy(logfile, param->value);
        } 
        if (!strcmp(param->key, "logfile-delay")) {
            strcpy(logfile_delay, param->value);
        } 
        if (!strcmp(param->key, "log-period")) {
            if (get_param_time(param->value, &(entitydata->log_period))) {
                goto error;
            }
        }
     }
 
    /* open node activity file */
    if ((entitydata->logfile = fopen(logfile, "w")) == NULL) {
        fprintf(stderr, "data_d_sink: can not open file %s !\n", logfile);
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
    double val;
    long time = (long) (get_time() * 0.000001);

    if (entitydata->attempt > 0) {
        val = ((double) entitydata->success) / ((double) entitydata->attempt);
        fprintf(entitydata->logfile, "%ld %f %d %d\n", time, val, entitydata->success, entitydata->attempt);
    }

    if (entitydata->logfile) {
        fclose(entitydata->logfile);
    }
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
    nodedata->sink = entitydata->sink++;
    nodedata->r_start = 0;
    nodedata->r_period = 1000000000;	
    nodedata->r_seq = 0;	
    nodedata->state = IDLE_STATE;	
    nodedata->metadata = 0;	
    nodedata->d_seq = -1;	

    /* parse config file */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "request-start")) {
            if (get_param_time(param->value, &(nodedata->r_start))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "request-period")) {
            if (get_param_time(param->value, &(nodedata->r_period))) {
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
    struct entitydata *entitydata = get_entity_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    uint64_t schedule = get_time() + nodedata->r_start + get_random_double() * nodedata->r_period;
  
    /* register log function */
    if (entitydata->init == 0) {
        scheduler_add_callback(get_time() + entitydata->log_period, c, entity_log, NULL);
        entitydata->init = 1;
    }

    /* get overhead */
    nodedata->overhead = GET_HEADER_SIZE(&c0);

    /* scheduler first request */
    scheduler_add_callback(schedule, c, fsm, NULL);
    return 0;
}


/**************************************************************************/
/**************************************************************************/
int fsm(call_t *c, void * arg) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata = get_entity_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    packet_t *packet = packet_create(c, nodedata->overhead + sizeof(struct sink_adv_p), -1);
    struct sink_adv_p *adv = (struct sink_adv_p *) (packet->data + nodedata->overhead);
    destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
    
    /* update interval variables */
    nodedata->state = ADV_STATE;
    nodedata->r_seq++;

    /* forge and send request */
    if (SET_HEADER(&c0, packet, &dst) == -1) {
        packet_dealloc(packet);
        return -1;
    }
    adv->type = SINK_ADV_TYPE;
    adv->sink = nodedata->sink;
    adv->r_seq = nodedata->r_seq;
    adv->metadata = nodedata->metadata;
    adv->home = BROADCAST_ADDR;
    adv->d_seq = nodedata->d_seq + 1;

    TX(&c0, packet);
    if (entitydata->d_rx) {
        entitydata->success++;
    }
    entitydata->attempt++;
    entitydata->d_rx = 0;
    
#ifdef LOG_APPLICATION_SINK_REQ
    printf("%lf SINK %d REQ %d\n", (get_time()*0.000001), c->node, adv->d_seq);
#endif

    /* schedule next request */
    scheduler_add_callback(get_time() + nodedata->r_period, c, fsm, NULL);
    return 0;
}


/**************************************************************************/
/**************************************************************************/
void rx_home_adv(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct home_adv_p *home = (struct home_adv_p *) (packet->data + nodedata->overhead);
    struct sink_adv_p *adv;
    packet_t *packet0;
    destination_t dst = {home->sensor, {-1, -1, -1}};
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

    /* check sink id */
    if (home->sink != nodedata->sink) {
        /* not for us */
        packet_dealloc(packet);
        return;
    }
    
    /* check adv sequence */
    if (home->r_seq != nodedata->r_seq) {
        /* old request */
        packet_dealloc(packet);
        return;
    }

    /* check state */
    if (nodedata->state != ADV_STATE) {
        /* request already sent */
        packet_dealloc(packet);
        return;
    }

    /* send data packet */
    packet0 = packet_create(c, nodedata->overhead + sizeof(struct sink_adv_p), -1);
    adv = (struct sink_adv_p *) (packet0->data + nodedata->overhead);
    if (SET_HEADER(&c0, packet0, &dst) == -1) {
        packet_dealloc(packet);
        packet_dealloc(packet0);
        return;
    }

    adv->type = SINK_ADV_TYPE;
    adv->sink = nodedata->sink;
    adv->r_seq = nodedata->r_seq;
    adv->metadata = nodedata->metadata;
    adv->home = home->sensor;
    adv->d_seq = nodedata->d_seq + 1;

#ifdef LOG_APPLICATION_SINK_REQ
    printf("%lf SINK %d REQ %d\n", (get_time()*0.000001), c->node, adv->d_seq);
#endif

    TX(&c0, packet0);
    packet_dealloc(packet);

    /* return to idle state */
    nodedata->state = IDLE_STATE;

    return;
}


/**************************************************************************/
/**************************************************************************/
void rx_sensor_data(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata = get_entity_private_data(c);
    struct sensor_data_p *data = (struct sensor_data_p *) (packet->data + nodedata->overhead);

    /* check meta data */
    if (data->metadata != nodedata->metadata) {
        /* bad metadata */
        packet_dealloc(packet);
        return;
    }

    /* check sink id */
    if (data->sink != nodedata->sink) {
        /* not for us */
        packet_dealloc(packet);
        return;
    }
    
    /* check request sequence */
    if (data->r_seq != nodedata->r_seq) {
        /* old request */
        packet_dealloc(packet);
        return;
    }
   
    /* check data sequence */
    if (data->d_seq <= nodedata->d_seq) {
        /* old data */
        packet_dealloc(packet);
        return;
    }
    
    /* packet accepted */
    entitydata->d_rx++;
    nodedata->d_seq = data->d_seq;
    nodedata->d_value = data->d_value;

    if (entitydata->last_packet_time == -1) {
      entitydata->last_packet_time = (get_time()*0.000001);
      entitydata->last_packet = 1;
    }
    else {
      entitydata->last_packet ++;
      entitydata->last_packet_time = (get_time()*0.000001);
    }


#ifdef LOG_APPLICATION_SINK
    printf("%lf SINK %d RX %d\n",(get_time()*0.000001), c->node, data->d_seq);
#endif

    packet_dealloc(packet);
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
        /* not for us */
        packet_dealloc(packet);
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
        /* for us */
        rx_sensor_data(c, packet);
        break;
    case HOME_ADV_TYPE:
        /* for us */
        rx_home_adv(c, packet);
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
int entity_log(call_t *c, void * arg) {
    struct entitydata *entitydata = get_entity_private_data(c);
    long time = (long) (get_time() * 0.000001);
    double val;

    scheduler_add_callback(get_time() + entitydata->log_period, c, entity_log, NULL);

    if (entitydata->attempt <= 0) {
        return 0;
    }
    val = ((double) entitydata->success) / ((double) entitydata->attempt);
    fprintf(entitydata->logfile, "%ld %f %d %d\n", time, val, entitydata->success, entitydata->attempt);
    
    entitydata->success = 0;
    entitydata->attempt = 0;
    return 0;
}


/**************************************************************************/
/**************************************************************************/
application_methods_t methods = {rx};
