/**
 *  \file   geostatic.c
 *  \brief  Static geographic routing
 *  \author Romain Kuntz
 *  \date   01/2010
 **/

#include <stdio.h>
#include <include/modelutils.h>

/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Static geographic routing",
    "Romain Kuntz",
    "0.1",
    MODELTYPE_ROUTING, 
    {NULL, 0}
};

struct routing_header {
    nodeid_t dst;
    nodeid_t src;
    int hop;
};

struct neighbor {
    nodeid_t id;
    position_t position;
    uint64_t time;
};

struct nodedata {
    int overhead;
    int hop;       
    int random_nexthop;
    int random_counter;
    double range;

    void *neighbors;    
    nodeid_t curr_dst;
    struct neighbor* curr_nexthop;
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
/* Find all the neighbors (i.e. nodes in range) of the current node */
int find_neighbors(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct neighbor *neighbor = NULL;    
    nodeid_t i;
    double dist = 0;
    int nb_neigh = 0;

    /* Parse all the nodes in the simulation: and find the ones 
     * that are in range of that node
     */
    for(i = 0; i < get_node_count(); i++) {
        /* Do not include myself */
        if (i == c->node) {
            continue;
        }

        dist = distance(get_node_position(c->node), get_node_position(i));
        if (dist <= nodedata->range) {
            /* Add the node in the list of neighbors */
            neighbor = (struct neighbor *) malloc(sizeof(struct neighbor));
            neighbor->id = i;
            neighbor->position.x = get_node_position(i)->x;
            neighbor->position.y = get_node_position(i)->y;
            neighbor->position.z = get_node_position(i)->z;
            neighbor->time = get_time();
            //PRINT_ROUTING("Node %d is added to neighbor list of node %d\n",
            //              neighbor->id, c->node);    
            das_insert(nodedata->neighbors, (void *) neighbor);
            nb_neigh++;
        }
    }

    return nb_neigh;
}

/* Get the best next hop for a specific destination */
struct neighbor* get_nexthop(call_t *c, nodeid_t dst) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct neighbor *neighbor = NULL, *n_hop = NULL;
    double dist = distance(get_node_position(c->node), get_node_position(dst));
    double d = dist;

    if (nodedata->curr_dst != dst 
            || (nodedata->curr_nexthop != NULL 
                && !is_node_alive(nodedata->curr_nexthop->id))) {
        /* If the destination is different from the last one,
         * or if the current next hop is dead, reinit the 
         * random counters (to force the selection of a 
         * new next_hop) */
        nodedata->random_counter = nodedata->random_nexthop;
    }

    if (nodedata->random_nexthop == 0) {
        /* We keep the current next hop if 
         * - next hop is not randomized
         * - the next hop is is still alive
         * - the destination is the same 
         */
        if (nodedata->curr_nexthop != NULL
                && nodedata->curr_dst == dst
                && is_node_alive(nodedata->curr_nexthop->id)) {
            return nodedata->curr_nexthop;
        }
        
        /* Parse neighbors */
        das_init_traverse(nodedata->neighbors);    
        while ((neighbor = (struct neighbor *) 
                            das_traverse(nodedata->neighbors)) != NULL) {        
            /* Choose next hop (the one the nearest from the final dst) 
             * and verify if it is still alive */
            if ((d = distance(&(neighbor->position), get_node_position(dst))) < dist
                    && is_node_alive(neighbor->id)) {
                dist = d;
                n_hop = neighbor;
            }
        }
    } else if (nodedata->random_counter == nodedata->random_nexthop) {
        void *next_hops = das_create();
        int nh = 0;
        double nexthop_dst = 0;

        /* Random geographic routing : we choose randomly among 
         * the neighbors that are nearer from the destination 
         * than the current node.
         */
        das_init_traverse(nodedata->neighbors);    
        while ((neighbor = (struct neighbor *) 
                            das_traverse(nodedata->neighbors)) != NULL) {        
            /* If the neighbor happens to be the final destination, 
             * then we just choose it as the next hop */
            if (neighbor->id == dst) {
                n_hop = neighbor;
                goto out;
            }

            /* Store the neighbors that are nearer from the destination 
             * and that are still alive */
            nexthop_dst = distance(&(neighbor->position), get_node_position(dst));
            if (nexthop_dst < dist && is_node_alive(neighbor->id)) {
                das_insert(next_hops, (void *) neighbor);
            }
        }
        /* Choose next hop randomly among the list */
        nh = das_getsize(next_hops);
        if (nh > 0) {
            int rnd = get_random_integer_range(1, nh);
            while (rnd--) {
                neighbor = das_pop(next_hops);
            }
            n_hop = neighbor;
        }
        das_destroy(next_hops);
    } else /* nodedata->random_counter != nodedata->random_nexthop */ {
        /* Keep the current next hop */
        n_hop = nodedata->curr_nexthop;
    }

out:
    nodedata->random_counter--;
    if (nodedata->random_counter <= 0) {
        nodedata->random_counter = nodedata->random_nexthop;    
    }

    /* Save the current next hop and destination */
    nodedata->curr_nexthop = n_hop;
    nodedata->curr_dst = dst;
    return n_hop;
}

/* ************************************************** */
/* ************************************************** */
int setnode(call_t *c, void *params) {
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    param_t *param;

    /* Find all the neighbors for this node */
    nodedata->neighbors = das_create();    
    nodedata->curr_dst = -1;
    nodedata->curr_nexthop = NULL;

    /* default values */
    nodedata->overhead = -1;
    nodedata->hop = 32;
    nodedata->range = 1;
    nodedata->random_nexthop = 0;
 
    /* Get parameters from config file */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        /* Hop-limit */
        if (!strcmp(param->key, "hop")) {
            if (get_param_integer(param->value, &(nodedata->hop))) {
                goto error;
            }
        }
        /* Range in which neighbors are selected */
        if (!strcmp(param->key, "range")) {
            if (get_param_double(param->value, &(nodedata->range))) {
                goto error;
            }
        }
        /* Randomize the choice of the next hop. 0 means never (always 
         * take the nearest one from the destination), and a value >= 1 
         * randomizes the next hop every "value" time
         */
        if (!strcmp(param->key, "random")) {
            if (get_param_integer(param->value, &(nodedata->random_nexthop))) {
                goto error;
            }
        }
    }
    nodedata->random_counter = nodedata->random_nexthop;
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
    int nb_neigh = 0;
    
    /* Get mac header overhead */
    nodedata->overhead = GET_HEADER_SIZE(&c0);

    /* Find all the node's neighbors (i.e. the one in range) */
    nb_neigh = find_neighbors(c);
    PRINT_ROUTING("Node %d has %d neighbors\n", c->node, nb_neigh);
        
    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}

/* ************************************************** */
/* ************************************************** */
int set_header(call_t *c, packet_t *packet, destination_t *dst) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct neighbor *n_hop = get_nexthop(c, dst->id);
    destination_t destination;
    struct routing_header *header = (struct routing_header *) 
                                            (packet->data + nodedata->overhead);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

    /* No route available */
    if (dst->id != BROADCAST_ADDR && n_hop == NULL) {
        return -1;
    } 

    /* Set routing header */
    header->dst = dst->id;
    header->src = c->node;
    header->hop = nodedata->hop;

    /* Set MAC header */
    if (dst->id == BROADCAST_ADDR) {
        destination.id = BROADCAST_ADDR;
    } else {
        destination.id = n_hop->id;        
    }
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
void tx(call_t *c, packet_t *packet) {
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    TX(&c0, packet);
}

/* ************************************************** */
/* ************************************************** */
void forward(call_t *c, packet_t *packet) {  
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    struct routing_header *header = (struct routing_header *) 
        (packet->data + nodedata->overhead);
    struct neighbor *n_hop = get_nexthop(c, header->dst);
    destination_t destination;    

    /* No route available */    
    if (n_hop == NULL) {
        packet_dealloc(packet);        
        return;
    }
    
    /* Update hop count */
    header->hop--;

    /* Hop count reached */
    if (header->hop == 0) {
        packet_dealloc(packet);
        return;
    }

    /* Set MAC header */
    destination.id = n_hop->id;
    destination.position.x = -1;
    destination.position.y = -1;
    destination.position.z = -1;
    if (SET_HEADER(&c0, packet, &destination) == -1) {
        packet_dealloc(packet);
        return;
    }
    
    /* Forwarding packet */
    PRINT_ROUTING("forward: Node %d forwards a packet "
                  "(from %d to %d, hop limit %d)\n",
                   c->node, header->src, header->dst, 
                   header->hop);
    TX(&c0, packet);
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    array_t *up = get_entity_bindings_up(c);
    int i = up->size;
    struct routing_header *header = (struct routing_header *) 
        (packet->data + nodedata->overhead);

    /* Forward packet if node is not the recipient */
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

    return;
}

/* ************************************************** */
/* ************************************************** */
routing_methods_t methods = {rx, 
                             tx, 
                             set_header, 
                             get_header_size,
                             get_header_real_size};
