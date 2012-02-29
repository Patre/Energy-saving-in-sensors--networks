#ifndef TWO_HOP_RBH
#define TWO_HOP_RBH

#include "../MPR/struct_MPR.h"
#include "time_RBH.h"
#include <include/modelutils.h>
//calcul de vosinage a two hop
void get_two_hop(call_t *c, double eps);
int init_two_hop(call_t *c, void *args);
int rx_two_hope(call_t *c, packet_t *packet);
void rx_two_hope_reponse(call_t *c, packet_t *packet);


/***********************************************************************************************/
/***********************************************************************************************/
// LANCEMENT DE L'APLLICATION
void get_two_hop(call_t *c, double eps)
{
    int count=get_node_count();
    uint64_t fin_one_hop=count*time_seconds_to_nanos(eps)+time_seconds_to_nanos(eps);
    uint64_t at=fin_one_hop + c->node*time_seconds_to_nanos(eps);
    scheduler_add_callback(at, c, init_two_hop, NULL);
}

/***********************************************************************************************/
/***********************************************************************************************/
// ENVOI DE PACKET HELLO
int init_two_hop(call_t *c, void *args) {
    struct nodedata *nodedata = get_node_private_data(c);

    DEBUG;
    //printf("lancemend de processus two hop Node:%d at %lf\n",c->node,get_time_now_second());

    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};

    //destination de paquet
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};

    //creation de paquet et initialisation de son data
    packet_t *packet = packet_alloc(c, nodedata->overhead[0] + sizeof(struct packet_hello));
    struct packet_hello *hello = (struct packet_hello *) (packet->data + nodedata->overhead[0]);

    //initilailser les données
    hello->type=HELLO2;
    hello->source=c->node;

    if (SET_HEADER(&c0, packet, &destination) == -1) {
            packet_dealloc(packet);
            return -1;
    }

    DEBUG;
    /*printf("Node %d (%lf %lf %lf) broadcast a packet hello, at %lf\n", c->node,                                 //id de Noeud
           get_node_position(c->node)->x,get_node_position(c->node)->y,get_node_position(c->node)->z,           //la postion x, y,z de Noeud
           get_time_now_second());//*/                                                                              //l'instant d'envoi.

    //L'envoi
    TX(&c0,packet);

    //tous c'est bien passé
    return 1;
}



/***********************************************************************************************/
/***********************************************************************************************/
//RECEPTION et REPONDRE AU PACKET HELLO
int rx_two_hop(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);

    //RECEPTION  DE PACKET HELLO
    struct packet_hello *hello = (struct packet_hello *) (packet->data + nodedata->overhead[0]);

    DEBUG;
    /*printf("NOde %d a recu un HELLO2 de %d at %lf\n",c->node,
           hello->source,
           get_time_now_second());//*/




    //REPONSE DE PAKET HELLO

    //recuperer le support de communication MAC
     entityid_t *down = get_entity_links_down(c);
     call_t c0 = {down[0], c->node};

     //destination de paquet
     destination_t destination = {hello->source, {get_node_position(hello->source)->x,get_node_position(hello->source)->y,get_node_position(hello->source)->z}};

     //creation de paquet et initialisation de son data
     packet_t *rpacket = packet_alloc(c, nodedata->overhead[0] + sizeof(struct packet_hello2));
     struct packet_hello2 *rhello = (struct packet_hello2 *) (rpacket->data + nodedata->overhead[0]);

     //initialiser les données
     rhello->type     =   REP_HELLO2;
     rhello->source   =   c->node;
     rhello->N1       =   Nullptr(list);
     list_copy(&rhello->N1,nodedata->N1);

     if (SET_HEADER(&c0, rpacket, &destination) == -1) {
         packet_dealloc(rpacket);
         return -1;
     }

     DEBUG;
     /*printf("Node %d (%lf %lf %lf) repond a %d  packet hello, at %lf\n", c->node,                               //id de Noeud de noeud encours
                get_node_position(c->node)->x,get_node_position(c->node)->y,get_node_position(c->node)->z,           //la postion x, y,z de Noeud
                hello->source,                                                                                       //id de noued de destination
                get_time_now_second()); //*/                                                                             //l'instant d'envoi.

    //L'envoi
    TX(&c0,rpacket);

    //liberer le packet
    packet_dealloc(packet);

    //tous c'est bien passé
    return 1;
}


/***********************************************************************************************/
/***********************************************************************************************/
//RECEPTION DE LA REPONSE DE PACKET HELLO
void rx_two_hop_reponse(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);

    struct packet_hello2 *data = (struct packet_hello2 *) (packet->data + nodedata->overhead[0]);


    DEBUG;
    /*printf("je suis le noeud %d j'ai recu la REPONSE de %d avec ",c->node,
            data->source);
    list_affiche(data->N1);//*/

    list_delete(&data->N1,c->node);
    list2_insert(&nodedata->N2,data->source,data->N1);

    //l'ajoute de voisin
    /*if(!list_recherche(nodedata->N1,data->source))
        list_insert(&nodedata->N1,data->source);*/

    //liberer le packet
    packet_dealloc(packet);

}

#endif //TWO_HOP_RBH

