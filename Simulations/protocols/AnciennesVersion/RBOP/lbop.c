/**
 *  \file   lbip.c
 *  \brief  Localized Broadcast Incremental Protocol
 *  \author LAOUADI Rabah
 *  \date   02-2012
 **/
#include <stdio.h>
#include "RBH/struct_RBH.h"
#include "RBH/time_RBH.h"
#include "LBOP/struct_LBOP.h"
#include "RBH/one_hop.h"
#include "LBOP/lbop.h"
#include "LBOP/init_LBOP.h"


/* ************************************************** */
/* ************************************************** */
/* Defining module informations*/
model_t model =  {
    "Localized Broadcast Incremental Protocol",
    "LAOUADI Rabah",
    "0.1",
    MODELTYPE_APPLICATION,
    {NULL, 0}
};


/***********************************************************************************************/
/***********************************************************************************************/
//INITIALISATION DE NOEUD DE FICHIER XML
int setnode(call_t *c, void *params) {
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    int i = get_entity_links_down_nbr(c);
    param_t *param;

    /* default values */
    nodedata->eps       = 0.1;

    //les voisinages
    nodedata->N1        = NULL;


    //ARBRE LMST
    nodedata->LMST       =Nullptr(arbre);

    //les packets
    nodedata->paquets   = Nullptr(list_PACKET);

    //STATS
    nodedata->packet_tx = 0;
    nodedata->packet_rx = 0;
    nodedata->nbr_evenement = 0;



    /* reading the "default" markup from the xml config file */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {

        if (!strcmp(param->key, "eps")) {
            if (get_param_double(param->value, &(nodedata->eps))) {
                goto error;
            }
        }

    }


    /* alloc overhead memory */
    if (i) {
        nodedata->overhead = malloc(sizeof(int) * i);
    } else {
        nodedata->overhead = NULL;
    }

    set_node_private_data(c, nodedata);

    DEBUG;
    SHOW_GRAPH("N: %d %lf %f\n",c->node,get_node_position(c->node)->x,get_node_position(c->node)->y);

    return 0;

 error:
    free(nodedata);
    return -1;
}
/* ************************************************** */
/* ************************************************** */

/* RECUPERATION DES PROPRIETE DE PROTOCOLE*/
int init(call_t *c, void *params) {
    struct entitydataLBOP *entitydata = malloc(sizeof(struct entitydataLBOP));
    param_t *param;

    /* init entity variables */
    entitydata->alpha   = 2;
    entitydata->c       = 0;
    entitydata->debut   = time_seconds_to_nanos(3);
    entitydata->periodEVE = time_seconds_to_nanos(1);


    /* reading the "init" markup from the xml config file */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "alpha")) {
          if (get_param_double(param->value, &(entitydata->alpha))) {
              goto error;
          }
        }
        if (!strcmp(param->key, "c")) {
          if (get_param_double(param->value, &(entitydata->c))) {
              goto error;
          }
        }

        if (!strcmp(param->key, "debut")) {
          if (get_param_time(param->value, &(entitydata->debut))) {
              goto error;
          }
        }

        if (!strcmp(param->key, "period_evnt")) {
          if (get_param_time(param->value, &(entitydata->periodEVE))) {
              goto error;
          }
        }
    }

    //INITAILISATION DES FICHIER DE SORTIES
    init_files();

    set_entity_private_data(c, entitydata);
    return 0;


error:
    free(entitydata);
    return -1;
}


/* ************************************************** */
/* ************************************************** */

int bootstrap(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydataLBOP *entitydata = get_entity_private_data(c);

    int i = get_entity_links_down_nbr(c);
    entityid_t *down = get_entity_links_down(c);

    while (i--) {
        call_t c0 = {down[i], c->node};

        if ((get_entity_type(&c0) != MODELTYPE_ROUTING)
            && (get_entity_type(&c0) != MODELTYPE_MAC)) {
            nodedata->overhead[i] = 0;
        } else {
            nodedata->overhead[i] = GET_HEADER_SIZE(&c0);
        }
    }


    //RECUPERER LE VOSINAGE a UN SAUT
    get_one_hop(c,nodedata->eps);


    //INITIALISATION DE L'ARBRE
    get_LBOP_init(c,nodedata->eps);

    //Commencé l'application
    /*if(c->node==0)
    {
        uint64_t at=entitydata->debut+get_random_time_range(0,entitydata->periodEVE);
        scheduler_add_callback(at, c, lbop, NULL);
    }//*/

    return 0;
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);

    packet_LBOP *data = (packet_LBOP *) (packet->data + nodedata->overhead[0]);

    //printf("je suis %d J'ai recu un packet de type %d de %d\n",c->node, data->type, packet->node);

    /*******************************HELLO 1 vosinage***************************/
    if(data->type == HELLO)
    {
        rx_one_hop(c,packet);
    }
    else if(data->type == REP_HELLO)
    {
        rx_one_hop_reponse(c,packet);
    }
    /*******************************RBOP***************************/
    else if(data->type == LBOP)
    {

        //S'il ya des instrucion pour lui
        if(list_recherche(data->destinations,c->node))
        {
            if(list_PACKET_recherche_tout(nodedata->paquets,data->source,data->seq))
                printf("le paquet exist deja;\n");

            //else rx_lbop(c,packet);
        }
    }

    /*******************************NON RECONNU***************************/
    else    printf("J'ai recu un packet de type %d NON reconnu\n", data->type);
}

/* ************************************************** */
/* ************************************************** */
//LA FIN DE LA SUMULATION
int unsetnode(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);

    DEBUG; /* Vosinage 1 hop*/
    //if(c->node==0){
    /*printf("N:%d -> ",c->node);
    list_affiche(nodedata->N1);//}//*/

    DEBUG;/*voisinage 2 hop*/
    /*if(c->node==0)
    {
        printf("VOISINAGE A un 2 HOP\n");
        list2_affiche(nodedata->N2);
        list *g=Nullptr(list);
        list2_to_list(&g,nodedata->N2);
        printf("TOUS ces VOISINAGE DE 1 HOPE et 2 HOP de CE NODE\n");
        list_affiche(g);
        list_detruire(&g);
    }//*/


    DEBUG; /*ARBRE DE LBIP*/
    /*if(c->node==0)
    {     printf("ARBRE DE BIP\n");
    arbre_affiche(nodedata->tree_LBIP);}//*/

    DEBUG;  //PAQUETs
  /*  printf("Node : %d ->",c->node);
    list_PACKET_affiche(nodedata->paquets);//*/

    //liberatio d'espace memoire
    //PAR USER PROTOCOLE
    list_detruire(&nodedata->N1);                   //1-HOP

    //PAR WSNET
    if (nodedata->overhead) {
        free(nodedata->overhead);
    }
    free(nodedata);
    return 0;
}

/* ************************************************** */
/* ************************************************** */
int destroy(call_t *c) {
    return 0;
}
int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}

application_methods_t methods = {rx};