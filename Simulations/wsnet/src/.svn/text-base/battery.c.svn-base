/**
 *  \file   battery.c
 *  \brief  Battery utilities
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include "internals.h"


#define FROM_CALLER 1
#define FROM_NONE   0

static inline entity_t *energy_entity(call_t *c, call_t *c0, int from) {
    node_t   *node   = get_node_by_id(c->node);
    bundle_t *bundle = get_bundle_by_id(node->bundle);
    entity_t *entity = NULL;

    if (bundle->energy >= 0) {
	entity     = get_entity_by_id(bundle->energy);
	c0->entity = entity->id;
	c0->node   = c->node;
	c0->from   = from ? c->entity : -1;
    }
    return entity;
}


void battery_consume_idle(call_t *c, uint64_t duration) {
    call_t    c0;
    entity_t *entity = energy_entity(c, &c0, FROM_CALLER);

    if (entity)
	entity->methods->energy.consume_idle(&c0, duration);
}


void battery_consume(call_t *c, double energy) {
    call_t    c0;
    entity_t *entity = energy_entity(c, &c0, FROM_CALLER);

    if (entity)
      entity->methods->energy.consume(&c0, energy);
}


void battery_consume_rx(call_t *c, uint64_t duration) {
    call_t    c0;
    entity_t *entity = energy_entity(c, &c0, FROM_CALLER);

    if (entity)
	entity->methods->energy.consume_rx(&c0, duration);
}

void battery_consume_tx(call_t *c, uint64_t duration, double txdBm) {
    call_t    c0;
    entity_t *entity = energy_entity(c, &c0, FROM_CALLER);

    if (entity)
	entity->methods->energy.consume_tx(&c0, duration, txdBm);
}

double battery_consumed(call_t *c) {
    call_t    c0;
    entity_t *entity = energy_entity(c, &c0, FROM_NONE);
    return    entity ? entity->methods->energy.energy_consumed(&c0)
	             : -1;
}

double battery_remaining(call_t *c) {
    call_t    c0;
    entity_t *entity = energy_entity(c, &c0, FROM_NONE);
    return    entity ? entity->methods->energy.energy_remaining(&c0)
	             : -1;
}

double battery_status(call_t *c) {
    call_t    c0;
    entity_t *entity = energy_entity(c, &c0, FROM_NONE);
    return    entity ? entity->methods->energy.energy_status(&c0)
	             : -1;
}
