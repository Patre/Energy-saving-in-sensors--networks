/**
 *  \file   noise.c
 *  \brief  Radio noise management
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include "internals.h"

/**
 * TODO:
 *  - optimize update_noise function, too many parameters, pointers and it seems incredible to have to use the factor correlation[packet->channel][packet->channe]
 **/

/* ************************************************** */
/* ************************************************** */
typedef struct noise_interval {
    uint64_t begin;
    uint64_t end;
    int active;

    double noise[CHANNELS_NUMBER];
    
    struct noise_interval *next;
    struct noise_interval *prev;
} noise_interval_t;

typedef struct noise {
    noise_interval_t *current;    
    noise_interval_t *first;
    noise_interval_t *last;    
} noise_t;


/* ************************************************** */
/* ************************************************** */
static void *mem_interval = NULL;
double correlation[CHANNELS_NUMBER][CHANNELS_NUMBER];
entity_t *noise_entity = NULL;
entity_t *interference_entity = NULL;


/* ************************************************** */
/* ************************************************** */
double get_white_noise(int node, int channel);


/* ************************************************** */
/* ************************************************** */
noise_t *get_noise_byid(call_t *c) {
    node_t *node = get_node_by_id(c->node);
    noise_t *noise = ((noise_t *)  node->noises) + c->entity;
    return noise;
}


/* ************************************************** */
/* ************************************************** */
int noise_init(void) {
    if ((mem_interval = mem_fs_slice_declare(sizeof(noise_interval_t))) == NULL) {
        return -1;
    }
    
    return 0;
}

int noise_bootstrap(void) {
    int i, j;
    call_t c = {-1, -1, -1};

    /* bootstrap interference entity */
    if (interference_entity->model->type != MODELTYPE_INTERFERENCES) {
        fprintf(stderr, "entity %s is not an interferences model\n", interference_entity->name);
        return -1;
    }
    c.entity = interference_entity->id;
    if (interference_entity->bootstrap) {
        interference_entity->bootstrap(&c);
    }   
    
    /* bootstrap noise entity */
    if (noise_entity) {
        if (noise_entity->model->type != MODELTYPE_NOISE) {
            fprintf(stderr, "entity %s is not an noise model\n", noise_entity->name);
            return -1;
        }
        c.entity = noise_entity->id;
        if (noise_entity->bootstrap) {
            noise_entity->bootstrap(&c);
        }   
    }
    
    /* fill correlation array */
    for (i = 0; i < CHANNELS_NUMBER; i++) {
        for (j = 0; j < CHANNELS_NUMBER; j++) {
            correlation[i][j] = interference_entity->methods->interferences.interfere(&c, i, j);
        }
    }
    
    /* initialize noises */
    for (i = 0; i < nodes.size; i++) {
        node_t *node = get_node_by_id(i);
        bundle_t *bundle = get_bundle_by_id(node->bundle);
        int j;
        
        /* allocate noises */
        node->noises = (void *) malloc(sizeof(noise_t) * entities.size);

        /* allocate antenna noises */
        for (j = 0; j < bundle->antenna.size; j++) {
            call_t c = {bundle->antenna.elts[j], i, -1};
            noise_t *noise = get_noise_byid(&c);
            noise->first = NULL;
            noise->current = NULL;
            noise->last = NULL;
        }
    }
    
    return 0;
}

void noise_clean(void) {
    int i;
    
    if (nodes.elts == NULL) {
        return;
    }
    
    for (i = 0; i < nodes.size; i++) {
        node_t *node = get_node_by_id(i);
        bundle_t *bundle = get_bundle_by_id(node->bundle);
        int j;
        
        if (node->noises == NULL) {
            continue;
        }
        
        for (j = 0; j < bundle->antenna.size; j++) {
            call_t c = {bundle->antenna.elts[j], i, -1};
            noise_t *noise = get_noise_byid(&c);
            noise_interval_t *noise_interval;
            
            while ((noise_interval = noise->first)) {
                noise->first = noise_interval->next;
                mem_fs_dealloc(mem_interval, noise_interval);
            }
        }
        
        free(node->noises);
    }

    return;
}


/* ************************************************** */
/* ************************************************** */
void add_signal2noise(double *noise, int channel, double signal) {
    int i;
    for (i = 0; i < CHANNELS_NUMBER; i++) {
        noise[i] += correlation[i][channel] * signal;
    }
}

void add_new_signal2noise(double *noise, int channel, double signal) {
    int i;
    for (i = 0; i < CHANNELS_NUMBER; i++) {
        noise[i] = correlation[i][channel] * signal;
    }
}


/* ************************************************** */
/* ************************************************** */
void noise_update_current(noise_t *noise, uint64_t time) {    
    while (noise->current && (noise->current->end <= time)) {
        noise->current = noise->current->next;
    }
}

int add_packet2interval(noise_t *noise, noise_interval_t *interval, packet_t *packet) {
    if ((interval->begin >= packet->clock0) && (interval->end <= packet->clock1)) {
        add_signal2noise(interval->noise, packet->channel, packet->rxmW);
        interval->active++;
        if (interval->end == packet->clock1) {
            return -1;
        } else {
            return 0;
        }
    } else if ((interval->begin >= packet->clock0) && (interval->end > packet->clock1)) {
        noise_interval_t *n_interval = (noise_interval_t *) mem_fs_alloc(mem_interval);
        
        /* add new interval at the end */
        memcpy(n_interval, interval, sizeof(noise_interval_t));
        n_interval->begin = packet->clock1;
        n_interval->prev = interval;

        /* update next prev link or noise->last */
        if (n_interval->next)
            n_interval->next->prev = n_interval;
        else 
            noise->last = n_interval;
        
        /* update considered interval */
        add_signal2noise(interval->noise, packet->channel, packet->rxmW);
        interval->active++;
        interval->end = packet->clock1;
        interval->next = n_interval;
        return -1;
    } else if ((interval->begin < packet->clock0) && (interval->end <= packet->clock1)) {
        noise_interval_t *p_interval = (noise_interval_t *) mem_fs_alloc(mem_interval);
        
        /* add new interval at the beginning */
        memcpy(p_interval, interval, sizeof(noise_interval_t));
        p_interval->end = packet->clock0;
        p_interval->next = interval;

        /* update next prev link or noise->last */
        if (p_interval->prev)
            p_interval->prev->next = p_interval;
        else 
            noise->first = p_interval;
        
        /* update considered interval */
        add_signal2noise(interval->noise, packet->channel, packet->rxmW);
        interval->active++;
        interval->begin = packet->clock0;
        interval->prev = p_interval;
        if (interval->end == packet->clock1) {
            return -1;
        } else {
            return 0;
        }
    } else if ((interval->begin < packet->clock0) && (interval->end > packet->clock1)) {
        noise_interval_t *n_interval = (noise_interval_t *) mem_fs_alloc(mem_interval);
        noise_interval_t *p_interval = (noise_interval_t *) mem_fs_alloc(mem_interval);

        /* add new interval at the beginning */
        memcpy(p_interval, interval, sizeof(noise_interval_t));
        p_interval->end = packet->clock0;
        p_interval->next = interval;

        /* update next prev link or noise->last */
        if (p_interval->prev)
            p_interval->prev->next = p_interval;
        else 
            noise->first = p_interval;

        /* add new interval at the end */
        memcpy(n_interval, interval, sizeof(noise_interval_t));
        n_interval->begin = packet->clock1;
        n_interval->prev = interval;

        /* update next prev link or noise->last */
        if (n_interval->next)
            n_interval->next->prev = n_interval;
        else 
            noise->last = n_interval;
        
        /* update considered interval */
        add_signal2noise(interval->noise, packet->channel, packet->rxmW);
        interval->active++;
        interval->begin = packet->clock0;
        interval->end = packet->clock1;
        interval->prev = p_interval;
        interval->next = n_interval;
        return -1;
    } 

    return -1;
}


void noise_packet_cs(call_t *c, packet_t *packet) {
    uint64_t time = packet->clock0;
    noise_t *noise = get_noise_byid(c);
    noise_interval_t *interval;
    
    /* go to current */
    noise_update_current(noise, time);
    if ((noise->current) == NULL && (noise->last == NULL)) {
        interval = (noise_interval_t *) mem_fs_alloc(mem_interval);
        interval->active = 1;
        interval->begin = time;
        interval->end = packet->clock1;    
        interval->next = NULL;
        interval->prev = NULL;
        add_new_signal2noise(interval->noise, packet->channel, packet->rxmW);

        noise->first = noise->current = noise->last = interval;
        return;
    } else if( (noise->current) == NULL && (noise->last != NULL)) {
        /* successive */
        interval = (noise_interval_t *) mem_fs_alloc(mem_interval);
        interval->active = 1;
        interval->begin = time;
        interval->end = packet->clock1;    
        interval->next = NULL;
        interval->prev = noise->last;
        add_new_signal2noise(interval->noise, packet->channel, packet->rxmW);
        
        noise->last->next = interval;
        noise->last = interval;
        return;
    }

    /* update intervals */
    interval = noise->current;
    while (interval) {
        if (add_packet2interval(noise, interval, packet)) {
            return;
        }
        interval = interval->next;
    }    

    /* append a new interval */
    if (interval == NULL) {
        interval = (noise_interval_t *) mem_fs_alloc(mem_interval);
        interval->active = 1;
        interval->begin = noise->last->end;
        interval->end = packet->clock1;    
        interval->next = NULL;
        interval->prev = noise->last;
        add_new_signal2noise(interval->noise, packet->channel, packet->rxmW);
        
        noise->last->next = interval;
        noise->last = interval;
    }
}


/* ************************************************** */
/* ************************************************** */
static inline uint64_t min(uint64_t a, uint64_t b) {
    return (a < b ? a : b);
}

#define AVG_NOISE

void update_noise(noise_interval_t *interval, nodeid_t node, packet_t *packet, uint64_t *f_begin, 
                  uint64_t *f_end, uint64_t *f_duration, int *f_current) {
    if (interval->begin > (*f_begin)) {
        
        /* interval begins in this frame: update for min((*f_end), interval->end) - interval->begin */
#ifdef AVG_NOISE
        packet->noise_mW[(*f_current)] += (interval->noise[packet->channel] - correlation[packet->channel][packet->channel] * packet->rxmW) 
            * ((double) min((*f_end), interval->end) - interval->begin) / ((double) (*f_duration));
#else /*AVG_NOISE*/
        packet->noise_mW[(*f_current)] = MAX((interval->noise[packet->channel] - correlation[packet->channel][packet->channel] * packet->rxmW),  packet->noise_mW[(*f_current)]);
#endif /*AVG_NOISE             */
    } else if (interval->begin == (*f_begin)) {

        /* interval begins with this frame: update for min((*f_end), interval->end) - interval->begin */
#ifdef AVG_NOISE
        packet->noise_mW[(*f_current)] += (interval->noise[packet->channel] - correlation[packet->channel][packet->channel] * packet->rxmW) 
            * ((double) min((*f_end), interval->end) - interval->begin) / ((double) (*f_duration));
#else /*AVG_NOISE*/
        packet->noise_mW[(*f_current)] = MAX((interval->noise[packet->channel] - correlation[packet->channel][packet->channel] * packet->rxmW), packet->noise_mW[(*f_current)]);
#endif /*AVG_NOISE*/

        /* add white/statistical noise, modulate, go to next frame and next interval */
        if (noise_entity) {
            packet->noise_mW[(*f_current)] += get_white_noise(node, packet->channel);
        }
        packet->ber[(*f_current)] = do_modulate(packet->modulation, packet->rxmW, packet->noise_mW[(*f_current)]);
        packet->PER *= pow((1 - packet->ber[(*f_current)]), (*f_duration) / packet->Tb);
        (*f_current)--;
        (*f_end) = (*f_begin);
        if ((*f_current) == 0) {
            (*f_begin) = packet->clock0;
            (*f_duration) = (*f_end) - (*f_begin);
            packet->noise_mW[(*f_current)] = 0;
        } else if ((*f_current) > 0) {
            (*f_begin) = (*f_end) - (*f_duration);
            packet->noise_mW[(*f_current)] = 0;
        }

    } else if (interval->begin < (*f_begin)) {
        /* interval begins before this frame: update for min((*f_end), interval->end) - (*f_begin) */
#ifdef AVG_NOISE
        packet->noise_mW[(*f_current)] += (interval->noise[packet->channel] - correlation[packet->channel][packet->channel] * packet->rxmW) 
            * ((double) min((*f_end), interval->end) - (*f_begin)) / ((double) (*f_duration));
#else /*AVG_NOISE*/
        packet->noise_mW[(*f_current)] = MAX((interval->noise[packet->channel] - correlation[packet->channel][packet->channel] * packet->rxmW), packet->noise_mW[(*f_current)]);
#endif /*AVG_NOISE*/
        
        /* add white/statistical noise, modulate, go to next frame but keep same interval */
        if (noise_entity) {
            packet->noise_mW[(*f_current)] += get_white_noise(node, packet->channel);
        }
        packet->ber[(*f_current)] = do_modulate(packet->modulation, packet->rxmW,  packet->noise_mW[(*f_current)]);
        packet->PER *= pow((1 - packet->ber[(*f_current)]), (*f_duration) / packet->Tb);
        (*f_current)--;
        (*f_end) = (*f_begin);
        if ((*f_current) == 0) {
            (*f_begin) = packet->clock0;
            (*f_duration) = (*f_end) - (*f_begin);
            packet->noise_mW[(*f_current)] = 0;
        } else if ((*f_current) > 0) {
            (*f_begin) = (*f_end) - (*f_duration);
            packet->noise_mW[(*f_current)] = 0;
        }
        
        update_noise(interval, node, packet, f_begin, f_end, f_duration, f_current);
    }
}

void noise_packet_rx(call_t *c, packet_t *packet) {
    noise_t *noise = get_noise_byid(c);
    noise_interval_t *interval;
    uint64_t f_begin;
    uint64_t f_end;
    uint64_t f_duration;
    int f_current; 

    /* set frame informations */
    f_end = packet->clock1;
#if (SNR_STEP > 0)
    f_duration = packet->duration / SNR_STEP;
    f_current = min(ceil(packet->real_size/8), SNR_STEP - 1);
#elif (SNR_STEP < 0)
    f_duration = packet->duration / ceil(packet->real_size/8);
    f_current = ceil(packet->real_size/8) - 1;
#endif /*SNR_STEP*/
    if (f_current == 0) {
        f_begin = packet->clock0;
        f_duration = f_end - f_begin;
    } else {
        f_begin = f_end - f_duration;
    }
    packet->noise_mW[f_current] = 0;

    /* get last considered frame */
    noise_update_current(noise, packet->clock1);
    if (noise->current == NULL) {
        interval = noise->last;
    } else {
        interval = noise->current->prev;
    }

    /* update activiy interval */
    while (interval) {
        /* we have reached the end of the packet */
        if (interval->end <= packet->clock0) {
            return;
        }
        
        /* update interval active packets */
        interval->active--;

        /* all previous intervals should be removed */
        if (interval->active == 0) {    

            /* update first pointer */
            if ((noise->first = interval->next) == NULL) {
                noise->last = noise->current = NULL;
            } else {
                noise->first->prev = NULL;
            }
    
            /* remove intervals */
            while (interval) {
                noise_interval_t *t_interval = interval;

                /* update noise */
                if (interval->end > packet->clock0) {
                    update_noise(interval, c->node, packet, &f_begin, &f_end, &f_duration, &f_current);
                }
                
                /* destroy interval */
                interval = interval->prev;
                mem_fs_dealloc(mem_interval, t_interval);
            }

            return;
        }

        /* update noise */
        update_noise(interval, c->node, packet, &f_begin, &f_end, &f_duration, &f_current);
            
        interval = interval->prev;
    }
    
    return;
}


/* ************************************************** */
/* ************************************************** */
double medium_get_noise(call_t *c, int channel) {
    uint64_t time = get_time();
    noise_t *noise = get_noise_byid(c);
    double value = 0;

    /* deterministic noise */
    noise_update_current(noise, time);
    if (noise->current) {
        value = noise->current->noise[channel];
    } else {
        value = 0;
    }

    /* white/statistical noise */
    if (noise_entity) {
        value += get_white_noise(c->node, channel);
    }
    
    /* convert to dBm */
    return mW2dBm(value);
}


/* ************************************************** */
/* ************************************************** */
double get_white_noise(int node, int channel) {
    if (noise_entity == NULL) {
        return 0;
    } else {
        call_t c = {noise_entity->id, -1, -1};
        return noise_entity->methods->noise.noise(&c, node, channel);
    }
}
