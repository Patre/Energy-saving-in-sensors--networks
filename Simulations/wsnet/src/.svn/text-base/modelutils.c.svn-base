/**
 *  \file   modelutils.c
 *  \brief  Model utilities
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "internals.h"


/* ************************************************** */
/* ************************************************** */
void TX(call_t *c, packet_t *packet) {
    entity_t *entity = get_entity_by_id(c->entity);
    entity->methods->generic.tx(c, packet);
}

void RX(call_t *c, packet_t *packet) {
    entity_t *entity = get_entity_by_id(c->entity);
    entity->methods->generic.rx(c, packet);
}

int IOCTL(call_t *c, int option, void *in, void **out) {
    entity_t *entity = get_entity_by_id(c->entity);
    return entity->ioctl(c, option, in, out);
}

/* edit by Quentin Lampin <quentin.lampin@orange-ftgroup.com> */

int GET_HEADER_SIZE(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    return entity->methods->generic.get_header_size(c);
}

int SET_HEADER(call_t *c, packet_t *packet, destination_t *dst) {
    entity_t *entity = get_entity_by_id(c->entity);
    return entity->methods->generic.set_header(c, packet, dst);
}

 
int GET_HEADER_REAL_SIZE(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    return entity->methods->generic.get_header_real_size(c);
}

/* end of edition */
