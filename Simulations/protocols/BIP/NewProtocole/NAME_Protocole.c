/**
 *  \file   BIP_Protocole.c
 *  \brief  Broadcast Incremental Protocol
 *  \author LAOUADI Rabah
 *  \date   03-2012
 **/


#include "includes/include.h"
#include "includes/one_hop.h"
#include "BIP/Initialisation.h"
#include "BIP/Implementation.h"


//LES FICHIERS SOURCES
#include "includes/src/one_hop.c"
#include "BIP/src/AddedFunction.c"
#include "includes/includeSRC.h"

/* ************************************************** */
/* ************************************************** */
/* Defining module informations*/
model_t model =  {
    "Broadcast Incremental Protocol",
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

    //les voisinages
    nodedata->N1        = Nullptr(list);
    nodedata->NodesV1   = Nullptr(list2);

    //Les abre de LBIP
    nodedata->tree_BIP   = Nullptr(arbre);
    
    //les packets
    nodedata->paquets   = Nullptr(list_PACKET);

    //ajout de la racine de l'arbre
    arbre_add_pere(&nodedata->tree_BIP,c->node);

    //STATS
    nodedata->nbr_evenement = 0;



    /* alloc overhead memory */
    if (i) {
        nodedata->overhead = malloc(sizeof(int) * i);
    } else {
        nodedata->overhead = NULL;
        goto error;
    }

    set_node_private_data(c, nodedata);

    /*DEBUG;
    SHOW_GRAPH("N: %d %lf %f\n",c->node,get_node_position(c->node)->x,get_node_position(c->node)->y);//*/

    return 0;

 error:
    free(nodedata);
    return -1;
}
/* ************************************************** */
/* ************************************************** */

/* RECUPERATION DES PROPRIETE DE PROTOCOLE*/
int init(call_t *c, void *params) {
    struct protocoleData *entitydata = malloc(sizeof(struct protocoleData));
    param_t *param;

    /* init entity variables */
    entitydata->alpha   = 2;
    entitydata->c       = 0;
    entitydata->eps     = 0.01;
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

        if (!strcmp(param->key, "eps")) {
            if (get_param_double(param->value, &(entitydata->eps))) {
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
//    init_files();

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
    struct protocoleData *entitydata = get_entity_private_data(c);

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
    /*get_one_hop(c,entitydata->eps);

    //INITIALISATION DE L'ARBRE
    get_PROTOCOLE_init(c,entitydata->eps);


    //Commencé l'application
    if(c->node==0)
    {
        uint64_t at=entitydata->debut+get_random_time_range(0,entitydata->periodEVE);
        scheduler_add_callback(at, c, bip, NULL);
    }//*/

    return 0;
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);

    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead[0]);

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

    /*******************************LBIP***************************/
    else if(data->type == BIP)
    {
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
    //if(c->node==0){printf("VOISINAGE A un HOP\n");//}
    //printf("N:%d -> ",c->node);
    //list_affiche(nodedata->N1);//*/
    //}

    DEBUG;/*voisinage 2 hop*/
    /*if(c->node==0)
    {
        list2_affiche(nodedata->NodesV1);
    }//*/


    DEBUG; /*ARBRE DE LBIP*/
    /*if(c->node==0)
    {     printf("ARBRE DE BIP\n");
    arbre_affiche(nodedata->tree_LBIP);}//*/

    DEBUG;  //PAQUETs
    /*printf("Node : %d ->",c->node);
    list_PACKET_affiche(nodedata->paquets);//*/

    //liberatio d'espace memoire
    //PAR USER PROTOCOLE
    list_detruire(&nodedata->N1);                   //1-HOP
    list2_detruire(&nodedata->NodesV1);                  //Nodes
    arbre_detruire(&nodedata->tree_BIP);           //BIP tree

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
