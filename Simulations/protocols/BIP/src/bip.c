/**
 *  \file   BIP_Protocole.c
 *  \brief  Broadcast Incremental Protocol
 *  \author LAOUADI Rabah
 *  \date   03-2012
 **/

#include "Implementation.h"


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
	
    nodedata->overhead = -1;
    //les voisinages
	
    //Les abre de LBIP
    nodedata->tree_BIP   = Nullptr(arbre);
    
    //les packets
    nodedata->paquets   = Nullptr(list_PACKET);
	
    //Chloé
    nodedata->g2hop = malloc(sizeof(graphe));
    initGraphe(nodedata->g2hop, c->node);


    //arbre
    //arbre_add_pere(&nodedata->tree_BIP,c->node);
    //STATS
    nodedata->nbr_evenement = 0;

    set_node_private_data(c, nodedata);
	
    DEBUG;
    SHOW_GRAPH("N: %d %lf %f\n",c->node,get_node_position(c->node)->x,get_node_position(c->node)->y);//*/
	
    return 0;
}

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
		

        if (!strcmp(param->key, "eps")) {
            if (get_param_double(param->value, &(entitydata->eps))) {
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
    struct protocoleData *entitydata = get_entity_private_data(c);
	
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
	/* get mac header overhead */
    nodedata->overhead = GET_HEADER_SIZE(&c0);

    //printf("TWO");
    //INITIALISATION DE L'ARBRE
    get_PROTOCOLE_init(c,entitydata->eps);




   return 0;
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);

    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
	

    switch(data->type)
    {
    case BIP:
    {
        PROTOCOLE_reception(c,packet);
        break;
    }
    default:
            printf("J'ai recu un packet de type %d NON reconnu\n", data->type);
            break;
    }
}

/* ************************************************** */
/* ************************************************** */
//LA FIN DE LA SUMULATION
int unsetnode(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);

    //PAR USER PROTOCOLE
    deleteGraphe(nodedata->g2hop);
    list_PACKET_detruire(&nodedata->paquets);               //packets
    arbre_detruire(&nodedata->tree_BIP);                    //BIP tree
	
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
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));


    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};


    TX(&c0,packet);

    //DEBUG
    /*struct nodedata *nodedata = get_node_private_data(c);
    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);



    list *des=data->destinations;
    while (des)
    {
         //retransmettre
        entityid_t *down = get_entity_links_down(c);
        call_t c0 = {down[0], c->node};

        packet_t *packetEnvoi = packet_clone(packet);

        destination_t destination = {des->val, *get_node_position(des->val)};
        if (SET_HEADER(&c0, packetEnvoi, &destination) == -1) {
            packet_dealloc(packetEnvoi);
            return;
        }
     //   printf("BIP - Paquet de type %d envoye de %d a %d\n", data->type, c->node, des->val);

        SHOW_GRAPH("G: %d %d\n",c->node,des->val);

        TX(&c0,packetEnvoi);
        des=des->suiv;
    }//*/
    //Librer le paquet de l'application de dessus
     //packet_dealloc(packet);
}

/* *********************************************** */
int set_header( call_t *c , packet_t * packet , destination_t * dst )
{
    struct nodedata *nodedata = get_node_private_data(c);
    packet_PROTOCOLE *header = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

    if(nodedata->tree_BIP == Nullptr(arbre)) // le BIP tree n'a pas encore ete construit
    {
		purgeGrapheOfStables(nodedata->g2hop);
        afficherGraphe(nodedata->g2hop);
        printf("\t\t\tje calcule l'arbre de %d\n",c->node);
        nodedata->tree_BIP = computeBIPtree(c, nodedata->g2hop);
        printf("arbre de BIP de %d construit : \n", c->node);
        arbre_affiche(nodedata->tree_BIP);

        setRangeToFarestNeighbour(c, nodedata->g2hop, nodedata->tree_BIP);

    }

    //augmenter le nbr d'evenement
    nodedata->nbr_evenement++;

	
    //remplissage de packet de Routage
    header->type = BIP;
    header->src = c->node;
    header->seq = nodedata->nbr_evenement;
    header->redirected_by = c->node;


    //envoi au voisin 1 de l'arbre
    list *destinations=Nullptr(list);
    arbre_get_fils(&destinations,nodedata->tree_BIP,c->node);

    //copier dans distination
    header->destinations=Nullptr(list);
    list_copy(&header->destinations,destinations);

    //envoyer aussi l'arbre pere
    header->pere_arbre=Nullptr(arbre);
    arbre_copy(&header->pere_arbre,nodedata->tree_BIP);


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
