/**
 *  \file   idealmac.c
 *  \brief  IDEAL MAC protocol with a DISK propagation model
 *  \author Elyes Ben Hamida and Guillaume Chelius and Damien Roth
 *  \date   2008
 **/
#include <stdio.h>
#include <include/modelutils.h>
#include <include/entity.h>

/* ************************************************** */
/* ************************************************** */

/* Please uncomment the following line to allow the transmission
   of only one data packet at the same time. */

//#define ONE_PACKET_AT_A_TIME


/* ************************************************** */
/* ************************************************** */
#define UNICAST_TYPE	    0
#define BROADCAST_TYPE      1
#define ONE_MS              1000000


/* ************************************************** */
/* ************************************************** */
struct _mac_header {
    int src;
    int dst;
    int type;
};


/* ************************************************** */
/* ************************************************** */
struct entitydata {
    double range;        // Communication range (m)
    double bandwidth;    // Data bandwidth (KB/s)
};


/* ************************************************** */
/* ************************************************** */
struct nodedata {
    void *buffer;
#ifdef ONE_PACKET_AT_A_TIME
    int scheduler;
#endif
};


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "IDEAL MAC module",
    "Elyes Ben Hamida and Guillaume Chelius and Damien Roth",
    "0.1",
    MODELTYPE_MAC, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;

    /* default values */
    entitydata->range     = 10;
    entitydata->bandwidth = 15; 

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "range")) {
            if (get_param_double(param->value, &(entitydata->range))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "bandwidth")) {
            if (get_param_double(param->value, &(entitydata->bandwidth))) {
                goto error;
            }
        }
    }

    // KB/s to B/s
    entitydata->bandwidth *= 1024;

    set_entity_private_data(c, entitydata);
    return 0;

 error:
    free(entitydata);
    return -1;
}

int destroy(call_t *c) {
    free(get_entity_private_data(c));
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int setnode(call_t *c, void *params) {
    struct nodedata *nodedata = (struct nodedata *) malloc(sizeof(struct nodedata));

    nodedata->buffer    = das_create();

#ifdef ONE_PACKET_AT_A_TIME
    nodedata->scheduler = 0;
#endif

    set_node_private_data(c, nodedata);
    return 0;
}

int unsetnode(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);

    packet_t *packet;
    while ((packet = (packet_t *) das_pop(nodedata->buffer)) != NULL)
        packet_dealloc(packet);

    das_destroy(nodedata->buffer);

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
int tx_delay(call_t *c, void *args) {
    struct entitydata *entitydata = get_entity_private_data(c);
    struct nodedata *nodedata = get_node_private_data(c);
    position_t *local = get_node_position(c->node);
    int i = 0;
    
    packet_t *packet;
    if ((packet = (packet_t *) das_pop_FIFO(nodedata->buffer)) == NULL)
      return 0;
    
    struct _mac_header *header = (struct _mac_header *) packet->data;

    /* Broadcast packet */
    if (header->type == BROADCAST_TYPE) {
#ifdef LOG_MAC
      fprintf(stdout, "[MAC] node %d wants to broadcast a packet\n", c->node);
#endif
      /* sending the packet to nodes within reach communication */
      for(i = 0; i < get_node_count(); i++) {
	
	if (i != c->node) {
	  
                if (distance(get_node_position(i), local) <= entitydata->range) {
		  call_t c0 = {-1, i, -1};
		  array_t *macs = get_mac_entities(&c0);
		  c0.entity = macs->elts[0];
		  packet_t *packet_up = packet_clone(packet);         
		  RX(&c0, packet_up);
#ifdef LOG_MAC
		  fprintf(stdout, "[MAC] node %d delivers a packet to node %d\n", c->node, i);
#endif
                }
	}       
      }
      packet_dealloc(packet);
    } 
    /* Unicast packet */
    else if (header->type == UNICAST_TYPE){
      
      if (distance(get_node_position(header->dst), local) <= entitydata->range) {
	call_t c0 = {-1, header->dst, -1};
	array_t *macs = get_mac_entities(&c0);
	c0.entity = macs->elts[0];
	RX(&c0, packet);
#ifdef LOG_MAC
	fprintf(stdout,"[MAC] node %d delivers a packet to node %d\n", c->node, header->dst);
#endif
      }
      else {
#ifdef LOG_MAC
	fprintf(stdout,"[MAC] node %d: destination node %d is unreachable !!!\n",c->node, header->dst);
#endif
	packet_dealloc(packet);
      }
    }
    else {
#ifdef LOG_MAC
      fprintf(stderr,"[MAC] Unknown packet type (%d) !!!\n", header->type);
#endif
      packet_dealloc(packet);
    }

    
#ifdef ONE_PACKET_AT_A_TIME
    if (das_getsize(nodedata->buffer) == 0) {
      nodedata->scheduler = 0;
    }
    else {
      das_init_traverse(nodedata->buffer);
      if ((packet = (packet_t *) das_traverse(nodedata->buffer)) == NULL)
	return 0;
      
      uint64_t delay = get_time() + (packet->size / entitydata->bandwidth) * ONE_MS;
      scheduler_add_callback(delay, c, tx_delay, NULL);
    }
#endif

    return 0;
}


/* ************************************************** */
/* ************************************************** */

void tx(call_t *c, packet_t *packet) {
    struct entitydata *entitydata = get_entity_private_data(c);
    struct nodedata *nodedata = get_node_private_data(c);

    das_insert(nodedata->buffer, (void *) packet);
    int duration = packet->size / entitydata->bandwidth * ONE_MS;

#ifdef ONE_PACKET_AT_A_TIME
   if (nodedata->scheduler == 0) {
        nodedata->scheduler = 1;
        uint64_t delay = get_time() + duration;
        scheduler_add_callback(delay, c, tx_delay, NULL);
    }
#else
   uint64_t delay = get_time() + duration;
   scheduler_add_callback(delay, c, tx_delay, NULL);   
#endif
}

/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    struct _mac_header *header = (struct _mac_header *) packet->data;
    array_t *up = get_entity_bindings_up(c);
    int i = up->size;

    if (header->type == UNICAST_TYPE && header->dst != c->node) {
        /* Packet not for us */
        packet_dealloc(packet);
        return;
    }

    /* forward packet to upper layer */
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
  
}


/* ************************************************** */
/* ************************************************** */
int set_header(call_t *c, packet_t *packet, destination_t *dst) {
    struct _mac_header *header = (struct _mac_header *) packet->data;

    if ((header->dst = dst->id) == BROADCAST_ADDR) {
        header->type =BROADCAST_TYPE;
    } else {
        header->type = UNICAST_TYPE;
    }
    header->src = c->node;
    
    return 0;
}

int get_header_size(call_t *c) {
    return (sizeof(struct _mac_header));
}

int get_header_real_size(call_t *c) {
    return (sizeof(struct _mac_header));
}


/* ************************************************** */
/* ************************************************** */
mac_methods_t methods = {rx, 
                         tx,
                         set_header, 
                         get_header_size,
                         get_header_real_size};


    
    
