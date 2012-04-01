/**
 *  \file   modulation.c
 *  \brief  Modulation management
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include "internals.h"


/* ************************************************** */
/* ************************************************** */
void *modulation_entities = NULL;


/* ************************************************** */
/* ************************************************** */
int modulation_init(void) {
    return 0;
}

void modulation_clean(void) {
    das_destroy(modulation_entities);
}


/* ************************************************** */
/* ************************************************** */
int modulation_bootstrap(void) {
    entity_t *entity;
    
    das_init_traverse(modulation_entities);
    while ((entity = (entity_t *) das_traverse(modulation_entities)) != NULL) {
        call_t c = {entity->id, -1, -1};
        
        if (entity->model->type != MODELTYPE_MODULATION) {
            fprintf(stderr, "entity %s is not a modulation model\n", entity->name);
            return -1;
        }
        
        if (entity->bootstrap) {
            entity->bootstrap(&c);
        }
    }
    
    return 0;
}

/* ************************************************** */
/* ************************************************** */
double do_modulate(entityid_t modulation, double rxmW, double noise) {
    entity_t *entity = get_entity_by_id(modulation);
    call_t    c      = {modulation, -1, -1};
    double    snr    = noise ? (rxmW / noise) : MAX_SNR;
    return entity->methods->modulation.modulate(&c, snr);
}


/* ************************************************** */
/* ************************************************** */

/* edit by Christophe Savigny <christophe.savigny@insa-lyon.fr> */
int modulation_bit_per_symbol(entityid_t modulation){
    entity_t *entity = get_entity_by_id(modulation);
    call_t    c      = {modulation, -1, -1};
    return entity->methods->modulation.bit_per_symbol(&c);
}
/* end of edition */

/* ************************************************** */
/* ************************************************** */
void modulation_errors(packet_t *packet) {
    /* TODO */
}
