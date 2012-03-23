/**
 *  \file   BIP_Protocole.c
 *  \brief  Broadcast Incremental Protocol
 *  \author LAOUADI Rabah
 *  \date   03-2012
 **/

#include "structure/one_hop.h"
#include "BIP/Implementation.h"


/* ************************************************** */
/* ************************************************** */
/* Defining module informations*/
model_t model =  {
    "Broadcast Incremental Protocol",
    "LAOUADI Rabah",
    "0.1",
//    MODELTYPE_APPLICATION,
    MODELTYPE_ROUTING,
    {NULL, 0}
};


/***********************************************************************************************/
/***********************************************************************************************/

void init_files()
{
    //REPLAY
    FILE *replay;
    replay=fopen("replay","w");
    fclose(replay);
	
    //GRAPH
    FILE *topo;
    topo=fopen("graphBIP","w");
    fclose(topo);
	
}

//INITIALISATION DE NOEUD DE FICHIER XML
int setnode(call_t *c, void *params) {
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    int i = get_entity_links_down_nbr(c);
	
    //les voisinages
    nodedata->N1        = Nullptr(listeNodes);
    nodedata->NodesV1   = Nullptr(list2N);
	
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
	
    DEBUG;
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
    get_one_hop(c,entitydata->eps);
	
    //INITIALISATION DE L'ARBRE
    get_PROTOCOLE_init(c,entitydata->eps);
	
	
    //Commencé l'application
    /*if(c->node==0)
    {
        uint64_t at=entitydata->debut+get_random_time_range(0,entitydata->periodEVE);
        scheduler_add_callback(at, c, PROTOCOLE_appelle, NULL);
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
        if(list_recherche(data->destinations,c->node))
            PROTOCOLE_reception(c,packet);
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
    /*printf("N:%d -> ",c->node);
	 list_affiche(nodedata->N1);//*/
    //}
	
    DEBUG;/*voisinage 2 hop*/
    /*if(c->node==0)
	 {
	 list2_affiche(nodedata->NodesV1);
	 }//*/
	
	
    DEBUG; /*ARBRE DE LBIP*/
    /*if(c->node==0)
	 {     printf("ARBRE DE BIP\n");
	 arbre_affiche(nodedata->tree_BIP);}//*/
	
    DEBUG;  //PAQUETs
    printf("Node : %d ->",c->node);
    list_PACKET_affiche(nodedata->paquets);//*/
	
    //liberatio d'espace memoire
    //PAR USER PROTOCOLE
    listeNodes_detruire(&nodedata->N1);                   //1-HOP
    list2N_detruire(&nodedata->NodesV1);                  //Nodes
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

void tx( call_t *c , packet_t * packet )
{
    //DEBUG
    struct nodedata *nodedata = get_node_private_data(c);
    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead[0]);
    printf("J'envoi %d at %.2lf avec %d %d %d\n",c->node,get_time_now_second(),data->source,data->seq,data->redirected_by);

    //retresemtre
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};

    TX(&c0,packet);
}

/* *********************************************** */
int set_header( call_t *c , packet_t * packet , destination_t * dst )
{
    struct nodedata *nodedata = get_node_private_data(c);
    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead[0]);

    //augmanter le nbr d'evenement
    nodedata->nbr_evenement++;

    //remplissage de packet de Routage
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
    return SET_HEADER(&c0, packet, &destination);
}

int get_header_size( call_t * c )
{
    struct nodedata *nodedata = get_node_private_data(c);

    if (nodedata->overhead == -1)
    {
        call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
        nodedata->overhead = GET_HEADER_SIZE(&c0);
    }

    return nodedata->overhead + sizeof(packet_PROTOCOLE);
}

int get_header_real_size( call_t * c )
{
    struct nodedata *nodedata = get_node_private_data(c);

    if (nodedata->overhead == -1)
    {
        call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
        nodedata->overhead = GET_HEADER_REAL_SIZE(&c0);
    }

    return nodedata->overhead + sizeof(packet_PROTOCOLE);
}
///
routing_methods_t methods = {rx, tx, set_header, get_header_size, get_header_real_size};



//application_methods_t methods = {rx};
