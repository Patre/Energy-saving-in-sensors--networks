/**
 *  \file   dcf_802_11.c
 *  \brief  MAC 802.11 DCF protocol
 *  \author Guillaume Chelius and Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdio.h>

#include <include/modelutils.h>


/**
 * TODO: 
 *  - réajuster les timeout pour prendre valeurs de 802.11, taille de paquet... notamment temps de transmission, cca threshold...
 *  - vérifier le backoff pour le difs au début ou direct backoff
 *  - tenter d'avoir différentes longueurs de paqgérer le multi-débit ????
 **/
#define SPEED_LIGHT 3000


/* ************************************************** */
/* ************************************************** */
#define STATE_IDLE			1
#define STATE_BACKOFF		2
#define STATE_RTS			3
#define STATE_TIMEOUT		4
#define STATE_CTS		    5
#define STATE_CTS_TIMEOUT	6
#define STATE_DATA			7
#define STATE_BROADCAST		8
#define STATE_ACK			9
#define STATE_DONE			10
#define STATE_BROAD_DONE	11


/* ************************************************** */
/* ************************************************** */
#define macMinDIFSPeriod      50000   
#define macMinSIFSPeriod      10000
#define macMinBE              5     /* 32 slots */
#define macMaxBE              10    /* 1024 slots */
#define macMaxCSMARetries     7     /* 7 trials before dropping */
#define aUnitBackoffPeriod    20000
#define EDThresholdMin        -74


/* ************************************************** */
/* ************************************************** */
#define RTS_TYPE			1
#define CTS_TYPE			2
#define DATA_TYPE			3
#define ACK_TYPE			4
#define BROADCAST_TYPE      5


/* ************************************************** */
/* ************************************************** */
struct _dcf_802_11_header {
    int src;
    int dst;
    int type;
};
struct _dcf_802_11_rts_header {
    uint64_t nav;
    int size;
    char padding[8];
};
struct _dcf_802_11_cts_header {
    uint64_t nav;
    char padding[6];
};
struct _dcf_802_11_data_header {
    uint64_t nav;
    int size;
    char padding[22];
};
struct _dcf_802_11_ack_header {
    char padding[14];
};


/* ************************************************** */
/* ************************************************** */
struct nodedata {
    uint64_t clock;
    int state;
    int state_pending;
    int dst;
    int size;

    uint64_t backoff;
    int backoff_suspended;
    int NB;
    int BE;

    uint64_t nav;
    int rts_threshold;

    void *packets;
    packet_t *txbuf;

    int cs;
    int cca;
    double EDThreshold;
;
};

struct entitydata {
    int maxCSMARetries;
};


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "802.11 DCF mac module",
    "Guillaume Chelius and Elyes Ben Hamida",
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
    entitydata->maxCSMARetries = macMaxCSMARetries;

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "retry")) {
            if (get_param_integer(param->value, &(entitydata->maxCSMARetries))) {
                goto error;
            }
        }
    }

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
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    param_t *param;
    
    /* default values */
    nodedata->rts_threshold = 500;
    nodedata->clock = 0;
    nodedata->state = STATE_IDLE;
    nodedata->state_pending = STATE_IDLE;
    nodedata->nav = 0;
    nodedata->cca = 1;
    nodedata->cs = 1;
    nodedata->EDThreshold = EDThresholdMin;

    /* Init packets buffer */
    nodedata->packets = das_create();
    nodedata->txbuf = NULL;

    /* get params */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
       if (!strcmp(param->key, "cca-threshold")) {
            if (get_param_double(param->value, &(nodedata->EDThreshold))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "cca")) {
            if (get_param_integer(param->value, &(nodedata->cca))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "cs")) {
            if (get_param_integer(param->value, &(nodedata->cs))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "rts-threshold")) {
            if (get_param_integer(param->value, &(nodedata->rts_threshold))) {
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
    packet_t *packet;
    while ((packet = (packet_t *) das_pop(nodedata->packets)) != NULL) {
        packet_dealloc(packet);
    }
    das_destroy(nodedata->packets);    
    if (nodedata->txbuf) {
        packet_dealloc(nodedata->txbuf);
    }
    free(nodedata);
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
int check_channel_busy(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

    if (nodedata->cs && (radio_get_cs(&c0) >= nodedata->EDThreshold)) {
        return 1;
    }
    if (nodedata->cca && (radio_get_noise(&c0) >= nodedata->EDThreshold)) {
        return 1;
    }
    return 0;
}

int dcf_802_11_state_machine(call_t *c, void *args) { 
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata = get_entity_private_data(c);
    packet_t *packet;	
    struct _dcf_802_11_header *header;
    struct _dcf_802_11_rts_header *rts_header;
    struct _dcf_802_11_cts_header *cts_header;
    struct _dcf_802_11_data_header *data_header;
    struct _dcf_802_11_ack_header *ack_header;
    uint64_t timeout;
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    
    /* Drop unscheduled events */
    if (nodedata->clock != get_time()) {
        return 0;
    }
    
    /* State machine */
    switch (nodedata->state) {
		
    case STATE_IDLE:
        /* Next packet to send */
        if (nodedata->txbuf == NULL) {
            nodedata->txbuf = (packet_t *) das_pop_FIFO(nodedata->packets);
            if (nodedata->txbuf == NULL) {
                return 0;
            }
        }
        
        /* Initial backoff */
        nodedata->state = STATE_BACKOFF;
        nodedata->state_pending = STATE_BACKOFF;				        
        nodedata->BE = macMinBE - 1;
        nodedata->NB = 0;
        nodedata->backoff = macMinDIFSPeriod;
        nodedata->backoff_suspended = 0;
			
        /* Backoff */
        nodedata->clock = get_time();  
        dcf_802_11_state_machine(c,NULL);
        return 0;			
        
    case STATE_BACKOFF:
        /* If the backoff is over, set to 0 */
        if ((nodedata->backoff > 0) && (nodedata->backoff < aUnitBackoffPeriod)) {
            nodedata->backoff = 0;
            nodedata->clock = get_time();
            dcf_802_11_state_machine(c,NULL);
            return 0;
        }
        
        /* Backoff */
        if (nodedata->backoff > 0) {
            if ((get_time() < nodedata->nav) || (check_channel_busy(c))) { 
                if (nodedata->backoff_suspended == 0) {
                    /* Suspend backoff and add difs */
                    nodedata->backoff_suspended = 1;
                    nodedata->backoff = nodedata->backoff + macMinDIFSPeriod;
                }			
            } else {
                /* Decrease backoff */
                nodedata->backoff_suspended = 0;
                nodedata->backoff = nodedata->backoff - aUnitBackoffPeriod;
            }
            
            /* Set next event to backoff */
            nodedata->clock = get_time() + aUnitBackoffPeriod;
            scheduler_add_callback(nodedata->clock, c, dcf_802_11_state_machine, NULL);
            return 0;
        }
        
        /* Broadcast or unicast */
        header = (struct _dcf_802_11_header *) nodedata->txbuf->data;
        if (header->dst == BROADCAST_ADDR) {
            nodedata->state = STATE_BROADCAST;
        } else {
            /* RTS or Data */
            if (nodedata->txbuf->size < nodedata->rts_threshold) {
                nodedata->state = STATE_DATA;
            } else {
                nodedata->state = STATE_RTS;
            }
        }
        nodedata->state_pending = STATE_IDLE;
        nodedata->dst = header->dst;
        
        /* Next state */
        nodedata->clock = get_time();  
        dcf_802_11_state_machine(c,NULL);
        return 0;
        
        
    case STATE_RTS:
        /* Build RTS */
        packet = packet_create(c, sizeof(struct _dcf_802_11_header) + sizeof(struct _dcf_802_11_rts_header), -1);
        header = (struct _dcf_802_11_header *) packet->data;
        header->dst = nodedata->dst;
        header->src = c->node;
        header->type = RTS_TYPE; 
        rts_header = (struct _dcf_802_11_rts_header *) (packet->data + sizeof(struct _dcf_802_11_header));
        rts_header->size = nodedata->txbuf->size;
        rts_header->nav = macMinSIFSPeriod + (sizeof(struct _dcf_802_11_header) + sizeof(struct _dcf_802_11_cts_header)) * radio_get_Tb(&c0) * 8 + macMinSIFSPeriod + nodedata->txbuf->size * 8 * radio_get_Tb(&c0) + macMinSIFSPeriod + (sizeof(struct _dcf_802_11_header) + sizeof(struct _dcf_802_11_ack_header)) * 8 * radio_get_Tb(&c0);				
        timeout = (sizeof(struct _dcf_802_11_header) + sizeof(struct _dcf_802_11_rts_header)) * 8 * radio_get_Tb(&c0) + macMinSIFSPeriod + (sizeof(struct _dcf_802_11_header) + sizeof(struct _dcf_802_11_cts_header)) * 8 * radio_get_Tb(&c0) + SPEED_LIGHT; 			
        
        /* Send RTS */
        TX(&c0, packet); 
        
        /* Wait for timeout or CTS */
        nodedata->state = STATE_TIMEOUT;
        nodedata->clock = get_time() + timeout;
        scheduler_add_callback(nodedata->clock, c, dcf_802_11_state_machine, NULL);			
        return 0;
        
        
    case STATE_TIMEOUT:			
        if ((++nodedata->NB) >= entitydata->maxCSMARetries) {
            /* Transmit retry limit reached */
            packet_dealloc(nodedata->txbuf);            
            nodedata->txbuf = NULL;
			
            /* Return to idle */
            nodedata->state = STATE_IDLE;
            nodedata->clock = get_time();
            dcf_802_11_state_machine(c,NULL);
            return 0;
        }
			
        /* Update backoff */
        if ((++nodedata->BE) > macMaxBE) {
            nodedata->BE = macMaxBE;
        }
        nodedata->backoff = get_random_double() 
            * (pow(2, nodedata->BE) - 1) 
            * aUnitBackoffPeriod 
            + macMinDIFSPeriod;
        nodedata->backoff_suspended = 0;
        nodedata->state = STATE_BACKOFF;
        nodedata->state_pending = STATE_BACKOFF;				
        
        /* Backoff */
        nodedata->clock = get_time();  
        dcf_802_11_state_machine(c,NULL);
        return 0;
			
        
    case STATE_CTS:
        /* Build CTS */
        packet = packet_create(c, sizeof(struct _dcf_802_11_header) + sizeof(struct _dcf_802_11_cts_header), -1);
        header= (struct _dcf_802_11_header *) packet->data;
        header->dst = nodedata->dst;
        header->src = c->node;
        header->type = CTS_TYPE; 
        cts_header = (struct _dcf_802_11_cts_header *) (packet->data + sizeof(struct _dcf_802_11_header));
        cts_header->nav = macMinSIFSPeriod + nodedata->size * 8 * radio_get_Tb(&c0) + macMinSIFSPeriod + (sizeof(struct _dcf_802_11_header) + sizeof(struct _dcf_802_11_ack_header)) * 8 * radio_get_Tb(&c0); 						
        timeout = (sizeof(struct _dcf_802_11_header) + sizeof(struct _dcf_802_11_cts_header)) * 8 * radio_get_Tb(&c0) + macMinSIFSPeriod + nodedata->size * 8 * radio_get_Tb(&c0) + SPEED_LIGHT;
        
        /* Send CTS */
        TX(&c0, packet); 
        
        /* Wait for timeout or DATA */
        nodedata->state = STATE_CTS_TIMEOUT;
        nodedata->clock = get_time() + timeout;
        scheduler_add_callback(nodedata->clock, c, dcf_802_11_state_machine, NULL);
        return 0;
        
        
    case STATE_CTS_TIMEOUT:
        /* Return to pending state */
        nodedata->state = nodedata->state_pending;
        if (nodedata->state != STATE_IDLE) {
            header = (struct _dcf_802_11_header *) nodedata->txbuf->data;
            nodedata->dst = header->dst;
        }
        nodedata->clock = get_time();
        dcf_802_11_state_machine(c,NULL);
        return 0;
			
        
    case STATE_DATA:
        /* Build data packet */	
        packet = packet_clone(nodedata->txbuf);
        header = (struct _dcf_802_11_header *) packet->data;
        data_header = (struct _dcf_802_11_data_header *) (packet->data + sizeof(struct _dcf_802_11_header));
        data_header->nav = macMinSIFSPeriod + (sizeof(struct _dcf_802_11_header) + sizeof(struct _dcf_802_11_ack_header)) * 8 * radio_get_Tb(&c0);
        timeout = packet->size * 8 * radio_get_Tb(&c0) + macMinSIFSPeriod + (sizeof(struct _dcf_802_11_header) + sizeof(struct _dcf_802_11_ack_header)) * 8 * radio_get_Tb(&c0) + SPEED_LIGHT;
        
        /* Send data */
        TX(&c0, packet);
        
        /* Wait for timeout or ACK */
        nodedata->state = STATE_TIMEOUT;
        nodedata->clock = get_time() + timeout;
        scheduler_add_callback(nodedata->clock, c, dcf_802_11_state_machine, NULL);
        return 0;
        
    case STATE_BROADCAST:
        /* Build data packet */	
        packet = packet_clone(nodedata->txbuf);
        timeout = packet->size * 8 * radio_get_Tb(&c0) + macMinSIFSPeriod;
        
        /* Send data */
        TX(&c0, packet);
        
        /* Wait for timeout or ACK */
        nodedata->state = STATE_BROAD_DONE;
        nodedata->clock = get_time() + timeout;
        scheduler_add_callback(nodedata->clock, c, dcf_802_11_state_machine, NULL);
        return 0;
        
    case STATE_ACK:
        /* Build ack packet */	
        packet = packet_create(c, sizeof(struct _dcf_802_11_header) + sizeof(struct _dcf_802_11_ack_header), -1);
        header = (struct _dcf_802_11_header *) packet->data;
        header->type = ACK_TYPE; 
        header->src = c->node;
        header->dst = nodedata->dst;
        ack_header = (struct _dcf_802_11_ack_header *) (packet->data + sizeof(struct _dcf_802_11_header));
        timeout =  packet->size * 8 * radio_get_Tb(&c0) + macMinSIFSPeriod;
        
        /* Send ack */
        TX(&c0, packet);
        
        /* Wait for end of transmission */
        nodedata->state = STATE_DONE;
        nodedata->clock = get_time() + timeout;
        scheduler_add_callback(nodedata->clock, c, dcf_802_11_state_machine, NULL);
        return 0;
        
    case STATE_DONE:
        /* Return to pending state */
        nodedata->state = nodedata->state_pending;
        if (nodedata->state != STATE_IDLE) {
            header = (struct _dcf_802_11_header *) nodedata->txbuf->data;
            nodedata->dst = header->dst;
        }
        nodedata->clock = get_time();
        dcf_802_11_state_machine(c,NULL);
        return 0;
        
        
    case STATE_BROAD_DONE:
        /* Destroy txbuf */
        packet_dealloc(nodedata->txbuf);
        nodedata->txbuf = NULL;
			
        /* Back to idle state*/
        nodedata->state = STATE_IDLE;
        nodedata->clock = get_time();			
        dcf_802_11_state_machine(c,NULL);
        return 0;

		
    default:
        return 0;
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
void tx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    
    das_insert(nodedata->packets, (void*)packet);

    if (nodedata->state == STATE_IDLE) {
        nodedata->clock = get_time();  
        dcf_802_11_state_machine(c,NULL);
    } 
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct _dcf_802_11_header *header = (struct _dcf_802_11_header *) packet->data;
    struct _dcf_802_11_rts_header *rts_header;
    struct _dcf_802_11_cts_header *cts_header;
    struct _dcf_802_11_data_header *data_header;
    struct _dcf_802_11_ack_header *ack_header;
    array_t *up = get_entity_bindings_up(c);
    int i = up->size;

    switch (header->type) {
		
    case RTS_TYPE:
        /* Receive RTS*/
        rts_header = (struct _dcf_802_11_rts_header *) (packet->data + sizeof(struct _dcf_802_11_header));
			
        if (header->dst != c->node) {
            /* Packet not for us */
            if (nodedata->nav < (get_time() + rts_header->nav)) {
                nodedata->nav = get_time() + rts_header->nav;
            }
            packet_dealloc(packet);
            return;
        }
        
        if ((nodedata->state != STATE_IDLE) 
            && (nodedata->state != STATE_BACKOFF)) {
            /* If not expecting rts, do nothing */
            packet_dealloc(packet);
            return;
        }
        
        /* Record RTS info */
        nodedata->dst = header->src;
        nodedata->size = rts_header->size;
        packet_dealloc(packet);
			
        /* Send CTS */
        if (nodedata->state == STATE_BACKOFF) {
            nodedata->state_pending = nodedata->state;
        } else {
            nodedata->state_pending = STATE_IDLE;
        }
        nodedata->state = STATE_CTS;
        nodedata->clock = get_time() + macMinSIFSPeriod;
        scheduler_add_callback(nodedata->clock, c, dcf_802_11_state_machine, NULL);
        break;
			
    case CTS_TYPE:
        /* Receive CTS */
        cts_header = (struct _dcf_802_11_cts_header *) (packet->data + sizeof(struct _dcf_802_11_header));
			
        if (header->dst != c->node) {
            /* Packet not for us */
            if (nodedata->nav < (get_time() + cts_header->nav)) {
                nodedata->nav = get_time() + cts_header->nav;
            }
            packet_dealloc(packet);
            return;
        }
        
        if (nodedata->state != STATE_TIMEOUT) {
            /* If not expecting cts, do nothing */
            packet_dealloc(packet);
            return;
        }

        if (nodedata->dst != header->src) {
            /* Expecting cts, but not from this node */
            packet_dealloc(packet);
            return;
        }

        /* Record CTS info */
        packet_dealloc(packet);
			
        /* Send DATA */
        nodedata->state = STATE_DATA;
        nodedata->clock = get_time() + macMinSIFSPeriod;
        scheduler_add_callback(nodedata->clock, c, dcf_802_11_state_machine, NULL);
        break;
			
    case DATA_TYPE:
        /* Received DATA */
        data_header = (struct _dcf_802_11_data_header *) (packet->data + sizeof(struct _dcf_802_11_header));
			
        if (header->dst != c->node) {
            /* Packet not for us */
            if (nodedata->nav < (get_time() + data_header->nav)) {
                nodedata->nav = get_time() + data_header->nav;
            }
            packet_dealloc(packet);
            return;
        }
				
        if ((nodedata->state != STATE_IDLE) 
            && (nodedata->state != STATE_BACKOFF) 
            && (nodedata->state != STATE_CTS_TIMEOUT)) {
            /* If not expecting data, do nothing */
            packet_dealloc(packet);
            return;
        }
			
        if ((nodedata->state == STATE_CTS_TIMEOUT) && (nodedata->dst != header->src)) {
            /* Expecting data, but not from this node */
            packet_dealloc(packet);
            return;
        }
							
        /* Send ACK */
        if (nodedata->state == STATE_BACKOFF) {
            nodedata->state_pending = nodedata->state;
        } else {
            nodedata->state_pending = STATE_IDLE;
        }
        nodedata->dst = header->src;
        nodedata->state = STATE_ACK;
        nodedata->clock = get_time() + macMinSIFSPeriod;
        scheduler_add_callback(nodedata->clock, c, dcf_802_11_state_machine, NULL);

        /* forward to upper layer */
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
			

    case BROADCAST_TYPE:
        /* Receive RTS*/
        data_header = (struct _dcf_802_11_data_header *) (packet->data + sizeof(struct _dcf_802_11_header));
        
        if (header->dst != BROADCAST_ADDR) {
            /* Packet not for us */
            packet_dealloc(packet);
            return;
        }
        
        /* forward to upper layer */
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

    case ACK_TYPE:
        /* Received ACK */
        ack_header = (struct _dcf_802_11_ack_header *) (packet->data + sizeof(struct _dcf_802_11_header));
        
        if (header->dst != c->node) {
            /* Packet not for us */
            packet_dealloc(packet);
            return;
        }
        
        if (nodedata->state != STATE_TIMEOUT) {
            /* If not expecting ack, do nothing */
            packet_dealloc(packet);
            return;
        }
				
        if (nodedata->dst != header->src) {
            /* Expecting ack, but not from this node */
            packet_dealloc(packet);
            return;
        }
				
        /* Destroy txbuf */
        packet_dealloc(packet);
        packet_dealloc(nodedata->txbuf);
        nodedata->txbuf = NULL;      
			
        /* Back to idle state*/
        nodedata->state = STATE_IDLE;
        nodedata->clock = get_time();			
      
        dcf_802_11_state_machine(c,NULL);
        break;
        
    default:
        packet_dealloc(packet);
        return;        
    }
}


/* ************************************************** */
/* ************************************************** */
int set_header(call_t *c, packet_t *packet, destination_t *dst) {
    struct _dcf_802_11_header *header = (struct _dcf_802_11_header *) packet->data;
    struct _dcf_802_11_data_header *dheader = (struct _dcf_802_11_data_header *) (packet->data + sizeof(struct _dcf_802_11_header));

    if ((header->dst = dst->id) == BROADCAST_ADDR) {
        header->type =BROADCAST_TYPE;
    } else {
        header->type = DATA_TYPE;
    }
    header->src = c->node;
    dheader->size = packet->size;
    return 0;
}

int get_header_size(call_t *c) {
    return (sizeof(struct _dcf_802_11_header) + sizeof(struct _dcf_802_11_data_header));
}

int get_header_real_size(call_t *c) {
    return (sizeof(struct _dcf_802_11_header) + sizeof(struct _dcf_802_11_data_header));
}


/* ************************************************** */
/* ************************************************** */
mac_methods_t methods = {rx, 
                         tx,
                         set_header, 
                         get_header_size,
                         get_header_real_size};


    
    
