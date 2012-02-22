#ifndef LBIP_RBH
#define LBIP_RBH


#include "../RBH/struct_RBH.h"
#include "../RBH/time_RBH.h"
#include "../RBH/list.h"
#include "struct_LBIP.h"

/*LES FONCTION DE CE FICHIER*/
int lbip(call_t *c, void *args);
int rx_lbip(call_t *c, packet_t *packet);
int lbip_rebroadcast(call_t *c, void *args);

/* ************************************************** */
/* ************************************************** */
//LBIP LANCEMENT
int lbip(call_t *c, void *args) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydataLBIP *entitydata=get_entity_private_data(c);


    //augmanter le nbr d'evenement
    nodedata->nbr_evenement++;

    //L'EVENEMENT
    DEBUG;
    printf("Node %d (%lf %lf %lf) detecte un evenement, at %lf\n", c->node,                                 //id de Noeud
           get_node_position(c->node)->x,get_node_position(c->node)->y,get_node_position(c->node)->z,           //la postion x, y,z de Noeud
           get_time_now_second());//*/                                                                              //l'instant d'envoi.

    //ENVOI

}

/***********************************************************************************************/
/***********************************************************************************************/
//RECEPTION de LBIP
int rx_lbip(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydataLBIP *entitydata=get_entity_private_data(c);


    //RECEPTION  DE PACKET HELLO
    packet_dealloc(packet);

}


/***********************************************************************************************/
/***********************************************************************************************/
// REBRODCAST le pakcet apres TIME OUT
int lbip_rebroadcast(call_t *c, void *args) {
    struct nodedata *nodedata = get_node_private_data(c);

    //recuperer le dernier packet
    packet_LBIP last_packet=nodedata->paquets->packet;

    DEBUG;
    /*printf("je suis %d rebrodcast le message de %d (Seq=%d) redirected by %d at %lf\n",c->node,
           last_packet.source,
           last_packet.seq,
           last_packet.redirected_by,
           get_time_now_second());//*/



}
#endif //LBIP_RBH
