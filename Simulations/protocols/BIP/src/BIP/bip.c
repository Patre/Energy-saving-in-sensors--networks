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
	
	nodedata->overhead = -1;
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
	
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
	/* get mac header overhead */
    nodedata->overhead = GET_HEADER_SIZE(&c0);
	
	
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
	array_t *up = get_entity_bindings_up(c);
	int i;
	
    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
	
	printf("BIP - Paquet de type %d recu par %d depuis %d et pour ", data->type, c->node, data->source);
	//list_affiche(data->destinations);
	printf("\n");
	
    //printf("je suis %d J'ai recu un packet de type %d de %d\n",c->node, data->type, packet->node);
	
    /*******************************HELLO 1 vosinage***************************/
	switch(data->type)
	{	
		case HELLO:
		{
			rx_one_hop(c,packet);
			break;
		}
		case REP_HELLO:
		{
			rx_one_hop_reponse(c,packet);
			break;
		}
		case BIP:
		{
			if(list_recherche(data->destinations,c->node))
				PROTOCOLE_reception(c,packet);
			break;
		}
		case APP:
		{
			// forward ???
			
			call_t c_up = {up->elts[i], c->node, c->entity};
			packet_t *packet_up;	     
			if (i > 0) 
			{
				packet_up = packet_clone(packet);         
			}
			else 
			{
				packet_up = packet;
			}
			RX(&c_up, packet_up);
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
	
	
	
    //liberation d'espace memoire
    //PAR USER PROTOCOLE
	list_PACKET_detruire(&nodedata->paquets);
    listeNodes_detruire(&nodedata->N1);                   //1-HOP
    list2N_detruire(&nodedata->NodesV1);                  //Nodes
    arbre_detruire(&nodedata->tree_BIP);           //BIP tree
	
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
    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
	
	
	printf("BIP - Paquet de type %d envoye de %d a ", data->type, data->source);
	list_affiche(data->destinations);
   // printf("J'envoi %d at %.2lf avec %d %d %d type %d\n",c->node,get_time_now_second(),data->source,data->seq,data->redirected_by, data->type);

    //retransmettre
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

    TX(&c0,packet);
}

/* *********************************************** */
int set_header( call_t *c , packet_t * packet , destination_t * dst )
{
    struct nodedata *nodedata = get_node_private_data(c);
    packet_PROTOCOLE *header = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
	destination_t destination;

    //augmenter le nbr d'evenement
    nodedata->nbr_evenement++;

	
    //remplissage de packet de Routage
    header->type=APP;
    header->source=c->node;
    header->seq=nodedata->nbr_evenement;
    header->redirected_by=c->node;


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
	destination.id = BROADCAST_ADDR;
	destination.position.x = -1;
	destination.position.y = -1;
	destination.position.z = -1;
	
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
