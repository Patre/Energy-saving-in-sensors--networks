/**
 *  \file   xmac.c
 *  \brief  X-MAC layer
 *  \author Romain Kuntz
 *  \date   01/2010
 **/

#include <stdio.h>
#include <include/modelutils.h>

/* X-MAC uses the same interfaces as BMAC */
#include "bmac.h"

/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Implementation of the X-MAC medium access control",
    "Romain Kuntz",
    "0.1",
    MODELTYPE_MAC, 
    {NULL, 0}
};

/* X-MAC is a preamble sampling MAC protocol
 * - It sends a serie of short preamble packets, each containing 
 *   ID of target node: nodes receiving the preamble and that 
 *   are not recipient of the ensuing data go back to sleep
 * - A short pause between preamble packets allows recipient to 
 *   send an early acknowledgement to receive the data sooner
 * - Potential senders that overhears such ack and that whish 
 *   to send to the same node can queue the packets directly 
 *   after (after a random backoff) the data sent by the other
 *   transmitter [Not implented yet, see TODO]
 */

/* ************************************************** */
/* ************************************************** */
struct _xmac_header {
    nodeid_t dst;
    nodeid_t src;
    uint8_t type;
};

/* ************************************************** */
/* ************************************************** */
struct xmac_nodedata {
    uint64_t clock;
    uint64_t backoff_begin;
    int64_t backoff_remain;
    uint64_t strobed_preamble;    
    uint64_t preamble_count;
    uint64_t preamble_pause;
    uint64_t preamble_listen;
    int state;
    int state_pending;
    int radio_state;
    int prio_queue;
    int pulse_count;
    int ack_wait_count;
    event_t *last_scheduled_event;
    nodeid_t dst_data;
    nodeid_t dst_ack;
    nodeid_t src_preamble;
    nodeid_t dst_preamble;
    packet_t *tx_pending;
    void *packets;

    // X-MAC configurable parameters
    int CCAEnabled;         // Whether CCA is enabled
    int LLAckEnabled;       // Whether LL Ack are required 
    int max_retrans;        // Max number of retransmission
    double busy_threshold;  // Busy channel threeshold
    uint64_t initBackoff;   // The Initial Backoff value
    uint64_t congBackoff;   // The Congestion Backoff value
    uint64_t LPL_checkint;  // The Check Interval for LPL
};

/* ************************************************** */
/* ************************************************** */
void switch_radio(int on, call_t *c); 
int state_machine(call_t *c, void *args);

/* ************************************************** */
/* ************************************************** */
// Debug macro
#ifdef LOG_MAC
#define DBG(arg...)  \
do { \
    fprintf(stderr, "%s: ", __FUNCTION__); \
    fprintf(stderr, arg); \
} while (0)
#else
#define DBG(...)
#endif /* LOG_MAC */

/* ************************************************** */
/* ************************************************** */
// X-MAC header types
#define PREAMBLE_WORD       0xaa    // 170
#define DATA                0x01    // 1
#define DATA_ACK            0x02    // 2
#define PREAMBLE_ACK        0x03    // 3

// X-MAC states
#define STATE_POWER_DOWN        1
#define STATE_PULSE_CHECK       2
#define STATE_IDLE              3
#define STATE_PRETX             4
#define STATE_TX_PREAMBLE       5
#define STATE_TX                6
#define STATE_TXING             7
#define STATE_TX_WAIT_ACK       8
#define STATE_RX_WAIT_DATA      9
#define STATE_TX_DACK           10
#define STATE_TX_DACK_END       11
#define STATE_TX_PACK           12

#define ONE_MS              1000000     // 1ms
#define ONE_SEC             1000000000  // 1s
#define RADIO_ON            1
#define RADIO_OFF           0

#define PULSE_TIME          450000      // Time to get a radio sample (450us)
#define PULSE_INIT_RADIO    2500000     // Time to init radio (2.5ms)

#define WAIT_ACK_TIMEOUT    100000000   // 100ms
#define ACK_DELTA           1000        // Delay between DATA and ACK (1us)

/* ************************************************** */
/* ************************************************** */
int get_header_size(call_t *c) {
    return (sizeof(struct _xmac_header));
}

int get_header_real_size(call_t *c) {
    return (sizeof(struct _xmac_header));
}

int set_header(call_t *c, packet_t *packet, 
                   destination_t *dst) {
    struct _xmac_header *header = 
        (struct _xmac_header *) packet->data;

    /* Fill the MAC header */ 
    header->dst = dst->id;
    header->src = c->node;
    header->type = DATA; 

    return 0;
}

int set_mac_header(call_t *c, packet_t *packet, 
               destination_t *dst, uint8_t type) {
    struct _xmac_header *header = 
        (struct _xmac_header *) packet->data;

    /* Fill the MAC header and rewrite the header type */
    set_header(c, packet, dst);
    header->type = type;    

    return 0;
}

/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    return 0;
}

int destroy(call_t *c) {
    return 0;
}

int bootstrap(call_t *c) {
    struct xmac_nodedata *nodedata = get_node_private_data(c);    

    nodedata->state = STATE_IDLE;
    switch_radio(RADIO_ON, c);
    state_machine(c, NULL);

    return 0;
}

int setnode(call_t *c, void *params) {
    struct xmac_nodedata *nodedata = malloc(sizeof(struct xmac_nodedata));
    call_t c0 = {get_entity_bindings_down(c)->elts[0],
                 c->node, c->entity};
    param_t *param;
    uint64_t pack_length;

    /* Initialization */
    nodedata->clock = 0;
    nodedata->backoff_begin = 0;
    nodedata->backoff_remain = 0;
    nodedata->state = STATE_IDLE;
    nodedata->state_pending = -1;
    nodedata->packets = das_create();
    nodedata->tx_pending = NULL;
    nodedata->dst_data = 0;
    nodedata->dst_ack = 0;
    nodedata->src_preamble = -1;
    nodedata->pulse_count = 1;
    nodedata->last_scheduled_event = NULL;
    nodedata->preamble_count = 0;
    nodedata->ack_wait_count = 1;
    nodedata->radio_state = RADIO_ON;
    nodedata->max_retrans = 2;
    nodedata->dst_preamble = -1;
    
    /* Length (in ns) of a Preamble ACK */
    pack_length = get_header_size(c) 
                  * 8 /* conversion Bytes -> bits */
                  * radio_get_Tb(&c0);

    /* Length (in ns) of a strobed preamble */
    nodedata->strobed_preamble = get_header_size(c) 
                            * 8 /* conversion Bytes -> bits */
                            * radio_get_Tb(&c0);

    /* The pause between the preamble depends on the 
     * radio speed. It must be equal to (at least) 
     * the time to send preamble ack. We set it to twice 
     * that time.
     */
    nodedata->preamble_pause = pack_length * 2;

    /* The time the node listens to the medium upon wake-up.
     * It must be equal to the pause between preambles plus
     * twice the size of a strobed preamble.
     */
    nodedata->preamble_listen = nodedata->preamble_pause
                                + (nodedata->strobed_preamble * 2);
   
    /* XMAC default parameters */
    nodedata->CCAEnabled = 1;
    nodedata->busy_threshold = -74; 
    nodedata->LLAckEnabled = 1;
    nodedata->LPL_checkint = MAC_LPL_MODE_4;
    nodedata->initBackoff = get_random_time_range(0, 10*ONE_MS);
    nodedata->congBackoff = get_random_time_range(0, 10*ONE_MS);

    /* XMAC parameters from the configuration file */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        /* Whether CCA is enabled */
        if (!strcmp(param->key, "cca")) {
            if (get_param_integer(param->value, 
                                  &(nodedata->CCAEnabled))) {
                goto error;
            }
        }
        /* Value of the busy channel threshold */
        if (!strcmp(param->key, "busy-threshold")) {
            if (get_param_double(param->value, 
                                 &(nodedata->busy_threshold))) {
                goto error;
            }
        }
        /* Whether LL ACK are enabled */      
        if (!strcmp(param->key, "ack")) {
            if (get_param_integer(param->value, 
                                  &(nodedata->LLAckEnabled))) {
                goto error;
            }
        }
        /* Value of the initial backoff */
        if (!strcmp(param->key, "init-back")) {
            if (get_param_time(param->value, 
                               &(nodedata->initBackoff))) {
                goto error;
            }
        }
        /* Value of the congestion backoff */
        if (!strcmp(param->key, "cong-back")) {
            if (get_param_time(param->value, 
                               &(nodedata->congBackoff))) {
                goto error;
            }
        }
        /* Value of LPL check interval */
        if (!strcmp(param->key, "lpl-check")) {
            if (get_param_time(param->value, 
                                  &(nodedata->LPL_checkint))) {
                goto error;
            }
        }
        /* Number of retransmission */
        if (!strcmp(param->key, "max-retrans")) {
            if (get_param_integer(param->value, 
                                  &(nodedata->max_retrans))) {
                goto error;
            }
        }
    }

    if (nodedata->LPL_checkint == MAC_LPL_MODE_0) {
        /* Do not send preamble in LPL mode 0 
         * (radio is always on)
         */
        nodedata->strobed_preamble = 0;
        nodedata->preamble_pause = 0;
        nodedata->preamble_listen = 0;
    }

    set_node_private_data(c, nodedata);
    return 0;

error:
    free(nodedata);
    return -1;
}

int unsetnode(call_t *c) {
    struct xmac_nodedata *nodedata = get_node_private_data(c);
    packet_t *packet;

    if (nodedata->tx_pending != NULL) {
        packet_dealloc(nodedata->tx_pending);
        nodedata->tx_pending = NULL;
    }

    /* Free the pending packets in the FIFO */
    while ((packet = (packet_t *) 
                das_pop(nodedata->packets)) != NULL) {
        packet_dealloc(packet);
    }
    das_destroy(nodedata->packets);   

    free(nodedata);
    return 0;
}

/* IOCTL function that can be called by other layers 
 * in order to configure some of the X-MAC parameters.
 * See xmac.h
 */
int ioctl(call_t *c, int option, void *in, void **out) {
    struct xmac_nodedata *nodedata = get_node_private_data(c);
    uint64_t new_value = *((uint64_t *) in);

    switch (option) {
        case MAC_UPDATE_CCA:
            DBG("CCAEnabled being modified from %d to %"PRId64"\n",
                nodedata->CCAEnabled, new_value);
            nodedata->CCAEnabled = (int) new_value;
            break;

        case MAC_UPDATE_LLACK:
            DBG("LLAckEnabled being modified from %d to %"PRId64"\n",
                nodedata->LLAckEnabled, new_value);
            nodedata->LLAckEnabled = (int) new_value;
            break;

        case MAC_UPDATE_INIT_BACKOFF:
            DBG("initBackoff being modified from %"PRId64" to %"PRId64"\n",
                nodedata->initBackoff, new_value);
            nodedata->initBackoff = new_value;
            break;

        case MAC_UPDATE_CONG_BACKOFF:
            DBG("congBackoff being modified from %"PRId64" to %"PRId64"\n",
                nodedata->congBackoff, new_value);
            nodedata->congBackoff = new_value;
            break; 

        case MAC_UPDATE_LPL_CHECKINT:
            DBG("LPL_checkint being modified from %"PRId64" to %"PRId64"\n",
                nodedata->LPL_checkint, new_value);
            nodedata->LPL_checkint = new_value;
            break;

         default:
            DBG("Unknown IOCTL message\n");
            break;
     }

    set_node_private_data(c, nodedata);
    return 0;    
}

/* ************************************************** */
/* ************************************************** */
void xmac_add_callback(uint64_t clock, call_t *c,
                        callback_t callback) {
    struct xmac_nodedata *nodedata = get_node_private_data(c);

    /* Only one event at a time can be scheduled by the 
     * state machine, so we remove the last scheduled 
     * event if it exists and has not been executed yet 
     */
    if (nodedata->last_scheduled_event != NULL
            && nodedata->last_scheduled_event->clock > get_time()) {
        scheduler_delete_callback(c, nodedata->last_scheduled_event);
    }

    /* Schedule the new event */
    nodedata->last_scheduled_event = 
        scheduler_add_callback(clock, c, callback, NULL);
    return;
}

/* This function must be called whenever the node leaves 
 * the state STATE_PRETX
 */
void backoff_stop(call_t *c) {
    struct xmac_nodedata *nodedata = get_node_private_data(c);
  
    /* Decrement the current backoff */
    if (nodedata->backoff_remain > 0) {
        nodedata->backoff_remain -= 
            (get_time() - nodedata->backoff_begin);
    }

    return;
}

/* This function must be called whenever the node goes 
 * back to the state STATE_PRETX
 */ 
uint64_t backoff_restart(call_t *c) {
    struct xmac_nodedata *nodedata = get_node_private_data(c);

    if (nodedata->backoff_remain <= 0) {
        nodedata->backoff_remain = 0;
    } else {
        /* Set the new backoff starting time */
        nodedata->backoff_begin = get_time();
    }

    /* Return to the state STATE_PRETX */
    nodedata->state = STATE_PRETX;
    return get_time() + nodedata->backoff_remain;
}

int check_channel_busy(call_t *c) {
    struct xmac_nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0],
                 c->node, c->entity};

    if (nodedata->radio_state == RADIO_OFF) {
        DBG("%d - BUG - checking channel while radio is OFF\n", 
            c->node);
    }

    if (radio_get_cs(&c0) >= nodedata->busy_threshold 
        || radio_get_noise(&c0) >= nodedata->busy_threshold) {
        return 1;
    }

    return 0;
}

void switch_radio(int on, call_t *c) {
    struct xmac_nodedata *nodedata = get_node_private_data(c); 
    call_t c0 = {get_entity_bindings_down(c)->elts[0], 
                 c->node, c->entity};

    /* Nothing to do if the state remains the same */
    if (nodedata->radio_state == on) {
        return;
    }

    if (on == RADIO_ON) {
        radio_wakeup(&c0);
        nodedata->radio_state = RADIO_ON;
    } else {
        radio_sleep(&c0);  
        nodedata->radio_state = RADIO_OFF;
    }
}

/* X-MAC state machine */
int state_machine(call_t *c, void *args) { 
    struct xmac_nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], 
                 c->node, c->entity};
    uint64_t timeout, backoff;
    packet_t *packet;
    int packet_size;
    struct _xmac_header *header; 
    destination_t destination;

    /* current event is going to be deleted, so stop to point at it */
    nodedata->last_scheduled_event = NULL;

    /* Drop unscheduled events */
    if (nodedata->clock != get_time()) {
        return 0;
    }

    switch (nodedata->state) {
        int busy_channel;            

        case STATE_IDLE:
            if (nodedata->tx_pending == NULL) {
                /* Check if a packet is available in the qeue */
                nodedata->tx_pending = 
                    (packet_t *) das_pop_FIFO(nodedata->packets);

                if (nodedata->tx_pending == NULL) {
                    if (nodedata->LPL_checkint != MAC_LPL_MODE_0) {
                        /* If no packet in queue, go to 
                         * STATE_POWER_DOWN, turn off the 
                         * radio and wake-up later
                         */
                        nodedata->state = STATE_POWER_DOWN;
                        switch_radio(RADIO_OFF, c);
                        nodedata->clock = get_time() 
                                + nodedata->LPL_checkint;
                        xmac_add_callback(nodedata->clock, c, 
                                          state_machine);
                        break;
                    } else {
                        /* In MAC_LPL_MODE_0, the radio is always on,
                         * we stay in STATE_IDLE
                         */
                        break;
                    }
                }
            }

            /* Use an initial backoff before transmitting
             * Note: this is different from 802.11 which does not 
             * use any backoff if the channel is clear after DIFS
             */
            if (nodedata->CCAEnabled) {
                /* Go to STATE_PRETX */
                backoff = nodedata->initBackoff;
                nodedata->backoff_begin = get_time();
                nodedata->backoff_remain = backoff;
                nodedata->state = STATE_PRETX;
            } else {
                /* CCA is disabled, backoff is set to 0 */
                backoff = 0;
                nodedata->backoff_begin = get_time();
                nodedata->backoff_remain = 0;

                if (nodedata->LPL_checkint == MAC_LPL_MODE_0) {
                    /* Go directly to STATE_TX */
                    nodedata->state = STATE_TX;
                } else {
                    /* Go to STATE_TX_PREAMBLE */
                    nodedata->state = STATE_TX_PREAMBLE;
                }
            }

            nodedata->clock = get_time() + backoff;
            xmac_add_callback(nodedata->clock, c, state_machine);
            break;

        case STATE_PRETX:      
            if (nodedata->tx_pending == NULL) {
                nodedata->backoff_remain = 0; 
                nodedata->backoff_begin = 0;
                nodedata->state = STATE_IDLE;
                nodedata->clock = get_time();
                xmac_add_callback(nodedata->clock, c, state_machine);
                break;
            }

            /* Decrement the current backoff */
            if (nodedata->backoff_remain > 0) {
                nodedata->backoff_remain -= 
                    (get_time() - nodedata->backoff_begin);
            }

            if (nodedata->backoff_remain > 0) {
                nodedata->clock = get_time() + nodedata->backoff_remain;
                nodedata->state = STATE_PRETX;
                xmac_add_callback(nodedata->clock, c, state_machine);
                break;
            }

            /* Backoff is 0, going to STATE_PULSE_CHECK 
             * in order to perform the CCA check
             */
            nodedata->state = STATE_PULSE_CHECK;
            nodedata->state_pending = STATE_PRETX;
            nodedata->pulse_count = 1;
            nodedata->clock = get_time() + nodedata->preamble_listen;
            xmac_add_callback(nodedata->clock, c, state_machine);
            break;

        case STATE_POWER_DOWN:
            /* Turn on the radio and wait at least 
             * preamble_listen before going to STATE_PULSE_CHECK */
            switch_radio(RADIO_ON, c);
            nodedata->state = STATE_PULSE_CHECK;
            nodedata->pulse_count = 1;            
            nodedata->clock = get_time() + nodedata->preamble_listen;
            xmac_add_callback(nodedata->clock, c, state_machine);
            break;

        case STATE_PULSE_CHECK:
            /* Looking for outliers (i.e. channel energy below 
             * threshold, which means channel is clear)
             */
            busy_channel = check_channel_busy(c);

            if (!busy_channel) {
                /* Outlier found, i.e. channel is clear */
                if (nodedata->state_pending == STATE_PRETX) {
                    nodedata->backoff_begin = get_time();
                    nodedata->backoff_remain = 0; 

                    if (nodedata->LPL_checkint == MAC_LPL_MODE_0) {
                        /* Go directly to STATE_TX */
                        nodedata->state = STATE_TX;
                    } else {
                        /* Go to STATE_TX_PREAMBLE */
                        nodedata->state = STATE_TX_PREAMBLE;
                    }

                    nodedata->clock = get_time();
                    nodedata->state_pending = -1;
                } else {
                    /* Back to STATE_PRETX */
                    nodedata->clock = backoff_restart(c);
                }

                xmac_add_callback(nodedata->clock, c, state_machine);
            } else if (nodedata->pulse_count < 5) {
                /* Channel is busy, but we sample the channel 5 
                 * times 
                 */
                nodedata->pulse_count++;
                nodedata->clock = get_time() + PULSE_TIME;
                xmac_add_callback(nodedata->clock, c, state_machine);
            } else {
                /* No outliers found after 5 trials, i.e 
                 * channel is busy
                 */
                if (nodedata->state_pending == STATE_PRETX) {
                    /* Configure a congestion backoff.
                     * Note: this is different from 802.11 which 
                     * uses congestion backoff only if it does 
                     * not receive an ack (e.g. in case of 
                     * collision)
                     */
                    backoff = nodedata->congBackoff;
                    nodedata->backoff_begin = get_time();
                    nodedata->backoff_remain = backoff; 
                    nodedata->clock = get_time() + backoff;
                    nodedata->state_pending = -1;                    
                } else {
                    nodedata->clock = get_time();
                }

                nodedata->state = STATE_PRETX;
                xmac_add_callback(nodedata->clock, c, state_machine);
            }
            break;

        case STATE_TX_PREAMBLE:
            /* X-MAC divides the preamble into small packets 
             * sent consecutively on the medium. The entire 
             * preamble is a little longer than the LPL check 
             * interval.
             */
            if (nodedata->preamble_count <= (nodedata->LPL_checkint
                                         + nodedata->strobed_preamble
                                         + nodedata->preamble_pause)) {
                packet_size = get_header_size(c);
                packet = packet_create(c, packet_size, -1);

                if (packet == NULL || nodedata->tx_pending == NULL) {
                    DBG("%d - STATE_TX_PREAMBLE: BUG (packet is NULL)\n",
                        c->node);
                    break;
                }

                /* Destination of the preamble is the same as the 
                 * packet destination
                 */
                header = (struct _xmac_header *) nodedata->tx_pending->data;
                destination.id = header->dst;
                nodedata->dst_preamble = header->dst;
                memset(packet->data, 0, packet_size);

                /* Init the preamble header */
                set_mac_header(c, packet, &destination, PREAMBLE_WORD);
                nodedata->state = STATE_TX_PREAMBLE;

                /* Adjust clock to the end of transmission */
                timeout = nodedata->strobed_preamble
                          + nodedata->preamble_pause;

                /* Adjust preamble_count to the length of a preamble 
                 * word plus the pause between the preambles
                 */
                nodedata->preamble_count += timeout;

                //DBG("%d - Tx preamble to dst %d: preamble_count = "
                //        "%"PRId64"\n", c->node, header->dst, 
                //        nodedata->preamble_count);

                /* Send the preamble packet */
                TX(&c0, packet);
            } else {
                /* All the preamble has been sent */
                nodedata->preamble_count = 0;
                nodedata->dst_preamble = -1;

                /* If the packet is a broadcast one, it can be sent 
                 * directly after the whole preamble (no ACK were
                 * expected). If it a unicast packet, a Preamble ACK 
                 * was expected but not received. We however still 
                 * send the packet as the Peamble ACK was maybe lost.
                 */
                timeout = 0;
                nodedata->state = STATE_TX;
            }

            nodedata->clock = get_time() + timeout;
            xmac_add_callback(nodedata->clock, c, state_machine);
            break;
 
        case STATE_TX:
            if (nodedata->tx_pending == NULL) {
                 DBG("%d - STATE_TX: BUG (Trying to send NULL packet)\n",
                     c->node);
                break;
            }

            packet = packet_clone(nodedata->tx_pending);
            header = (struct _xmac_header *) packet->data;
            header->type = DATA;
            timeout = (packet->size * 8 * radio_get_Tb(&c0));

            /* Send the Data packet */
            TX(&c0, packet); 

            nodedata->state = STATE_TXING;
            nodedata->dst_data = header->dst;

            /* Adjust clock to the end of transmission */
            nodedata->clock = get_time() + timeout;
            xmac_add_callback(nodedata->clock, c, state_machine);

            DBG("%d - STATE_TX: TX at %"PRId64" until %"PRId64"\n",
                c->node, get_time(), nodedata->clock);
            break;

        case STATE_TXING:
            /* End of transmission of a Data packet.  
             * Wait for an ACK if the packet is not 
             * broadcasted and if LL Ack are enabled
             */
            if (nodedata->LLAckEnabled 
                    && nodedata->dst_data != BROADCAST_ADDR) {
                /* Switch to STATE_TX_WAIT_ACK */
                nodedata->state = STATE_TX_WAIT_ACK;
                nodedata->clock = get_time() + WAIT_ACK_TIMEOUT;
                xmac_add_callback(nodedata->clock, c, state_machine);
            } else {
                /* Dealloc the buffered packet */
                packet_dealloc(nodedata->tx_pending);                
                nodedata->tx_pending = NULL;
                nodedata->backoff_remain = 0; 
                nodedata->backoff_begin = 0;

                /* Switch to STATE_IDLE */
                nodedata->state = STATE_IDLE;
                nodedata->clock = get_time();
                xmac_add_callback(nodedata->clock, c, state_machine);
            }
            break;

        case STATE_TX_WAIT_ACK:
            /* This state is reached either because no data ACK were 
             * received, or because the whole preamble was sent 
             * without receiving a single preamble ACK
             */
            if (nodedata->ack_wait_count >= nodedata->max_retrans) {
                /* We reached the retransmission limit */ 
                DBG("%d - STATE_TX_WAIT_ACK: No preamble/data ack received "
                    "after %d trials\n", c->node, nodedata->max_retrans);

                /* Reinit the retransmission counter */
                nodedata->ack_wait_count = 1;

                /* Dealloc the buffered packet */
                packet_dealloc(nodedata->tx_pending);
                nodedata->tx_pending = NULL;
                nodedata->backoff_remain = 0; 
                nodedata->backoff_begin = 0;
            } else {
                /* Retransmit packet */
                DBG("%d - STATE_TX_WAIT_ACK: No ack received "
                    "(trial %d), trying to retransmit packet\n",
                    c->node, nodedata->ack_wait_count);
                nodedata->ack_wait_count++;
            }

            /* Back to STATE_IDLE */
            nodedata->state = STATE_IDLE;
            nodedata->clock = get_time();
            xmac_add_callback(nodedata->clock, c, state_machine);
            break;

        case STATE_TX_PACK:
            /* Send a PREAMBLE ACK */
            packet_size = get_header_size(c);
            packet = packet_create(c, packet_size, -1);

            if (packet == NULL) {
                DBG("%d - STATE_TX_PACK: BUG (packet is NULL)\n",
                    c->node);
                break;
            }

            destination.id = nodedata->src_preamble;
            memset(packet->data, 0, packet_size);
                
            /* Init the Preamble ACK header */
            set_mac_header(c, packet, &destination, PREAMBLE_ACK);

            /* Send the Preamble ACK */
            TX(&c0, packet);

            /* Adjust clock to the end of transmission */
            timeout = (packet_size * 8 /* conversion Bytes -> bits */ 
                                  * radio_get_Tb(&c0))
                                  // We wait the DATA at least during
                                  // the time of the preamble length
                                  + nodedata->LPL_checkint;            
            nodedata->clock = get_time() + timeout;

            DBG("%d - STATE_TX_PACK: TX at %"PRId64" until "
                 "%"PRId64"\n", c->node, get_time(), nodedata->clock);

            /* Go to STATE_RX_WAIT_DATA */
            nodedata->state = STATE_RX_WAIT_DATA;            
            xmac_add_callback(nodedata->clock, c, state_machine); 
            break;

        case STATE_TX_DACK:
            /* Send a DATA ACK */
            packet_size = get_header_size(c);
            packet = packet_create(c, packet_size, -1);

            if (packet == NULL) {
                DBG("%d - STATE_TX_DACK: BUG (packet is NULL)\n",
                    c->node);
                break;
            }

            destination.id = nodedata->dst_ack;
            memset(packet->data, 0, packet_size);
                
            /* Init the DATA ACK header */
            set_mac_header(c, packet, &destination, DATA_ACK);
            nodedata->dst_ack = 0;

            /* Send the DATA ACK */
            TX(&c0, packet);

            /* Adjust clock to the end of transmission */
            timeout = packet_size * 8 /* conversion Bytes -> bits */ 
                                  * radio_get_Tb(&c0); 
            nodedata->clock = get_time() + timeout;

            /* Go to STATE_TX_DACK_END */
            nodedata->state = STATE_TX_DACK_END;
            xmac_add_callback(nodedata->clock, c, state_machine);
 
            DBG("%d - STATE_TX_DACK: TX at %"PRId64" until "
                 "%"PRId64"\n", c->node, get_time(), nodedata->clock);
            break;

        case STATE_TX_DACK_END:
            /* Back to STATE_PRETX */
            nodedata->clock = backoff_restart(c);
            xmac_add_callback(nodedata->clock, c, state_machine);
            break;

        case STATE_RX_WAIT_DATA:
            DBG("%d - %"PRId64" - Waiting for DATA for too long "
                 "(from node %d), back to STATE_PRETX\n", 
                 c->node, get_time(), nodedata->src_preamble); 

            nodedata->src_preamble = -1;

            /* Back to STATE_PRETX */
            nodedata->clock = backoff_restart(c);
            xmac_add_callback(nodedata->clock, c, state_machine);
            break;

        default:        
            DBG("%d - STATE BUG (Should never reach this state)\n", 
                c->node);
            break;
    }

    if (args) {
        free((int *) args);
    }

    return 0;
}

/* ************************************************** */
/* ************************************************** */
void tx(call_t *c, packet_t *packet) {
    struct xmac_nodedata *nodedata = get_node_private_data(c);

    /* Add the packet in the FIFO */
    das_insert(nodedata->packets, (void *) packet);

    if (nodedata->state == STATE_POWER_DOWN) {
        nodedata->state = STATE_IDLE;
        switch_radio(RADIO_ON, c);
    } 

    if (nodedata->state == STATE_IDLE) {
        nodedata->clock = get_time();
        xmac_add_callback(nodedata->clock, c, state_machine);
    }
}

/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    struct _xmac_header *header;
    array_t *up = get_entity_bindings_up(c);
    struct xmac_nodedata *nodedata = get_node_private_data(c);    

    header = (struct _xmac_header *) packet->data;

    /* Configure a congestion backoff if a preamble word
     * is received while waiting for sending a preamble or
     * data packet */ 
    if (header->type == PREAMBLE_WORD
             && nodedata->state == STATE_PULSE_CHECK
             && nodedata->state_pending == STATE_PRETX) {
        /* Configure a congestion backoff */
        nodedata->backoff_begin = get_time();
        nodedata->backoff_remain = nodedata->congBackoff; 
        nodedata->clock = get_time() + nodedata->congBackoff;
        nodedata->state_pending = -1;                    
        nodedata->state = STATE_PRETX;
        xmac_add_callback(nodedata->clock, c, state_machine);
    }

    /* Accept packet only if destined to myself or 
     * if it is a broadcast packet. With X-MAC, preambles 
     * have a destination set to the destination of the DATA.
     * Thus, nodes overhearing a preamble that is not destined 
     * to them automatically go back to sleep.
     */
    if ((header->dst != c->node) 
            && (header->dst != BROADCAST_ADDR)) {
        packet_dealloc(packet);
        goto out;
    }

    switch (header->type) {
        case PREAMBLE_WORD:
            /* Drop preamble if we are not ready to receive it */
            if (nodedata->state != STATE_IDLE 
                    && nodedata->state != STATE_PRETX
                    && nodedata->state != STATE_PULSE_CHECK) {
                //DBG("%d - Dropping preamble packet, "
                //     "node is in state %d\n", 
                //     c->node, nodedata->state);
            } else {
                DBG("%d - Rx PREAMBLE_WORD\n", c->node);

                backoff_stop(c);
                nodedata->src_preamble = header->src;

                if (header->dst != BROADCAST_ADDR) {
                    /* Send the Preamble ACK after a small delta: it
                     * avoids capture effect problems between
                     * the ACK and the data at the receiving node
                     */
                    nodedata->state = STATE_TX_PACK;
                    nodedata->clock = get_time() + ACK_DELTA;

                    DBG("%d - Tx PACK\n", c->node);
                    xmac_add_callback(nodedata->clock, c, state_machine);
                } else {
                    /* The preamble precedes a broadcast packet, 
                     * so we do not send a preamble ACK and wait for 
                     * the data.
                     */
                    nodedata->state = STATE_RX_WAIT_DATA;
                    nodedata->clock = get_time() 
                            + nodedata->LPL_checkint 
                            + 10*ONE_MS;
                    xmac_add_callback(nodedata->clock, c, state_machine); 
                }
            }

            /* The preamble packet does not need to be 
             * distributed to the upper layers
             */
            packet_dealloc(packet);
            break;

        case DATA:  
            DBG("%d - Rx DATA from node %d\n", c->node, header->src);
            if (nodedata->state == STATE_RX_WAIT_DATA
                    || nodedata->state == STATE_PULSE_CHECK
                    || nodedata->LPL_checkint == MAC_LPL_MODE_0) {
                /* Send ACK if LL ACK are enabled and the 
                 * received packet is an unicast one
                 */
                if (nodedata->LLAckEnabled 
                        && header->dst != BROADCAST_ADDR) {
                    nodedata->dst_ack = header->src;
                    /* Send the DATA ACK after a small delta: it
                     * avoids capture effect problems between 
                     * the ack and the data at the receiving node
                     */
                    nodedata->state = STATE_TX_DACK;
                    nodedata->clock = get_time() + ACK_DELTA;
                } else {
                    /* Go back to STATE_PRETX */
                    // TODO X-MAC 
                    // Here the node could wait for more data 
                    // (in order to implement the packet sent 
                    // consecutively on the medium by other 
                    // transmitters)
                    nodedata->clock = backoff_restart(c);
                }

                /* Drop packet if no upper layer available */
                if (up == NULL) {
                    packet_dealloc(packet);                 
                } else {
                    int i = up->size;

                    /* Distribute the packet to each entity
                     * located one layer above
                     */
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

                xmac_add_callback(nodedata->clock, c, state_machine);
            } else {
                packet_dealloc(packet);
            }
            break;

        case DATA_ACK:   
            if (nodedata->state == STATE_TX_WAIT_ACK) {
                /* Check if we are waiting for an ack from this node */
                if (nodedata->dst_data == header->src) {
                    DBG("%d - Rx DATA_ACK from %d, going to "
                         "STATE_IDLE\n", c->node, header->src);
                    nodedata->dst_data = 0;
                    nodedata->state = STATE_IDLE;
                    nodedata->clock = get_time();

                    /* Dealloc the buffered Data packet */
                    packet_dealloc(nodedata->tx_pending);
                    nodedata->backoff_remain = 0; 
                    nodedata->backoff_begin = 0;
                    nodedata->tx_pending = NULL;

                    /* Reinit retransmission counter */
                    nodedata->ack_wait_count = 1;

                    xmac_add_callback(nodedata->clock, c, 
                                      state_machine);
                } else {
                    DBG("%d - Undesired ACK (from %d but waiting "
                         "from %d)\n", c->node, header->src, 
                         nodedata->dst_data);
                }
            }

            packet_dealloc(packet);
            break;

        case PREAMBLE_ACK:   
            if (nodedata->state == STATE_TX_PREAMBLE) {
                /* Check if we are waiting for a preamble 
                 * ack from this node 
                 */
                if (nodedata->dst_preamble == header->src) {
                    DBG("%d - Rx PREAMBLE_ACK from %d, going to "
                         "STATE_TX\n", c->node, header->src);
                    nodedata->dst_preamble = -1;

                    /* Reinitialize preamble counter */
                    nodedata->preamble_count = 0;                

                    nodedata->state = STATE_TX;
                    nodedata->clock = get_time();
                    xmac_add_callback(nodedata->clock, c, 
                                      state_machine);
                } else {
                    DBG("%d - Undesired PREAMBLE_ACK (from %d "
                        "but waiting from %d)\n", c->node, 
                        header->src, nodedata->dst_preamble);
                }
            }

            packet_dealloc(packet);
            break;
            
        default:
            DBG("%d - Rx BUG (Unknown packet type)\n", c->node);
            packet_dealloc(packet);
            break;
    }

out:
    return;
}

/* ************************************************** */
/* ************************************************** */
mac_methods_t methods = {rx, 
                         tx,
                         set_header, 
                         get_header_size,
                         get_header_real_size};
