
#include "Implementation.h"

/* ************************************************** */
/*************************************************** */
//LANCEMENT
int PROTOCOLE_appelle(call_t *c, packet_t * packetUP) {
    struct nodedata *nodedata=get_node_private_data(c);
    struct protocoleData *entitydata = get_entity_private_data(c);

    //augmanter le nbr d'evenement
    nodedata->nbr_evenement++;

    /*
         Creation de Packet
     */

    //creation de paquet et initialisation de son data
    packet_t *packet = packet_create(c, nodedata->overhead + sizeof(packet_PROTOCOLE), -1);
    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);

    //remplissage de data
    data->type=LBOP;
    data->src=c->node;
    data->seq=nodedata->nbr_evenement;
    data->redirected_by=c->node;

    //ENVOI
    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};

    //destination de paquet
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};
    if (SET_HEADER(&c0, packet, &destination) == -1) {
        packet_dealloc(packet);
        return -1;
    }


//	printf("BIP - Paquet de type %d envoye de %d a %d.\n", data->type, c->node, destination.id);
    //L'envoi
    TX(&c0,packet);//*/

    //Prochaine evenement
    uint64_t at=get_time_next(entitydata->debut,entitydata->periodEVE,get_time_now());
    scheduler_add_callback(at, c, PROTOCOLE_appelle, NULL);//*/

    //tous c'est bien passé
    return 1;
}


/***********************************************************************************************/
/***********************************************************************************************/
//RECEPTION
int PROTOCOLE_reception(call_t *c, packet_t *packetRecu) {
    struct nodedata *nodedata=get_node_private_data(c);
    packet_PROTOCOLE *data=(packet_PROTOCOLE *) (packetRecu->data + nodedata->overhead);


    //AJOUTE de packet dans la liste de packet
    list_PACKET_insert_tout(&nodedata->paquets,data->src,data->seq,data->redirected_by);


    entityid_t *up = get_entity_links_up(c);
    call_t c_up = {up[0], c->node};
    packet_t *packet_up;

    packet_up = packet_clone(packetRecu);
    RX(&c_up, packet_up);//*/


    //changer les destination
    data->redirected_by=c->node;
    data->destinations=Nullptr(listeNodes);
    listeNodes_copy(&data->destinations,nodedata->oneHopNeighbourhood);//*/

    //ENVOI
    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};

    if (SET_HEADER(&c0, packetRecu, &destination) == -1) {
        packet_dealloc(packetRecu);
        return -1;
    }
	
    TX(&c0,packetRecu);

    //tout c'est bien passé
    return 1;
}

double get_range_Tr(call_t *c)
{
    array_t *mac=get_mac_entities(c);
    call_t c0 = {mac->elts[0], c->node, c->entity};
    struct mac_data* macdata = get_entity_private_data(&c0);
    return macdata->range;
}

void set_range_Tr(call_t *c,double range)
{
    array_t *mac=get_mac_entities(c);
    call_t c0 = {mac->elts[0], c->node, c->entity};
    struct mac_data* macdata = get_entity_private_data(&c0);
    macdata->range = ceil(range);
}

