
#include "Implementation.h"


int PROTOCOLE_reception(call_t *c, packet_t *packetRecu) {
    struct nodedata *nodedata=get_node_private_data(c);
    packet_PROTOCOLE *data=(packet_PROTOCOLE *) (packetRecu->data + nodedata->overhead);
    struct protocoleData *entitydata=get_entity_private_data(c);
    if(nodedata->range<0)
    {

        listeNodes *tmp=nodedata->oneHopNeighbourhood;
        position_t pos1 = *get_node_position(c->node);
        double distMax = 0;
        while(tmp)
        {
            if(list_recherche(nodedata->RNG,tmp->values.node))
            {
                position_t pos2={tmp->values.x,tmp->values.y,tmp->values.z};
                double dist=distance(&pos1,&pos2);
                if(distMax<dist)    distMax=dist;
            }
            tmp=tmp->suiv;
        }
        set_range_Tr(c,distMax);
        nodedata->range=get_range_Tr(c);
        if(entitydata->debug)
            DBG("RBOP  - %d FIXE RANGE TO %.2lf  \n",c->node,get_range_Tr(c));

    }

    //AJOUTE de packet dans la liste de packet
    list_PACKET_insert_tout(&nodedata->paquets,data->src,data->seq,data->redirected_by);


    //SHOW_GRAPH("G: %d %d\n",data->redirected_by,c->node);

    entityid_t *up = get_entity_links_up(c);
    call_t c_up = {up[0], c->node};
    packet_t *packet_up;

    packet_up = packet_clone(packetRecu);

    RX(&c_up, packet_up);//*/


    //changer les destination
    data->redirected_by=c->node;

    data->destinations=Nullptr(list);
    list_copy(&data->destinations,nodedata->RNG);



    //ENVOI
    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};

    //destination de paquet
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};
    if (SET_HEADER(&c0, packetRecu, &destination) == -1) {
        packet_dealloc(packetRecu);
        return -1;
    }
	
	c0.entity = c->entity;
    tx(&c0,packetRecu);


    //tout c'est bien passé
    return 1;
}

