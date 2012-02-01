/**
 *  \file   radio.c
 *  \brief  Radio functions
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include "internals.h"


/* ************************************************** */
/* ************************************************** */
double radio_get_noise(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return MIN_DBM;
    return entity->methods->radio.get_noise(c);    
}

double radio_get_cs(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return MIN_DBM;
    return entity->methods->radio.get_cs(c);    
}

double radio_get_power(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return MIN_DBM;
    return entity->methods->radio.get_power(c);    
}

void radio_set_power(call_t *c, double power) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return;
    entity->methods->radio.set_power(c, power);    
}

int radio_get_channel(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return 0;
    return entity->methods->radio.get_channel(c);    
}

void radio_set_channel(call_t *c, int channel) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return;
    entity->methods->radio.set_channel(c, channel);    
}

entityid_t radio_get_modulation(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return -1;
    return entity->methods->radio.get_modulation(c);    
}

void radio_set_modulation(call_t *c, entityid_t modulation) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return;
    entity->methods->radio.set_modulation(c, modulation);    
}

uint64_t radio_get_Tb(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return 0;
    return entity->methods->radio.get_Tb(c);    
} 

void radio_set_Ts(call_t *c, uint64_t Ts) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return;
    entity->methods->radio.set_Ts(c, Ts);    
}

void radio_cs(call_t *c, packet_t *packet) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return;
    return entity->methods->radio.cs(c, packet);    
}

void radio_set_sensibility(call_t *c, double sensibility) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return;
    entity->methods->radio.set_sensibility(c, sensibility);
}

double radio_get_sensibility(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return MIN_DBM;
    return entity->methods->radio.get_sensibility(c);
}

void radio_sleep(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return;
    entity->methods->radio.sleep(c);
}

void radio_wakeup(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return;
    entity->methods->radio.wakeup(c);
}


/* edit by Christophe Savigny <christophe.savigny@insa-lyon.fr> */
int radio_get_modulation_bit_per_symbol(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_RADIO)
        return 0;
    return entity->methods->radio.get_modulation_bit_per_symbol(c);
}
/* end of edition */

