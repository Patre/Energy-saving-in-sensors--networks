#ifndef LBIP_RBH
#define LBIP_RBH


#include "../RBH/struct_RBH.h"
#include "../RBH/time_RBH.h"
#include "../RBH/list.h"
#include "struct_LBIP.h"
#include "init_LBIP.h"

/*LES FONCTION DE CE FICHIER*/
int lbip(call_t *c, void *args);
int rx_lbip(call_t *c, packet_t *packet);

/* ************************************************** */
/*************************************************** */
//LBIP LANCEMENT
int lbip(call_t *c, void *args) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydataLBIP *entitydata =get_entity_private_data(c);

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
    data->seq=nodedata->nbr_evenement;
    data->redirected_by=c->node;


    //envoi au voisin 1 de l'arbre
    list *destinations=Nullptr(list);
    arbre_get_fils(&destinations,nodedata->tree_LBIP,c->node);

    //copier dans distination
    data->destinations=Nullptr(list);
    list_copy(&data->destinations,destinations);

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

    //LE CALCULE de PROCHAINE EVENEMENT
    //PROCHAINE EVENEMENT
    uint64_t at=get_time_next(entitydata->debut,entitydata->periodEVE,get_time_now());
    scheduler_add_callback(at, c, lbip, NULL);//*/


    //tous c'est bien passé
    return 1;
}

/***********************************************************************************************/
/***********************************************************************************************/
//RECEPTION de LBIP
int rx_lbip(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydataLBIP *entitydata =get_entity_private_data(c);

    packet_LBIP *data=(packet_LBIP *) (packet->data + nodedata->overhead[0]);

    //AJOUTE de packet dans la liste de packet
    list_PACKET_insert_tout(&nodedata->paquets,data->source,data->seq,data->redirected_by);

    if(!list_recherche(nodedata->source_packet,data->redirected_by)) nodedata->firsttime=true;

    if(nodedata->firsttime)
    {
        nodedata->firsttime=false;

        //copier l'arbre data localement afin que ce traitement sera fait une seul fois
        arbre_copy(&nodedata->tree_LBIP_re,data->pere_arbre);

        //list d'element deja dans l'arbre
        list *deja=Nullptr(list);
        arbre_to_list(&deja,nodedata->tree_LBIP_re);



        //RECUPERER le connexion vers les noeuds
        listC *connect=Nullptr(listC);
        list2_to_listC(&connect,nodedata->N2);

        DEBUG;
        /*printf("les connextion traiter ->");
        list_con_affiche(connect);//*/



        listC *connectN=Nullptr(listC);
        //Suppresion de tout les connection deja traiter
        listC *parc=Nullptr(listC);
        parc=connect;
        while(parc)
        {

            if(!list_recherche(deja,parc->node1)
                    || !list_recherche(deja, parc->node2))  list_con_insert(&connectN,parc->node1,
                                                                            parc->node2,
                                                                            calcul_energie(parc->node1,parc->node2,
                                                                                           entitydata->alpha,
                                                                                           entitydata->c));
            parc=parc->suiv;
        }



        list *g=Nullptr(list);
        list2_to_list(&g,nodedata->N2);
        list_intersection(&g,deja);

        list *deb=Nullptr(list);
        listC_to_list(&deb,connectN);
        list_intersection(&deb,g);

        DEBUG;
        /*printf("les noued de l'arbre ->");
        list_affiche(deja);
        printf("le noeud Non traiter sont ->");
        list_affiche(g);
        printf("la liste de commencement ->");
        list_affiche(deb);
        printf("les connextion non traiter ->");
        list_con_affiche(connectN);
        //*/

        DEBUG;/*les elements des*/
        /*printf("les noeuds atteint  par l'arbre pere sauf les sien");
        list_affiche(lst);//*/
        /*printf("**************************************"
               "l'arbre arriver\n");
        arbre_affiche(nodedata->tree_LBIP_re);//*/

        prim_tree2(deb,&nodedata->tree_LBIP_re,connectN,g);

        /*printf("l'arbre apres\n");
        arbre_affiche(nodedata->tree_LBIP_re);
        //printf("**************************************\n");//*/

    }



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

    //RECUPERE LA DESTINATION
    list *destinations=Nullptr(list);
    arbre_get_fils(&destinations,nodedata->tree_LBIP_re,c->node);


    DEBUG;
    if(data->seq==1){printf("prochaine destinations -> ");
    list_affiche(destinations);}//*/


    //copier la destination
    redata->destinations=Nullptr(list);
    list_copy(&redata->destinations,destinations);

    //envoyer aussi l'arbre apres l'ajoute
    redata->pere_arbre=Nullptr(arbre);
    arbre_copy(&redata->pere_arbre,nodedata->tree_LBIP_re);


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
