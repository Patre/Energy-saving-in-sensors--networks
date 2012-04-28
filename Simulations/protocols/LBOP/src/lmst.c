#include "lmst.h"
/***********************************************************************************************/
/***********************************************************************************************/
// LANCEMENT DE L'APPLICATION

void get_lmst(call_t *c, double eps)
{
    uint64_t fin_intial= (get_node_count()+2)*time_seconds_to_nanos(eps);
    uint64_t at=fin_intial+c->node*time_seconds_to_nanos(eps);
    scheduler_add_callback(at, c, broadcast_lmst, NULL);
}

/***********************************************************************************************/
/***********************************************************************************************/
// ENVOI DE PACKET HELLO
int broadcast_lmst(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct protocoleData *entitydata=get_entity_private_data(c);
    /*if(entitydata->debug)
        DBG("LBOP - LMST ON %d AT %lf\n",c->node,get_time_now_second());*/

    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};

    //destination de paquet
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};

    //creation de paquet et initialisation de son data
    packet_t *packet = packet_create(c, nodedata->overhead + sizeof(packet_lmst), -1);
    packet_lmst *hello = (packet_lmst *) (packet->data + nodedata->overhead);


    //initialiser les donnees
    hello->type=LMST;
    hello->src=c->node;
    hello->MST = Nullptr(list);
    list_copy(&hello->MST,nodedata->LMST_intial);


    if (SET_HEADER(&c0, packet, &destination) == -1) {
                packet_dealloc(packet);
                return -1;
    }
	
	c0.entity = c->entity;
    tx(&c0,packet);

    //tous c'est bien passé
    return 1;
}
int rx_lmst(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct protocoleData *entitydata = get_entity_private_data(c);

    packet_lmst *lmstPacket = (packet_lmst *) (packet->data + nodedata->overhead);

    // ajout du voisin dans la liste du 1-voisinage
    /*DEBUG;
    printf("LMST - %d from %d with ",c->node,lmstPacket->src);
    list_affiche(lmstPacket->MST);//*/

    if(list_recherche(nodedata->LMST_intial,lmstPacket->src))
        list_insert(&nodedata->LMST_voisin,lmstPacket->src);

    packet_dealloc(packet);

    return 1;

}
