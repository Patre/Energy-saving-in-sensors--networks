/**
 *  \file   greedy.c
 *  \brief  Greedy geographic routing
 *  \author Elyes Ben Hamida and Guillaume Chelius
 *  \date   2007
 **/
#include <stdio.h>

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Greedy geographic routing",
    "Elyes Ben Hamida and Guillaume Chelius",
    "0.1",
    MODELTYPE_ROUTING, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
#define HELLO_PACKET 0
#define DATA_PACKET  1


/* ************************************************** */
/* ************************************************** */
struct routing_header {
    nodeid_t dst;
    position_t dst_pos;
    nodeid_t src;
    position_t src_pos;
    int hop;
    int type;
};

struct neighbor {
    int id;
    position_t position;
    uint64_t time;
};

struct nodedata {
    void *neighbors;
    int overhead;

    uint64_t start;
    uint64_t period;
    uint64_t timeout;
    int hop;       

    /* stats */
    int hello_tx;
    int hello_rx;
    int data_tx;
    int data_rx;
    int data_noroute;
    int data_hop;
};


/* ************************************************** */
/* ************************************************** */
int advert_callback(call_t *c, void *args);


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
    nodedata->neighbors = das_create();
    nodedata->overhead = -1;
    nodedata->hello_tx = 0;
    nodedata->hello_rx = 0;
    nodedata->data_tx = 0;
    nodedata->data_rx = 0;
    nodedata->data_noroute = 0;
    nodedata->data_hop = 0;
    nodedata->start = 0;
    nodedata->hop = 32;
    nodedata->period = 1000000000;
    nodedata->timeout = 2500000000ull;
 
    /* get params */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "start")) {
            if (get_param_time(param->value, &(nodedata->start))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "period")) {
            if (get_param_time(param->value, &(nodedata->period))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "hop")) {
            if (get_param_integer(param->value, &(nodedata->hop))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "timeout")) {
            if (get_param_time(param->value, &(nodedata->timeout))) {
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
    struct neighbor *neighbor;
    while ((neighbor = (struct neighbor *) das_pop(nodedata->neighbors)) != NULL) {
        free(neighbor);
    }
    das_destroy(nodedata->neighbors);    
    free(nodedata);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    
    /* get mac header overhead */
    nodedata->overhead = GET_HEADER_SIZE(&c0);
        
    /*  hello packet */
    if (nodedata->period > 0) {
        uint64_t start = get_time() + nodedata->start + get_random_double() * nodedata->period;
        scheduler_add_callback(start, c, advert_callback, NULL);
    }

    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
struct neighbor* get_nexthop(call_t *c, position_t *dst) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct neighbor *neighbor = NULL, *n_hop = NULL;
    uint64_t clock = get_time();
    double dist = distance(get_node_position(c->node), dst);
    double d = dist;

    /* parse neighbors */
    das_init_traverse(nodedata->neighbors);    
    while ((neighbor = (struct neighbor *) das_traverse(nodedata->neighbors)) != NULL) {        
        if ((nodedata->timeout > 0)
            && (clock - neighbor->time) >= nodedata->timeout ) {
            continue;
        }
        
        /* choose next hop */
        if ((d = distance(&(neighbor->position), dst)) < dist) {
            dist = d;
            n_hop = neighbor;
        }
    }    
    
    return n_hop;
}

void add_neighbor(call_t *c, struct routing_header *header) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct neighbor *neighbor = NULL;

    /* check wether neighbor already exists */
    das_init_traverse(nodedata->neighbors);      
    while ((neighbor = (struct neighbor *) das_traverse(nodedata->neighbors)) != NULL) {      
        if (neighbor->id == header->src) {
            neighbor->position.x = header->src_pos.x;
            neighbor->position.y = header->src_pos.y;
            neighbor->position.z = header->src_pos.z;
            neighbor->time = get_time();
            return;
        }
    }  

    neighbor = (struct neighbor *) malloc(sizeof(struct neighbor));
    neighbor->id = header->src;
    neighbor->position.x = header->src_pos.x;
    neighbor->position.y = header->src_pos.y;
    neighbor->position.z = header->src_pos.z;
    neighbor->time = get_time();
    das_insert(nodedata->neighbors, (void *) neighbor);
    return;
}


/* ************************************************** */
/* ************************************************** */
int set_header(call_t *c, packet_t *packet, destination_t *dst) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct neighbor *n_hop = get_nexthop(c, &(dst->position));
    destination_t destination;
    struct routing_header *header = (struct routing_header *) (packet->data + nodedata->overhead);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

    /* if no route, return -1 */
    if (dst->id != BROADCAST_ADDR && n_hop == NULL) {
        nodedata->data_noroute++;
        return -1;
    }
    else if (dst->id == BROADCAST_ADDR) {
      n_hop->id = BROADCAST_ADDR;
    }

    /* set routing header */
    header->dst = dst->id;
    header->dst_pos.x = dst->position.x;
    header->dst_pos.y = dst->position.y;
    header->dst_pos.z = dst->position.z;
    header->src = c->node;
    header->src_pos.x = get_node_position(c->node)->x;
    header->src_pos.y = get_node_position(c->node)->y;
    header->src_pos.z = get_node_position(c->node)->z;
    header->type = DATA_PACKET;
    header->hop = nodedata->hop;

    /* Set mac header */
    destination.id = n_hop->id;
    destination.position.x = -1;
    destination.position.y = -1;
    destination.position.z = -1;
    return SET_HEADER(&c0, packet, &destination);
}

int get_header_size(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);

    if (nodedata->overhead == -1) {
        call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};        
        nodedata->overhead = GET_HEADER_SIZE(&c0);
    }
    
    return nodedata->overhead + sizeof(struct routing_header);
}

int get_header_real_size(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);

    if (nodedata->overhead == -1) {
        call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};        
        nodedata->overhead = GET_HEADER_REAL_SIZE(&c0);
    }
    
    return nodedata->overhead + sizeof(struct routing_header);
}


/* ************************************************** */
/* ************************************************** */
int neighbor_timeout(void *data, void *arg) {
    struct neighbor *neighbor = (struct neighbor *) data;
    call_t *c = (call_t *) arg;
    struct nodedata *nodedata = get_node_private_data(c);
    if ((get_time() - neighbor->time) >= nodedata->timeout) {
        return 1;
    }
    return 0;
}

int advert_callback(call_t *c, void *args) {
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};
    packet_t *packet = packet_create(c, nodedata->overhead + sizeof(struct routing_header), -1);
    struct routing_header *header = (struct routing_header*) (packet->data + nodedata->overhead);
        
    /* set mac header */
    if (SET_HEADER(&c0, packet, &destination) == -1) {
        packet_dealloc(packet);
        return -1;
    }

    /* set routing header */
    header->dst = BROADCAST_ADDR;
    header->dst_pos.x = -1;
    header->dst_pos.y = -1;
    header->dst_pos.z = -1;
    header->src = c->node;
    header->src_pos.x = get_node_position(c->node)->x;
    header->src_pos.y = get_node_position(c->node)->y;
    header->src_pos.z = get_node_position(c->node)->z;
    header->type = HELLO_PACKET;
    header->hop = 1;
    
    /* send hello */
    TX(&c0, packet);
    nodedata->hello_tx++;

    /* check neighbors timeout  */
    das_selective_delete(nodedata->neighbors, neighbor_timeout, (void *) c);

    /* schedules hello */
    scheduler_add_callback(get_time() + nodedata->period, c, advert_callback, NULL);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void tx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    
    nodedata->data_tx++;
    TX(&c0, packet);
}


/* ************************************************** */
/* ************************************************** */
void forward(call_t *c, packet_t *packet) {  
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    struct routing_header *header = (struct routing_header *) (packet->data + nodedata->overhead);
    struct neighbor *n_hop = get_nexthop(c, &(header->dst_pos));
    destination_t destination;    

    /* delivers packet to application layer */
    if (n_hop == NULL) {
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
    
    /* update hop count */
    header->hop--;
    if (header->hop == 0) {
        nodedata->data_hop++;
        packet_dealloc(packet);
        return;
    }
    

    /* set mac header */
    destination.id = n_hop->id;
    destination.position.x = -1;
    destination.position.y = -1;
    destination.position.z = -1;
    if (SET_HEADER(&c0, packet, &destination) == -1) {
        packet_dealloc(packet);
        return;
    }
    
    /* forwarding packet */
    nodedata->data_tx++;
    TX(&c0, packet);
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    array_t *up = get_entity_bindings_up(c);
    int i = up->size;
    struct routing_header *header = (struct routing_header *) (packet->data + nodedata->overhead);

    switch(header->type) {
    case HELLO_PACKET:         
        nodedata->hello_rx++;
        add_neighbor(c, header);
        packet_dealloc(packet);
        break;

    case DATA_PACKET : 
        nodedata->data_rx++;

	if ((header->dst != BROADCAST_ADDR) && (header->dst != c->node) ) {
            forward(c, packet);
            return;
        }
        
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
        break;

    default : 
        break;       
    }

    return;
}


/* ************************************************** */
/* ************************************************** */
routing_methods_t methods = {rx, 
                             tx, 
                             set_header, 
                             get_header_size,
                             get_header_real_size};
