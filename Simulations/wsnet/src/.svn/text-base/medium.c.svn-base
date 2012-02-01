/**
 *  \file   medium.c
 *  \brief  Radio medium management
 *  \author Guillaume Chelius & Elyes Ben Hamida & Quentin Lampin
 *  \date   2007
 **/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "internals.h"

/**
 * TODO: 
 *  - consider antenna angle at transmission time for gain computation
 *  - pass channel to antenna for gain computation 
 *  - deal with error introduction
 **/


/* ************************************************** */
/* ************************************************** */
entity_t *propagation_entity = NULL;
entity_t *shadowing_entity = NULL;
entity_t *fading_entity = NULL;
double propagation_range = 0;




/* ************************************************** */
/* ************************************************** */
int medium_init(void) {
    return 0;
}

void medium_clean(void) {
    return;
}


/* ************************************************** */
/* ************************************************** */
int medium_bootstrap(void) {
    call_t c = {-1, -1, -1};

    /* bootstrap propagation entity */
    if (propagation_entity->model->type != MODELTYPE_PROPAGATION) {
        fprintf(stderr, "entity %s is not a propagation model\n", propagation_entity->name);
        return -1;
    }
    c.entity = propagation_entity->id;
    if (propagation_entity->bootstrap) {
        propagation_entity->bootstrap(&c);
    }   

    /* bootstrap shadowing entity */
    if (shadowing_entity) {
        if (shadowing_entity->model->type != MODELTYPE_SHADOWING) {
            fprintf(stderr, "entity %s is not a shadowing model\n", shadowing_entity->name);
            return -1;
        }
        c.entity = shadowing_entity->id;
        if (shadowing_entity->bootstrap) {
            shadowing_entity->bootstrap(&c);
        }   
    }
    
    /* bootstrap fading entity */
    if (fading_entity) {
        if (fading_entity->model->type != MODELTYPE_FADING) {
            fprintf(stderr, "entity %s is not a fading model\n", fading_entity->name);
            return -1;
        }
        c.entity = fading_entity->id;
        if (fading_entity->bootstrap) {
            fading_entity->bootstrap(&c);
        }   
    }
    
    return 0;
}


/* ************************************************** */
/* ************************************************** */
double MEDIA_GET_NOISE(call_t *c, int channel) {
    call_t c0 = {c->from, c->node, -1};
    return medium_get_noise(&c0, channel);
}


void MEDIA_TX(call_t *c, packet_t *packet) {
    int i = get_node_count();
    node_t *node = get_node_by_id(c->node);
    uint64_t clock;
    
    /* check wether node is active */
    if (node->state != NODE_ACTIVE) {
        packet_dealloc(packet);
        return;
    }

    /* edit by Loic Lemaitre */
    if (node->worldsens == NODE_LOCAL) {  /* WORLDSENS MODE? -> radio part is simulated in wsim */
        /* fill packet info */
        packet->node = c->node;
	packet->antenna = c->from;
	packet->txdBm = radio_get_power(c);
	packet->channel = radio_get_channel(c);
	packet->modulation = radio_get_modulation(c);
	packet->Tb = radio_get_Tb(c);
	//packet->duration = packet->size * packet->Tb * 8;
	/* edit by Quentin Lampin <quentin.lampin@orange-ftgroup.com> */
	packet->duration = packet->real_size * packet->Tb;
	/* end of edition */
	packet->clock0 = get_time();
	packet->clock1 = packet->clock0 + packet->duration;

	/* scheduler tx_end event */
	scheduler_add_tx_end(packet->clock1, c, packet);
    }
    /* end of edition */
    
    /* scheduler rx_begin event */
#ifdef N_DAS_O
    if (propagation_range) {
        void *rx_nodes;
        node_t *rx_node;

        rx_nodes = spadas_rangesearch(location, NULL, &(node->position), propagation_range);
        while ((rx_node = (node_t *) das_pop(rx_nodes)) != NULL) {
            double derive = distance(&(node->position), &(rx_node->position)) / 0.3; 
            bundle_t *bundle = get_bundle_by_id(rx_node->bundle);
            int i;
    
            if (rx_node->state == NODE_DEAD) {
                continue;
            }

            for (i = 0; i < bundle->antenna.size; i++) {
                entity_t *entity = get_entity_by_id(bundle->antenna.elts[i]);
                call_t c0 = {entity->id, rx_node->id, -1}; 
                packet_t *packet_rx = packet_rxclone(packet);
                
                clock = packet->clock0 + ((uint64_t) derive);
                packet_rx->clock0 = clock;
                packet_rx->clock1 = clock + packet->duration;
                scheduler_add_rx_begin(clock, &c0, packet_rx);
            }
        }
        das_destroy(rx_nodes);
    } else {
        while (i--) {
            node_t *rx_node = get_node_by_id(i);
            double derive = distance(&(node->position), &(rx_node->position)) / 0.3; 
            bundle_t *bundle = get_bundle_by_id(rx_node->bundle);
            int i;
            
            if (rx_node->state == NODE_DEAD) {
                continue;
            }
            
            for (i = 0; i < bundle->antenna.size; i++) {
                entity_t *entity = get_entity_by_id(bundle->antenna.elts[i]);
                call_t c0 = {entity->id, rx_node->id, -1}; 
                packet_t *packet_rx = packet_rxclone(packet);

                clock = packet->clock0 + ((uint64_t) derive);
                packet_rx->clock0 = clock;
                packet_rx->clock1 = clock + packet->duration;
                scheduler_add_rx_begin(clock, &c0, packet_rx);
            }
        }
    }   
#else /* N_DAS_O */
    while (i--) {
        node_t *rx_node = get_node_by_id(i);
        double dist = distance(&(node->position), &(rx_node->position));
        double derive = dist / 0.3; 
        bundle_t *bundle = get_bundle_by_id(rx_node->bundle);
        int i;

        if (rx_node->state == NODE_DEAD) {
            continue;
        }
        
        if ((propagation_range) && (dist > propagation_range)) {
            continue;
        }
        
        for (i = 0; i < bundle->antenna.size; i++) {
            entity_t *entity = get_entity_by_id(bundle->antenna.elts[i]);
            call_t c0 = {entity->id, rx_node->id, -1}; 
            packet_t *packet_rx = packet_rxclone(packet);
            
            clock = packet->clock0 + ((uint64_t) derive);
            packet_rx->clock0 = clock;
            packet_rx->clock1 = clock + packet->duration;
            scheduler_add_rx_begin(clock, &c0, packet_rx);
        }
    }
#endif /* N_DAS_O */

}

void medium_tx_end(packet_t *packet, call_t *c) {
    node_t *node = get_node_by_id(c->node);
    entity_t *entity = get_entity_by_id(c->entity);
    
    /* check wether the node is still active */
    if (node->state == NODE_ACTIVE) {
        entity->methods->radio.tx_end(c, packet);
    }
    
    packet_dealloc(packet);
    return;
}


/* ************************************************** */
/* ************************************************** */
void medium_cs(packet_t *packet, call_t *c) {
    uint64_t clock = get_time() + packet->duration;
    node_t *node = get_node_by_id(c->node);
    entity_t *entity = get_entity_by_id(c->entity);

    /* check wether the node is able to receive */
    if (node->state != NODE_ACTIVE) {
        packet_dealloc(packet);
        return;
    }

    /*  propagation */
    medium_compute_rxdBm(packet, c);

    /* filter */
    /* edit by Loic Lemaitre */
    if (node->worldsens == NODE_LOCAL) {  /* WORLDSENS MODE? wsim deals with filtering */
        if (packet->rxdBm == MIN_DBM) {
	    packet_dealloc(packet);
	    return;
	}
    }
    /* end of edition */
    
    /* update noise */
#if (SNR_STEP != 0)
    noise_packet_cs(c, packet);
#endif /*SNR_STEP*/
    /* start reception */
    entity->methods->antenna.cs(c, packet);
    /* end reception */
    scheduler_add_rx_end(clock, c, packet);
}

void medium_rx(packet_t *packet, call_t *c) {
    node_t *node = get_node_by_id(c->node);

    /* check wether the node is able to receive */
    if (node->state != NODE_ACTIVE) {
        packet_dealloc(packet);
        return;
    }

    /* noise & PER */
#if (SNR_STEP != 0)
    packet->PER = 1;
    noise_packet_rx(c, packet);
    packet->PER = 1 - packet->PER;
#ifdef SNR_ERRORS
    modulation_errors(packet);
#endif /*SNR_ERRORS*/
#else /*SNR_STEP == 0*/
    packet->PER = 0;
#endif /*SNR_STEP*/

    /* receive */
    /* edit by Loic Lemaitre */
    if (node->worldsens == NODE_LOCAL) {   /* WORLDSENS MODE? */
        antenna_rx(c, packet);
    }
    else {
        worldsens_nodes_rx(c, packet);
    }
    /* end of edition */

}


/* ************************************************** */
/* ************************************************** */
void medium_compute_rxdBm(packet_t *packet, call_t *c) {
    call_t c0 = {packet->antenna, packet->node, -1};
    double rxdBm = packet->txdBm;
    position_t *pos_tx = get_node_position(packet->node);
    position_t *pos_rx = get_node_position(c->node);

    /* antenna tx white noise */
    rxdBm += antenna_get_loss(&c0);

    /* antenna tx gain (TODO: angle au moment de l'émission) */
    rxdBm += antenna_gain_tx(&c0, pos_rx);

    /* propagation */
    c0.entity = propagation_entity->id;
    if (c->node != packet->node) {
        rxdBm = propagation_entity->methods->propagation.propagation(&c0, packet, 
                                                                     packet->node,
                                                                     c->node, rxdBm);
    }

    /* TODO: add shadowing & fading only if > MIN_DBM */

    /* antenna rx gain */
    rxdBm += antenna_gain_rx(c, pos_tx);

    /* antenna rx white noise */
    rxdBm += antenna_get_loss(c);

    /* rx power */
    packet->rxdBm = rxdBm;
    packet->rxmW = dBm2mW(rxdBm);
}
