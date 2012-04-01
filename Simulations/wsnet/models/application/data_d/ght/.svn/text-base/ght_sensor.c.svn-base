/**
 *  \file   ght_sensor.c
 *  \brief  Sensor for ght data dissemination protocols
 *  \author Guillaume Chelius and Elyes Ben Hamida
 *  \date   2007
 **/
#include <include/modelutils.h>

#include "../data_d_common.h"


/* ************************************************** */
/* ************************************************** */
struct ght_node {
    int metadata;
    position_t position;
};

struct ght_neighbor {
	int id;
    position_t position;                         
	uint64_t time;
};


/* ************************************************** */
/* ************************************************** */
#define GHT_HELLO_TYPE    10
#define GHT_DATA_TYPE     11
#define GHT_REQUEST_TYPE  12
#define GHT_RESPONSE_TYPE 13


/* ************************************************** */
/* ************************************************** */
struct ght_hello_p {
    int type;
    nodeid_t src;
    position_t position;
};

struct ght_data_p {
    int type;  
    int n_hop;
    int source;
    int metadata;
    int d_seq;
    int d_value;
};

struct ght_request_p {
    int type;  
    int n_hop;
    int sink;
    position_t position;
    int r_seq;
    int metadata;
    int d_seq;
};

struct ght_response_p {
    int type;  
    int n_hop;
    int sink;
    int source;
    position_t position;
    int r_seq;
    int metadata;
    int d_seq;
    int d_value;
};


/**************************************************************************/
/**************************************************************************/
struct nodedata {
    int overhead;
    
    /* Hello protocol */
    uint64_t h_start;
    uint64_t h_timeout;	
    uint64_t h_period;	

    /* storage */
    void *neighbors;
    int s_seq[MAX_SOURCE];
    int r_seq[MAX_SINK];
    int d_source[MAX_METADATA];
    int d_value[MAX_METADATA];
    int d_seq[MAX_METADATA];
};

struct entitydata {
    void *ght_nodes;
};


/**************************************************************************/
/**************************************************************************/
model_t model =  {
    "Sensor for ght data dissemination protocol",
    "Elyes Ben Hamida",
    "0.1",
    MODELTYPE_APPLICATION, 
    {NULL, 0}
};


/**************************************************************************/
/**************************************************************************/
int hello_callback(call_t *c, void *args);
void rx_ght_hello(call_t *c, packet_t *packet);
void rx_ght_data(call_t *c, packet_t *packet);
void rx_ght_request(call_t *c, packet_t *packet);
void rx_ght_response(call_t *c, packet_t *packet);
void rx_source_adv(call_t *c, packet_t *packet);
void rx_source_data(call_t *c, packet_t *packet);
void rx_sink_adv(call_t *c, packet_t *packet);

position_t *ght_get_position(call_t *c, int metadata);
struct ght_neighbor *ght_next_hop(call_t *c, position_t *position);


/**************************************************************************/
/**************************************************************************/
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;
    char *filepath = NULL; 
    FILE *file;
    char str[128];

    /* init values */
    entitydata->ght_nodes = das_create();

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "hashfile")) {
            filepath = param->value;
        }
    }
  
    /* get default ght node */
    if (filepath == NULL) {
        struct ght_node *ghtnode = (struct ght_node *) malloc(sizeof(struct ght_node));
        ghtnode->metadata = -1;
        ghtnode->position.x = get_random_x_position();
        ghtnode->position.y = get_random_y_position();
        ghtnode->position.z = get_random_z_position();
        das_insert(entitydata->ght_nodes, (void *) ghtnode);
    } else {    
        if ((file = fopen(filepath, "r")) == NULL) {
            fprintf(stderr, "ght_sensor: can not open file %s in init()\n", filepath);
            goto error;
        }

        /* parse file */
        fseek(file, 0L, SEEK_SET);
        while (fgets(str, 128, file) != NULL) {
            struct ght_node *ghtnode = (struct ght_node *) malloc(sizeof(struct ght_node));
            if (sscanf(str, "%d %lf %lf %lf\n",  
                       &(ghtnode->metadata), 
                       &(ghtnode->position.x), 
                       &(ghtnode->position.y), 
                       &(ghtnode->position.z) ) != 4) {
                fprintf(stderr, "ght_sensor  unable to read ght position in init()\n");
                goto error;
            }
            das_insert(entitydata->ght_nodes, (void *) ghtnode);
        }
        
        fclose(file);
    }

    set_entity_private_data(c, entitydata);
    return 0;

 error:
    free(entitydata);
    return -1;    
}

int destroy(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);
    struct ght_node *ghtnode;
    while ((ghtnode = (struct ght_node *) das_pop(entitydata->ght_nodes)) != NULL) {
        free(ghtnode);
    }
    das_destroy(entitydata->ght_nodes);
    free(entitydata);
    return 0;
}


/**************************************************************************/
/**************************************************************************/
int setnode(call_t *c, void *params) {
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));    
    param_t *param;
    int i = MAX_METADATA;
    int j = MAX_SOURCE;
    int k = MAX_SINK;
    
    /* default values */
    nodedata->h_start = 0;
    nodedata->h_period = 1000000000;	
    nodedata->h_timeout = nodedata->h_period * 2;
    nodedata->neighbors = das_create();
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

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "hello-start")) {
            if (get_param_time(param->value, &(nodedata->h_start)))
                goto error;
        }
        if (!strcmp(param->key, "hello-period")) {
            if (get_param_time(param->value, &(nodedata->h_period)))
                goto error;
        }
        if (!strcmp(param->key, "hello-timeout")) {
            if (get_param_time(param->value, &(nodedata->h_timeout)))
                goto error;
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
    struct ght_neighbor *neighbor;

    while ((neighbor = (struct ght_neighbor *) das_pop(nodedata->neighbors)) != NULL) {
        free(neighbor);
    }
    das_destroy(nodedata->neighbors);

    free(nodedata);
    return 0;
}


/**************************************************************************/
/**************************************************************************/
int bootstrap(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    uint64_t schedule = get_time() + nodedata->h_start + get_random_double() * nodedata->h_period;

    /* get overhead */
    nodedata->overhead = GET_HEADER_SIZE(&c0);

    /* scheduler first hello */
    scheduler_add_callback(schedule, c, hello_callback, NULL);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
struct ght_neighbor *ght_next_hop(call_t *c, position_t *position) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct ght_neighbor *neighbor = NULL, *n_hop = NULL;
    double dist = distance(get_node_position(c->node), position);
    double d = dist;
    uint64_t clock = get_time();

    /* parse neighbors */
    das_init_traverse(nodedata->neighbors);    
    while ((neighbor = (struct ght_neighbor *) das_traverse(nodedata->neighbors)) != NULL) {
        if ((nodedata->h_timeout > 0)
            && (clock - neighbor->time) >= nodedata->h_timeout ) {
            continue;
        }
        
        /* choose next hop */
        if ((d = distance(&(neighbor->position), position)) < dist) {
            dist = d;
            n_hop = neighbor;
        }
    }    
    
    return n_hop;
}


/* ************************************************** */
/* ************************************************** */
position_t *ght_get_position(call_t *c, int metadata) {
    struct entitydata *entitydata = get_entity_private_data(c);
    struct ght_node *ghtnode, *def = NULL, *found = NULL;

    das_init_traverse(entitydata->ght_nodes);    
    while ((ghtnode = (struct ght_node *) das_traverse(entitydata->ght_nodes)) != NULL) {
        if (ghtnode->metadata == metadata) {
            found = ghtnode;
            break;
        } else if (ghtnode->metadata == -1) {
            def = ghtnode;
        }
    }
    if (found) {
        return &(found->position);
    } else if (def) {
        return &(def->position);
    }
    
    return NULL;
}


/**************************************************************************/
/**************************************************************************/
int neighbor_timeout(void *data, void *arg) {
    call_t *c = (call_t *) arg;
    struct nodedata *nodedata = get_node_private_data(c);
    struct ght_neighbor *neighbor = (struct ght_neighbor *) data;

    if ((get_time() - neighbor->time) > nodedata->h_timeout) {
        return 1;
    }

    return 0;
}

int hello_callback(call_t *c, void *args) {
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};
    packet_t *packet = packet_create(c, nodedata->overhead + sizeof(struct ght_hello_p), -1);
    struct ght_hello_p *hello = (struct ght_hello_p *) (packet->data + nodedata->overhead);
    position_t *pos = get_node_position(c->node);

    /* set mac header */
    if (SET_HEADER(&c0, packet, &destination) == -1) {
        packet_dealloc(packet);
        return -1;
    }
    
    /* set header */
    hello->type = GHT_HELLO_TYPE;
    hello->src = c->node;
    hello->position.x = pos->x;
    hello->position.y = pos->y;
    hello->position.z = pos->z;
    TX(&c0, packet);	   
    
    /* check neighbors timeout */
    if (nodedata->h_timeout > 0) {
        das_selective_delete(nodedata->neighbors, neighbor_timeout, (void *) c);
    }
    
    /* schedules hello */
    scheduler_add_callback(get_time() + nodedata->h_period, c, hello_callback, NULL);
    return 0;
}

void rx_ght_hello(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct ght_hello_p *hello = (struct ght_hello_p *) (packet->data + nodedata->overhead);
    struct ght_neighbor *neighbor;

   /* check for existing neighbors */
    das_init_traverse(nodedata->neighbors);      
    while ((neighbor = (struct ght_neighbor *) das_traverse(nodedata->neighbors)) != NULL) {
        if (neighbor->id == hello->src) {
            neighbor->position.x = hello->position.x;
            neighbor->position.y = hello->position.y;
            neighbor->position.z = hello->position.z;
            neighbor->time = get_time();
            packet_dealloc(packet);
            return;
        }
    }  

    /* new neighbor */
    neighbor = (struct ght_neighbor *) malloc(sizeof(struct ght_neighbor));
    neighbor->id = hello->src;
    neighbor->position.x = hello->position.x;
    neighbor->position.y = hello->position.y;
    neighbor->position.z = hello->position.z;
    neighbor->time = get_time();
    das_insert(nodedata->neighbors, (void *) neighbor);
    packet_dealloc(packet);
    return;
}


/* ************************************************** */
/* ************************************************** */
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
void rx_source_data(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct source_data_p *data = (struct source_data_p *) (packet->data + nodedata->overhead);
    position_t *ght_position;
    struct ght_neighbor *n_hop;

    /* check sensor */
    if (data->sensor != c->node) {
        /* not for us */
        packet_dealloc(packet);
        return;
    }
    
   /* get ght storing node */
    ght_position = ght_get_position(c, data->metadata);
    /* get next hop */
    n_hop = ght_next_hop(c, ght_position);
    
    /* if I am the storing node */
    if (n_hop == NULL) {
        /* store information */
        if (data->d_seq <= nodedata->d_seq[data->metadata]) {
            /* old data */
            packet_dealloc(packet);
            return;
        }
        nodedata->d_seq[data->metadata] = data->d_seq;
        nodedata->d_source[data->metadata] = data->source;
        nodedata->d_value[data->metadata] = data->d_value;
        packet_dealloc(packet);
        return;
    } else {
        /* create ght data */
        destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
        packet_t *packet0 = packet_create(c, nodedata->overhead + sizeof(struct ght_data_p), -1);
        struct ght_data_p *disseminate =  (struct ght_data_p *) (packet0->data + nodedata->overhead);
        call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
        
        /* forward data */
        if (SET_HEADER(&c0, packet0, &dst) == -1) {
            packet_dealloc(packet0);
            packet_dealloc(packet);
            return;
        }
        disseminate->type = GHT_DATA_TYPE;
        disseminate->n_hop = n_hop->id;
        disseminate->source = data->source;
        disseminate->metadata = data->metadata;
        disseminate->d_seq = data->d_seq;
        disseminate->d_value = data->d_value;
        TX(&c0, packet0);
        packet_dealloc(packet);
        return;
    }

    return;
}


/**************************************************************************/
/**************************************************************************/
void rx_ght_data(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct ght_data_p *data = (struct ght_data_p *) (packet->data + nodedata->overhead);
    position_t *ght_position;
    struct ght_neighbor *n_hop;

    /* are we the next hop*/
    if (data->n_hop != c->node) {
        packet_dealloc(packet);
        return;
    }

    /* get ght storing node */
    ght_position = ght_get_position(c, data->metadata);
    /* get next hop */
    n_hop = ght_next_hop(c, ght_position);
    
    /* if I am the storing node */
    if (n_hop == NULL) {
        /* store information */
        if (data->d_seq <= nodedata->d_seq[data->metadata]) {
            /* old data */
            packet_dealloc(packet);
            return;
        }
        nodedata->d_seq[data->metadata] = data->d_seq;
        nodedata->d_source[data->metadata] = data->source;
        nodedata->d_value[data->metadata] = data->d_value;
        packet_dealloc(packet);
        return;
    } else {
        /* forward ght data */
        destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
        call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
        
        /* forward data */
        if (SET_HEADER(&c0, packet, &dst) == -1) {
            packet_dealloc(packet);
            return;
        }
        data->n_hop = n_hop->id;
        TX(&c0, packet);
        return;
    }

    return;
}


/* ************************************************** */
/* ************************************************** */
void rx_sink_adv(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct sink_adv_p *sink = (struct sink_adv_p *) (packet->data + nodedata->overhead);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
            
    /* starts home node election */
    if (sink->home == -1) {
        packet_t *packet0;
        struct home_adv_p *adv;
        destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
        
        /* check request sequence */
        if (sink->r_seq <= nodedata->r_seq[sink->sink]) {
            /* old request */
            packet_dealloc(packet);
            return;
        }
        nodedata->r_seq[sink->sink] = sink->r_seq;

        /* reply */
        packet0 = packet_create(c, nodedata->overhead + sizeof(struct home_adv_p), -1);
        adv = (struct home_adv_p *) (packet0->data + nodedata->overhead);
        if (SET_HEADER(&c0, packet0, &dst) == -1) {
            packet_dealloc(packet);
            packet_dealloc(packet0);
            return;
        }
        adv->type = HOME_ADV_TYPE;
        adv->sensor = c->node;
        adv->sink = sink->sink;
        adv->r_seq = sink->r_seq;
        
        TX(&c0, packet0);	   
        packet_dealloc(packet);
        return;
    } else if (sink->home == c->node) {
        /* sends the request towards the hash location */
        position_t *ght_position;
        struct ght_neighbor *n_hop;

        /* get ght storing node */
        ght_position = ght_get_position(c, sink->metadata);
        n_hop = ght_next_hop(c, ght_position);
        
        /* if I am the storing node */
        if (n_hop == NULL) {
            packet_t *packet0;
            struct sensor_data_p *data;
            destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
            
            /* check wether we have the data */
            if (sink->d_seq > nodedata->d_seq[sink->metadata]) {
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
        } else {
            destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
            packet_t *packet0 = packet_create(c, nodedata->overhead + sizeof(struct ght_request_p), -1);
            struct ght_request_p *request =  (struct ght_request_p *) (packet0->data + nodedata->overhead);
            position_t *pos = get_node_position(c->node);

            /* create request */
            if (SET_HEADER(&c0, packet0, &dst) == -1) {
                packet_dealloc(packet);
                packet_dealloc(packet0);
                return;
            }
            request->type = GHT_REQUEST_TYPE;
            request->n_hop = n_hop->id;
            request->sink = sink->sink;
            request->r_seq = sink->r_seq;
            request->metadata = sink->metadata;
            request->d_seq = sink->d_seq;
            request->position.x = pos->x;
            request->position.y = pos->y;
            request->position.z = pos->z;
            TX(&c0, packet0);    
            packet_dealloc(packet);
            return;
        }
    } else {
        packet_dealloc(packet);
        return;
    }
}


/**************************************************************************/
/**************************************************************************/
void rx_ght_request(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct ght_request_p *request = (struct ght_request_p *) (packet->data + nodedata->overhead);
    position_t *ght_position;
    struct ght_neighbor *n_hop;
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

    /* are we the next hop */
    if (request->n_hop != c->node) {
        packet_dealloc(packet);
        return;
    }
    
    /* get ght storing node and next hop */
    ght_position = ght_get_position(c, request->metadata);
    n_hop = ght_next_hop(c, ght_position);    

    /* if I am the storing node */
    if (n_hop == NULL) {
        /* get next hop to home node */
        struct ght_neighbor *p_hop = ght_next_hop(c, &(request->position));
        
        /* check wether we have the data */
        if (request->d_seq > nodedata->d_seq[request->metadata]) {
            packet_dealloc(packet);
            return;
        }
        
        if (p_hop == NULL) {
            packet_t *packet0;
            struct sensor_data_p *data;
            destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};

            /* reply */
            packet0 = packet_create(c, nodedata->overhead + sizeof(struct sensor_data_p), -1);
            data = (struct sensor_data_p *) (packet0->data + nodedata->overhead);
            if (SET_HEADER(&c0, packet0, &dst) == -1) {
                packet_dealloc(packet);
                packet_dealloc(packet0);
                return;
            }
            data->type = SENSOR_DATA_TYPE;
            data->metadata = request->metadata;
            data->sink = request->sink;
            data->r_seq = request->r_seq;
            data->source = nodedata->d_source[request->metadata];
            data->d_seq = nodedata->d_seq[request->metadata];
            data->d_value = nodedata->d_value[request->metadata];
            TX(&c0, packet0);
            packet_dealloc(packet);
            return;
        } else {
            packet_t *packet0;
            struct ght_response_p *response;
            destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
            
            /* reply */
            packet0 = packet_create(c, nodedata->overhead + sizeof(struct ght_response_p), -1);
            response = (struct ght_response_p *) (packet0->data + nodedata->overhead);
            if (SET_HEADER(&c0, packet0, &dst) == -1) {
                packet_dealloc(packet);
                packet_dealloc(packet0);
                return;
            }
            response->type = GHT_RESPONSE_TYPE;
            response->n_hop = p_hop->id;
            response->metadata = request->metadata;
            response->sink = request->sink;
            response->r_seq = request->r_seq;
            response->source = nodedata->d_source[request->metadata];
            response->d_seq = nodedata->d_seq[request->metadata];
            response->d_value = nodedata->d_value[request->metadata];
            response->position.x = request->position.x;
            response->position.y = request->position.y;
            response->position.z = request->position.z;
            TX(&c0, packet0);
            packet_dealloc(packet);
            return;
        }
    } else {
        /* forward packet */
        destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
        
        /* forward data */
        if (SET_HEADER(&c0, packet, &dst) == -1) {
            packet_dealloc(packet);
            return;
        }
        request->n_hop = n_hop->id;
        TX(&c0, packet);    
    }

    return;
}


/**************************************************************************/
/**************************************************************************/
void rx_ght_response(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct ght_response_p *response = (struct ght_response_p *) (packet->data + nodedata->overhead);
    struct ght_neighbor *n_hop;
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

    /* are we the next hop */
    if (response->n_hop != c->node) {
        packet_dealloc(packet);
        return;
    }

    /* get next hop */
    n_hop = ght_next_hop(c, &(response->position));
    
    /* if I am the home node */
    if (n_hop == NULL) {
        packet_t *packet0;
        struct sensor_data_p *data;
        destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
        
        /* reply */
        packet0 = packet_create(c, nodedata->overhead + sizeof(struct sensor_data_p), -1);
        data = (struct sensor_data_p *) (packet0->data + nodedata->overhead);
        if (SET_HEADER(&c0, packet0, &dst) == -1) {
            packet_dealloc(packet);
            packet_dealloc(packet0);
            return;
        }
        data->type = SENSOR_DATA_TYPE;
        data->metadata = response->metadata;
        data->sink = response->sink;
        data->r_seq = response->r_seq;
        data->source = response->source;
        data->d_seq = response->d_seq;
        data->d_value = response->d_value;
        TX(&c0, packet0);
        packet_dealloc(packet);
        return;
    } else {
        /* forward packet */
        destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
        
        /* forward data */
        if (SET_HEADER(&c0, packet, &dst) == -1) {
            packet_dealloc(packet);
            return;
        }
        response->n_hop = n_hop->id;
        TX(&c0, packet);    
    }

    return;
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
   struct nodedata *nodedata = get_node_private_data(c);
   struct data_d_header *header = (struct data_d_header *) (packet->data + nodedata->overhead);
    
   switch (header->type) {       
   case GHT_HELLO_TYPE:
        /* for us */
       rx_ght_hello(c, packet);
       break;       
   case GHT_DATA_TYPE:
        /* for us */
       rx_ght_data(c, packet);
       break;       
   case GHT_REQUEST_TYPE:
        /* for us */
       rx_ght_request(c, packet);
       break;       
   case GHT_RESPONSE_TYPE:
        /* for us */
       rx_ght_response(c, packet);
       break;       
    case SOURCE_ADV_TYPE:
        /* for us */
        rx_source_adv(c, packet);
        break;
   case SOURCE_DATA_TYPE:
        /* for us */
        rx_source_data(c, packet);
        break;
   case SINK_ADV_TYPE:
        /* for us */
       rx_sink_adv(c, packet);
       break;
   default:
       /* not for us */
       packet_dealloc(packet);
       break;
   }
   
    return;
}



/* ************************************************** */
/* ************************************************** */
application_methods_t methods = {rx};

