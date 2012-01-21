/**
 *  \file   skeleton.c
 *  \brief  MAC layer skeleton
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include <stdio.h>

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
#define SIFS 1000000

/* ************************************************** */
/* ************************************************** */
#define STATE_IDLE  1
#define STATE_TXING 2


/* ************************************************** */
/* ************************************************** */
struct nodedata {
    uint64_t clock;
    int state;

    void *packets;
};


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Code skeleton for MAC protocol",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_MAC, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct _skeleton_header {
    nodeid_t dst;
    nodeid_t src;
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

    /* init fsm */
    nodedata->clock = 0;
    nodedata->state = STATE_IDLE;

    /* init packet fifo */
    nodedata->packets = das_create();

    /* set node private data */
    set_node_private_data(c, nodedata);
    return 0;
}

int unsetnode(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    packet_t *packet;

    /* clean pending packets in fifo */
    while ((packet = (packet_t *) das_pop(nodedata->packets)) != NULL) {
        packet_dealloc(packet);
    }

    /* clean fifo */
    das_destroy(nodedata->packets);    

    /* clean private data*/
    free(nodedata);

    return 0;
}


/* ************************************************** */
/* ************************************************** */
int state_machine(call_t *c, void *args) { 
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    uint64_t timeout;
    packet_t *packet;	

    if (nodedata->clock != get_time()) {
        return 0;
    }

    switch (nodedata->state) {
		
    case STATE_IDLE:
        /* get next packet to send */
        packet = (packet_t *) das_pop_FIFO(nodedata->packets);
        timeout = packet->size * 8 * radio_get_Tb(&c0) + SIFS;

        /* send packet */
        TX(&c0, packet); 

        /* change state */
        nodedata->state = STATE_TXING;

        /* wait for end of transmission */
        nodedata->clock = get_time() + timeout;
        scheduler_add_callback(nodedata->clock, c, state_machine, NULL);

    case STATE_TXING:
        nodedata->state = STATE_IDLE;
        nodedata->clock = get_time();
        state_machine(c,NULL);
        return 0;
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
int set_header(call_t *c, packet_t *packet, destination_t *dst) {
    struct _skeleton_header *header = (struct _skeleton_header *) packet->data;

    /* fill mac header */
    header->dst = dst->id;
    header->src = c->node;
    return 0;
}

int get_header_size(call_t *c) {
    return (sizeof(struct _skeleton_header));
}

int get_header_real_size(call_t *c) {
    return (sizeof(struct _skeleton_header));
}


/* ************************************************** */
/* ************************************************** */
void tx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    
    /* add packet in fifo */
    das_insert(nodedata->packets, (void *) packet);

    /* if not currently sending, send packet */
    if (nodedata->state == STATE_IDLE) {
        nodedata->clock = get_time();  
        state_machine(c,NULL);
    } 
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    struct _skeleton_header *header = (struct _skeleton_header *) packet->data;
    array_t *up = get_entity_bindings_up(c);
    int i = up->size;

    if ((header->dst != c->node) && (header->dst != BROADCAST_ADDR)) {
        packet_dealloc(packet);
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
mac_methods_t methods = {rx, 
                         tx,
                         set_header, 
                         get_header_size,
                         get_header_real_size};


    
    
