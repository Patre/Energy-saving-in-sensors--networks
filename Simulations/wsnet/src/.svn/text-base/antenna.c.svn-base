/**
 *  \file   antenna.c
 *  \brief  Antenna utilities
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include "internals.h"


/* ************************************************** */
/* ************************************************** */
double antenna_get_loss(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_ANTENNA)
        return 0;
    return entity->methods->antenna.get_loss(c);
}

angle_t *antenna_get_angle(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_ANTENNA)
        return NULL;
    return entity->methods->antenna.get_angle(c);
}

void antenna_set_angle(call_t *c, angle_t *angle) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_ANTENNA)
        return;
    entity->methods->antenna.set_angle(c, angle);
}

void antenna_rx(call_t *c, packet_t *packet) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_ANTENNA)
        return;
    entity->methods->antenna.rx(c, packet);
}

double  antenna_gain_tx(call_t *c, position_t *pos) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_ANTENNA)
        return 0;
    return entity->methods->antenna.gain_tx(c, pos);
}

double  antenna_gain_rx(call_t *c, position_t *pos) {
    entity_t *entity = get_entity_by_id(c->entity);
    if (entity->model->type != MODELTYPE_ANTENNA)
        return 0;
    return entity->methods->antenna.gain_rx(c, pos);
}


