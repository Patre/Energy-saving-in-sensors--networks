#ifndef LBIP_RBH
#define LBIP_RBH


#include "../RBH/struct_RBH.h"
#include "../RBH/time_RBH.h"
#include "../RBH/list.h"
#include "struct_LBIP.h"

/*LES FONCTION DE CE FICHIER*/
int lbip(call_t *c, void *args);
int rx_lbip(call_t *c, packet_t *packet);

/* ************************************************** */
/*************************************************** */
//LBIP LANCEMENT
int lbip(call_t *c, void *args) {
    struct nodedata *nodedata = get_node_private_data(c);

    //augmanter le nbr d'evenement
    nodedata->nbr_evenement++;

    //L'EVENEMENT
    DEBUG;
    /*printf("Node %d (%lf %lf %lf) detecte un evenement, at %lf\n", c->node,                                 //id de Noeud
           get_node_position(c->node)->x,get_node_position(c->node)->y,get_node_position(c->node)->z,           //la postion x, y,z de Noeud
           get_time_now_second());//*/                                                                              //l'instant d'envoi.

    //ENVOI
    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};

    //destination de paquet
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};

    //creation de paquet et initialisation de son data
    packet_t *packet = packet_alloc(c, nodedata->overhead[0] + sizeof(packet_LBIP));
    packet_LBIP *data = (packet_LBIP *) (packet->data + nodedata->overhead[0]);

    //initilailser les données
    data->type=LBIP;
    data->source=data->source;
    data->seq=data->seq;
    data->redirected_by=c->node;

    //couvert node
    data->covred=Nullptr(list);
    list_insert(&data->covred,c->node);

    //envoi au voisin 1 de l'arbre
    list *voisin1_arbre=Nullptr(list);
    arbre_to_list_fils(&voisin1_arbre,nodedata->tree_LBIP);

    //copier dans distination
    data->destinations=Nullptr(list);
    list_copy(&data->destinations,voisin1_arbre);

    //envoyer aussi l'arbre pere
    data->pere_arbre=Nullptr(arbre);
    arbre_copy(&data->pere_arbre,nodedata->tree_LBIP);

    if (SET_HEADER(&c0, packet, &destination) == -1) {
            packet_dealloc(packet);
            return -1;
    }

    DEBUG;
    /*printf("Node %d (%lf %lf %lf) broadcast a evenement, at %lf\n", c->node,                                 //id de Noeud
           get_node_position(c->node)->x,get_node_position(c->node)->y,get_node_position(c->node)->z,           //la postion x, y,z de Noeud
           get_time_now_second());//*/                                                                              //l'instant d'envoi.

    //L'envoi
    TX(&c0,packet);

    //tous c'est bien passé
    return 1;
}

/***********************************************************************************************/
/***********************************************************************************************/
//RECEPTION de LBIP
int rx_lbip(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);

    packet_LBIP *data=(packet_LBIP *) (packet->data + nodedata->overhead[0]);

    //AJOUTE de packet dans la liste de packet
    list_PACKET_insert_tout(&nodedata->paquets,data->source,data->seq,data->redirected_by);

    DEBUG;
    /*printf("le noeud %d a recu de %d at %lf\n",c->node,
           data->redirected_by,
           get_time_now_second());


    //RECUPERER LES instructions (les noeuds a Qui envoyer)
    /*
      *ETAPE 1 recuperer les noueds atteint pas le pere sans lui
      */
    list *deja_couvert=Nullptr(list);
    arbre_to_list_sauf(&deja_couvert,data->pere_arbre,c->node);
    list_union(&deja_couvert,data->covred);

    DEBUG;/*affichge de resultat*/
    /*list *l=Nullptr(list);
    arbre_to_list(&l,nodedata->tree_LBIP);
    printf("neouds dans l'originale de %d ->",c->node);
    list_affiche(l);
    list *l1=Nullptr(list);
    arbre_to_list(&l1,data->pere_arbre);
    printf("neouds dans l'arbre recu sont de node %d ->",data->pere_arbre->node);
    list_affiche(l1);
    printf("le noued couvert par d'autres' est ->");
    list_affiche(deja_couvert);//*/


    /*
      *ETAPE 2 suppression de tout les noeud deja atteint de son arbre LBIP
      */
    arbre *lbip_re=Nullptr(arbre);              //contient le nouveua arbre
    arbre_copy(&lbip_re,nodedata->tree_LBIP);   //faire une copy de l'arbre originale

    arbre_moins_list(lbip_re,deja_couvert);     //supprimer de l'arbre tout les noeud deja couvert

    DEBUG;
    /*list *l=Nullptr(list);
    arbre_to_list(&l,nodedata->tree_LBIP);
    printf("neouds dans l'originale de %d ->",c->node);
    list_affiche(l);
    list *l1=Nullptr(list);
    arbre_to_list(&l1,lbip_re);
    printf("neouds pas encore couvert de %d ->",c->node);
    list_affiche(l1);
    printf("neouds  couvert d'autre' ->");
    list_affiche(deja_couvert);//*/

    //arbre_affiche(lbip_re);


    /*
      * ETAPE 3  rebroadcasté avec le nouveua arbre lbip_re
      */
    DEBUG;/*Affichage des prochain destination*/
    /*printf("**********************************************\n"
           "le noued %d recu de %d at %lf va en voyer a : \n",c->node,data->redirected_by,get_time_now_second());
    list *l=Nullptr(list);
    arbre_to_list_fils(&l,lbip_re);
    list_affiche(l);

    /*printf("L'arbre original de ce NOued est ");
    arbre_affiche(nodedata->tree_LBIP);
    printf("\n\nL'arbre apres l'elmination de ce NOued est ");
    arbre_affiche(lbip_re);//*/




    //ENVOI
    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};

    //destination de paquet
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};

    //creation de paquet et initialisation de son data
    packet_t *repacket = packet_alloc(c, nodedata->overhead[0] + sizeof(packet_LBIP));
    packet_LBIP *redata = (packet_LBIP *) (repacket->data + nodedata->overhead[0]);

    //initilailser les données
    redata->type=LBIP;
    redata->source=data->source;
    redata->seq=data->seq;
    redata->redirected_by=c->node;


    //envoi au voisin 1 de l'arbre
    list *voisin1_arbre=Nullptr(list);
    arbre_to_list_fils(&voisin1_arbre,lbip_re);

    //copier dans distination
    data->destinations=Nullptr(list);
    list_copy(&redata->destinations,voisin1_arbre);


    //envoi au voisin 1 de l'arbre
    redata->pere_arbre=Nullptr(arbre);
    arbre_copy(&redata->pere_arbre,nodedata->tree_LBIP);

    //copier les noeud deja couvert
    redata->covred=Nullptr(list);
    list_insert(&redata->covred,c->node);
    list_copy(&redata->covred,deja_couvert);

    if (SET_HEADER(&c0, repacket, &destination) == -1) {
            packet_dealloc(repacket);
            return -1;
    }

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
