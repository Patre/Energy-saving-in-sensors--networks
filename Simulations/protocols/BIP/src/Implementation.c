
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
	
    //initilailser les données
    data->type=BIP;
    data->src=c->node;
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
    struct protocoleData *entitydata = get_entity_private_data(c);

    packet_PROTOCOLE *data=(packet_PROTOCOLE *) (packetRecu->data + nodedata->overhead);

	
    //AJOUTE de packet dans la liste de packet
    list_PACKET_insert_tout(&nodedata->paquets,data->src,data->seq,data->redirected_by);

    if(data->seq==1)
    {
        setRangeToFarestNeighbour(c,nodedata->g2hop,data->pere_arbre);
    }

    SHOW_GRAPH("G: %d %d\n",data->redirected_by,c->node);

    entityid_t *up = get_entity_links_up(c);
    call_t c_up = {up[0], c->node};
    packet_t *packet_up;

    packet_up = packet_clone(packetRecu);

    RX(&c_up, packet_up);//*/

    //initialiser les données
    list *destinations=Nullptr(list);
    arbre_get_fils(&destinations,data->pere_arbre,c->node);
	
    //copier dans distination
    data->destinations=Nullptr(list);
    list_copy(&data->destinations,destinations);

    data->redirected_by=c->node;

    if(list_taille(data->destinations)!=0)
    {
        if(entitydata->debug)
        {
            printf("BIP SUIVRE - ON %d TO",c->node);
            list_affiche(data->destinations);
        }
        //ENVOI
        entityid_t *down = get_entity_links_down(c);
        call_t c0 = {down[0], c->node};

        //destination de paquet

        destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};
        if (SET_HEADER(&c0, packetRecu, &destination) == -1) {
            packet_dealloc(packetRecu);
            return;
        }


        //L'envoi
        TX(&c0,packetRecu);//*/
    }

    //tout c'est bien passé
    return 1;
}
