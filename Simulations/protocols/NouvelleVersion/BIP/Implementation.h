#ifndef BIP_RBH
#define BIP_RBH

#include "../includes/include.h"

#include "Implementation.h"
#include "Initialisation.h"


/*LES FONCTION DE CE FICHIER*/
int PROTOCOLE_appelle(call_t *c, void *args);
int PROTOCOLE_reception(call_t *c, packet_t *packetRecu);


/* ************************************************** */
/*************************************************** */
//LANCEMENT
int PROTOCOLE_appelle(call_t *c, void *args) {
    struct nodedata *nodedata=get_node_private_data(c);
    struct protocoleData *entitydata = get_entity_private_data(c);

    //augmanter le nbr d'evenement
    nodedata->nbr_evenement++;

    /*
      Creation de Packet
      */
    //creation de paquet et initialisation de son data
    packet_t *packet = packet_alloc(c, nodedata->overhead[0] + sizeof(packet_PROTOCOLE));
    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead[0]);

    //initilailser les données
    data->type=BIP;
    data->source=c->node;
    data->seq=nodedata->nbr_evenement;
    data->redirected_by=c->node;


    //envoi au voisin 1 de l'arbre
    list *destinations=Nullptr(list);
    arbre_get_fils(&destinations,nodedata->tree_BIP,c->node);

    //copier dans distination
    data->destinations=Nullptr(list);
    list_copy(&data->destinations,destinations);

    //envoyer aussi l'arbre pere
    data->pere_arbre=Nullptr(arbre);
    arbre_copy(&data->pere_arbre,nodedata->tree_BIP);


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

    DEBUG;
    if(data->seq==1)while(destinations){SHOW_GRAPH("G: %d %d\n",c->node,destinations->val);destinations=destinations->suiv;}


    //L'envoi
    TX(&c0,packet);//*/

    //LE CALCULE de PROCHAINE EVENEMENT
    //PROCHAINE EVENEMENT
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

    packet_PROTOCOLE *dataRecu=(packet_PROTOCOLE *) (packetRecu->data + nodedata->overhead[0]);

    //AJOUTE de packet dans la liste de packet
    list_PACKET_insert_tout(&nodedata->paquets,dataRecu->source,dataRecu->seq,dataRecu->redirected_by);

    /*
      Creation de Packet
      */
    //creation de paquet et initialisation de son data
    packet_t *packet = packet_alloc(c, nodedata->overhead[0] + sizeof(packet_PROTOCOLE));
    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead[0]);

    //initilailser les données
    data->type=BIP;
    data->source=dataRecu->source;
    data->seq=dataRecu->seq;
    data->redirected_by=c->node;


    //envoi au voisin 1 de l'arbre
    list *destinations=Nullptr(list);
    arbre_get_fils(&destinations,dataRecu->pere_arbre,c->node);

    //copier dans distination
    data->destinations=Nullptr(list);
    list_copy(&data->destinations,destinations);

    //envoyer aussi l'arbre pere
    data->pere_arbre=Nullptr(arbre);
    arbre_copy(&data->pere_arbre,dataRecu->pere_arbre);


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

    DEBUG;
    if(data->seq==1)while(destinations){SHOW_GRAPH("G: %d %d\n",c->node,destinations->val);destinations=destinations->suiv;}

    //L'envoi
    TX(&c0,packet);//*/

    //Liberer l'espace memoire;
    //packet_dealloc(packetRecu);

    //tous c'est bien passé
    return 1;
}

#endif //PROTOCOLE_RBH
