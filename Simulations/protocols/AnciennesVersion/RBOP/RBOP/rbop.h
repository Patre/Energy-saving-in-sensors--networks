#ifndef RBOP_RBH
#define RBOP_RBH


#include "../RBH/struct_RBH.h"
#include "../RBH/time_RBH.h"
#include "../RBH/list.h"
#include "struct_RBOP.h"
#include "init_RBOP.h"

/*LES FONCTION DE CE FICHIER*/
int rbop(call_t *c, void *args);
int rx_rbop(call_t *c, packet_t *packet);

/* ************************************************** */
/*************************************************** */
//LBIP LANCEMENT
int rbop(call_t *c, void *args) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydataRBOP *entitydata =get_entity_private_data(c);

    //augmanter le nbr d'evenement
    nodedata->nbr_evenement++;


    //ENVOI
    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};

    //destination de paquet
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};

    //creation de paquet et initialisation de son data
    packet_t *packet = packet_alloc(c, nodedata->overhead[0] + sizeof(packet_RBOP));
    packet_RBOP *data = (packet_RBOP *) (packet->data + nodedata->overhead[0]);

    //initilailser les données
    data->type=RBOP;
    data->source=data->source;
    data->seq=nodedata->nbr_evenement;
    data->redirected_by=c->node;

    //copier dans distination
    data->destinations=Nullptr(list);
    list_copy(&data->destinations,nodedata->RNG);


    if (SET_HEADER(&c0, packet, &destination) == -1) {
            packet_dealloc(packet);
            return -1;
    }

    DEBUG;
    list *destinations=Nullptr(list);
    list_copy(&destinations,nodedata->RNG);
    if(data->seq==1)while(destinations){SHOW_GRAPH("G: %d %d\n",c->node,destinations->val);destinations=destinations->suiv;}

    //L'envoi
    TX(&c0,packet);

    //LE CALCULE de PROCHAINE EVENEMENT
    //PROCHAINE EVENEMENT
    uint64_t at=get_time_next(entitydata->debut,entitydata->periodEVE,get_time_now());
    scheduler_add_callback(at, c, rbop, NULL);//*/


    //tous c'est bien passé
    return 1;
}

/***********************************************************************************************/
/***********************************************************************************************/
//RECEPTION
int rx_rbop(call_t *c, packet_t *packet) {

    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydataRBOP *entitydata =get_entity_private_data(c);

    packet_RBOP *data=(packet_RBOP *) (packet->data + nodedata->overhead[0]);

    //AJOUTE de packet dans la liste de packet
    list_PACKET_insert_tout(&nodedata->paquets,data->source,data->seq,data->redirected_by);
    //ENVOI
    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};

    //destination de paquet
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};

    //creation de paquet et initialisation de son data
    packet_t *repacket = packet_alloc(c, nodedata->overhead[0] + sizeof(packet_RBOP));
    packet_RBOP *redata = (packet_RBOP *) (repacket->data + nodedata->overhead[0]);

    //initilailser les données
    redata->type=RBOP;
    redata->source=data->source;
    redata->seq=data->seq;
    redata->redirected_by=c->node;


    //copier la destination
    redata->destinations=Nullptr(list);
    list_copy(&redata->destinations,nodedata->RNG);

    if (SET_HEADER(&c0, repacket, &destination) == -1) {
            packet_dealloc(repacket);
            return -1;
    }

    DEBUG;
    list *destinations=Nullptr(list);
    list_copy(&destinations,nodedata->RNG);
    if(data->seq==1)while(destinations){SHOW_GRAPH("G: %d %d\n",c->node,destinations->val);destinations=destinations->suiv;}

    //L'envoi
    TX(&c0,repacket);//*/



    //Suppression de packet
    //arbre_detruire(&lbip_re);
    //arbre_detruire(&data->pere_arbre);
    //packet_dealloc(packet);


    //tous c'est bien passé
    return 1;
}

#endif //LBIP_RBH
