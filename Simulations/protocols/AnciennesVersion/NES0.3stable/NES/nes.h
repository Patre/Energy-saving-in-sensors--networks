#ifndef NES_RBH
#define NES_RBH


#include "../RBH/struct_RBH.h"
#include "../RBH/time_RBH.h"
#include "../RBH/list.h"
#include "struct_NES.h"

/*LES FONCTION DE CE FICHIER*/
int nes(call_t *c, void *args);
int nes_rebroadcast(call_t *c, void *args);

/* ************************************************** */
/* ************************************************** */
//NES LANCEMENT
int nes(call_t *c, void *args) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydataNES *entitydata=get_entity_private_data(c);

    //augmanter le nbr d'evenement
    nodedata->nbr_evenement++;

    //L'EVENEMENT
    DEBUG;
    printf("Node %d (%lf %lf %lf) detecte un evenement, at %lf\n", c->node,                                 //id de Noeud
           get_node_position(c->node)->x,get_node_position(c->node)->y,get_node_position(c->node)->z,           //la postion x, y,z de Noeud
           get_time_now_second());//*/                                                                              //l'instant d'envoi.

    //ENVOI
    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};

    //ENVOYER AU VOISIN
    list *p=nodedata->N1;
    while(p)
    {
        //creation de paquet et initialisation de son data
        packet_t *packet = packet_alloc(c, nodedata->overhead[0] + sizeof(struct packet_nes));
        struct packet_nes *data_nes = (struct packet_nes *) (packet->data + nodedata->overhead[0]);


        //initilailser les données
        data_nes->type=NES;
        data_nes->source=c->node;
        data_nes->seq=nodedata->nbr_evenement;
        data_nes->redirected_by=c->node;
        data_nes->N1    = Nullptr(list);
        list_copy(&data_nes->N1,nodedata->N1);

        //recuper le voisin
        int node_voisin=p->val;
        p=p->suiv;

        //destination de paquet
        destination_t destination = {node_voisin, {get_node_position(node_voisin)->x,
                                     get_node_position(node_voisin)->y,
                                     get_node_position(node_voisin)->z}};

        if (SET_HEADER(&c0, packet, &destination) == -1) {
                packet_dealloc(packet);
                return -1;
        }

        DEBUG;
        /*printf("Node %d send to %d, at %lf\n", c->node,                                 //id de Noeud
                node_voisin,
               get_time_now_second());//*/                                                                              //l'instant d'envoi.

        //L'envoi
        TX(&c0,packet);
    }


    //LE CALCULE de PROCHAINE EVENEMENT
    //PROCHAINE EVENEMENT
    uint64_t at=get_time_next(entitydata->debut,entitydata->periodEVE,get_time_now());
    scheduler_add_callback(at, c, nes, NULL);//*/

}

/***********************************************************************************************/
/***********************************************************************************************/
//RECEPTION de NES
int rx_nes(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydataNES *entitydata=get_entity_private_data(c);


    //RECEPTION  DE PACKET HELLO
    struct packet_nes *data = (struct packet_nes *) (packet->data + nodedata->overhead[0]);

    if(!list_NES_recherche(nodedata->paquets,*data))
    {
        //copier le voisnage
        list_copy(&nodedata->Voi_NES,nodedata->N1);

        //il recu le message
        nodedata->first_time=true;


        //enregister le paquet
        list_NES_insert(&nodedata->paquets,*data);

        //calcule de timeout
        uint64_t at=get_time_now()+get_random_time_range(entitydata->timeoutMin,entitydata->timeoutMax);

        //rebrodacast apres timeout
        scheduler_add_callback(at, c, nes_rebroadcast, NULL);
    }

    DEBUG; //affichage des voisin
    //list_affiche(data->N1);
    //list_affiche(nodedata->Voi_NES);

    //suppresion des voisin de node source
    list_intersection(&nodedata->Voi_NES,data->N1);
    list_delete(&nodedata->Voi_NES,data->redirected_by);

    DEBUG;//affichage apres supression
    //list_affiche(nodedata->Voi_NES);

    DEBUG;
    //printf("Node %d a recu de %d la seq %d\n",c->node, data->source,data->seq);

    //liberer le packet
    packet_dealloc(packet);

}


/***********************************************************************************************/
/***********************************************************************************************/
// REBRODCAST le pakcet apres TIME OUT
int nes_rebroadcast(call_t *c, void *args) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydataNES *entitydata=get_entity_private_data(c);

    //recuperer le dernier packet
    packet_NES last_packet=nodedata->paquets->packet;

    DEBUG;
    /*printf("je suis %d rebrodcast le message de %d (Seq=%d) redirected by %d at %lf\n",c->node,
           last_packet.source,
           last_packet.seq,
           last_packet.redirected_by,
           get_time_now_second());//*/


    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};

    list *p=nodedata->Voi_NES;
    while(p)
    {
        //creation de paquet et initialisation de son data
        packet_t *packet = packet_alloc(c, nodedata->overhead[0] + sizeof(struct packet_nes));
        struct packet_nes *data_nes = (struct packet_nes *) (packet->data + nodedata->overhead[0]);


        //initilailser les données
        data_nes->type      =   NES;
        data_nes->source    =   last_packet.source;
        data_nes->seq       =   last_packet.seq;
        data_nes->redirected_by =   c->node;


        //recuper le voisin
        int node_voisin=p->val;
        p=p->suiv;

        //destination de paquet
        destination_t destination = {node_voisin, {get_node_position(node_voisin)->x,
                                     get_node_position(node_voisin)->y,
                                     get_node_position(node_voisin)->z}};

        if (SET_HEADER(&c0, packet, &destination) == -1) {
                packet_dealloc(packet);
                return -1;
        }

        DEBUG;
        /*printf("Node %d send to %d, at %lf\n", c->node,                                 //id de Noeud
                node_voisin,
               get_time_now_second());//*/                                                                              //l'instant d'envoi.

        //L'envoi
        TX(&c0,packet);
    }

    //reinialiser les parmaetres
    if(nodedata->Voi_NES)   list_detruire(&nodedata->Voi_NES);
    nodedata->first_time=false;

}




#endif //NES_RBH