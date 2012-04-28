
#include "Implementation.h"


int PROTOCOLE_reception(call_t *c, packet_t *packetRecu) {
    struct nodedata *nodedata=get_node_private_data(c);
    packet_PROTOCOLE *data=(packet_PROTOCOLE *) (packetRecu->data + nodedata->overhead);



    //AJOUTE de packet dans la liste de packet
    list_PACKET_insert_tout(&nodedata->paquets,data->src,data->seq,data->redirected_by);

    //
    data->redirected_by = c->node;

    entityid_t *up = get_entity_links_up(c);
    call_t c_up = {up[0], c->node};
    packet_t *packet_up;

    packet_up = packet_clone(packetRecu);
    RX(&c_up, packet_up);//*/

    //ENVOI
    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};

    if (SET_HEADER(&c0, packetRecu, &destination) == -1) {
        packet_dealloc(packetRecu);
        return -1;
    }
	
	c0.entity = c->entity;
    tx(&c0, packetRecu);

    //tout c'est bien passé
    return 1;
}

double get_range_Tr(call_t *c)
{	
    array_t *mac=get_mac_entities(c);
    call_t c0 = {mac->elts[0], c->node, c->entity};
    struct mac_data* macdata = get_node_private_data(&c0);
    return macdata->range;
}

void set_range_Tr(call_t *c,double range)
{
    array_t *mac=get_mac_entities(c);
    call_t c0 = {mac->elts[0], c->node, c->entity};
    struct mac_data* macdata = get_node_private_data(&c0);
    macdata->range = range;
}

