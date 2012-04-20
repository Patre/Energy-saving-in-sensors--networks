/**
 *  \file   BIP_Protocole.c
 *  \brief  Broadcast Incremental Protocol
 *  \author LAOUADI Rabah
 *  \date   03-2012
 **/

#include <include/modelutils.h>

#include <time_wsnet.h>

#include "BIP_tree_utils.h"
#include "structures.h"
#include "one_hop.h"

int last_id = 0;


/* ************************************************** */
/* ************************************************** */
/* Defining module informations*/
model_t model =  {
    "Broadcast Incremental Protocol",
    "LAOUADI Rabah",
    "0.1",
    MODELTYPE_ROUTING,
    {NULL, 0}
};


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

int destroy(call_t *c) {
    return 0;
}
int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


// initialisation des noeuds a partir du fichier xml
int setnode(call_t *c, void *params) {
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
	
	nodedata->overhead = -1;
    nodedata->oneHopNeighbourhood = 0;
	nodedata->twoHopNeighbourhood = 0;
	nodedata->g2hop = malloc(sizeof(graphe));
	initGraphe(nodedata->g2hop, c->node);
	nodedata->BIP_tree = 0;
	nodedata->nbr_evenement = 0; //STATS
	nodedata->lastIDs = malloc(get_node_count()*sizeof(int));
	int i;
	for(i = 0 ; i < get_node_count() ; i++)
	{
		nodedata->lastIDs[i] = -1;
	}
	
	
    set_node_private_data(c, nodedata);
	
    DEBUG;
    SHOW_GRAPH("N: %d %lf %f\n",c->node,get_node_position(c->node)->x,get_node_position(c->node)->y);
	
//	printf("Node %d at ( %.1lf ; %.1lf ; %.1lf )\n", c->node,get_node_position(c->node)->x,get_node_position(c->node)->y,get_node_position(c->node)->z);
	
    return 0;
}

/* RECUPERATION DES PROPRIETES DE PROTOCOLE*/
int init(call_t *c, void *params) {
    struct protocoleData *entitydata = malloc(sizeof(struct protocoleData));
    param_t *param;
	
    /* init entity variables */
    entitydata->alpha   = 2;
    entitydata->c       = 0;
    entitydata->eps     = 0.01;
    //entitydata->debut   = time_seconds_to_nanos(3);
    //entitydata->periodEVE = time_seconds_to_nanos(1);
	
	
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
		
        /*if (!strcmp(param->key, "debut")) {
			if (get_param_time(param->value, &(entitydata->debut))) {
				goto error;
			}
        }
		
        if (!strcmp(param->key, "period_evnt")) {
			if (get_param_time(param->value, &(entitydata->periodEVE))) {
				goto error;
			}
        }*/
    }
	
	// init_files();
	
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
	
    init_one_hop(c,entitydata->eps);
	
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
	if(!is_node_alive(c->node))
		return;
    struct nodedata *nodedata = get_node_private_data(c);
	array_t *up = get_entity_bindings_up(c);
	
    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
	
	switch(data->type)
	{	
		case HELLO:
		{
			//printf("BIP - Paquet de type HELLO recu par %d depuis %d\n", c->node, data->src);
			rx_hello(c,packet);
			break;
		}
		case HELLO2:
		{
			//printf("BIP - Paquet de type HELLO2 recu par %d depuis %d\n", c->node, data->src);
			rx_two_hop(c,packet);
			break;
		}
		case APP:
		{
			//printf("BIP - Paquet de type APP recu par %d depuis %d ; source : %d et destine a %d\n", c->node, data->pred, data->src, data->dst);
			if(nodedata->lastIDs[data->src] == data->id || data->src == c->node)
				break;
			else
				nodedata->lastIDs[data->src] = data->id;
			if(data->dst == BROADCAST_ADDR)
			{
				
				// faire remonter le paquet a la couche application
				call_t c_up = {up->elts[0], c->node, c->entity};
				RX(&c_up, packet_clone(packet));
				
				if(listeNodes_recherche(data->askedToRedirect, c->node)) // si le paquet contient des instructions pour ce noeud
				{
                                    SHOW_GRAPH("G: %d %d\n",data->pred,c->node);

					//uint64_t time = rand() % 11000000000000;
					//scheduler_add_callback(time, c, forward, packet);
					forward(c, packet);
				}
			}
                        /*else
			{
				printf("Message non broadcaste pas traite ... TODO\n");
                        }*/
			//packet_dealloc(packet);
			break;
		}
		default:
			printf("%d a recu un packet de type %d NON reconnu\n", c->node, data->type);
			break;
	}
}

void tx( call_t *c , packet_t * packet )
{
    struct nodedata *nodedata = get_node_private_data(c);
    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
	
	
    //printf("BIP - Paquet de type %d envoye de %d a %d (at %lf s).\n", data->type, data->src, data->dst, get_time_now_second());
	
	//retransmettre
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    TX(&c0,packet);
}

int set_header( call_t *c , packet_t * packet , destination_t * dst )
{
    struct nodedata *nodedata = get_node_private_data(c);
	
    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};
	
	
	// initialisation des donnees de routage du paquet
    packet_PROTOCOLE *header = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);

    header->type = APP;
    header->src = c->node;
	header->dst = dst->id;
	header->pred = c->node;
	header->askedToRedirect = 0;
	header->needsToBeCovered = 0;
	header->id = last_id++;
	
	if(dst->id == BROADCAST_ADDR)
	{
		if(nodedata->BIP_tree == 0) // le BIP tree n'a pas encore ete construit
		{
			/*printf("Graphe de 2-voisinage de %d :\n", c->node);
			afficherGraphe(nodedata->g2hop);*/
			nodedata->BIP_tree = computeBIPtree(c, nodedata->g2hop, 0, 0, 0);
		}
		setRangeToFarestNeighbour(c, nodedata->g2hop, nodedata->BIP_tree);
		setRelayNodes(c, nodedata->g2hop, nodedata->BIP_tree, &header->askedToRedirect, &header->needsToBeCovered, c->node);
	}
	else
	{
		// TODO
	}
	
    if (SET_HEADER(&c0, packet, dst) == -1) {
		packet_dealloc(packet);
		return -1;
    }
	return 0;
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

//LA FIN DE LA SUMULATION
int unsetnode(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
        //printf("Unset node %d\n",c->node);
	



    listeNodes_detruire(&nodedata->oneHopNeighbourhood);
    listeNodes_detruire(&nodedata->twoHopNeighbourhood);

    DEBUG; /* ARBRE DE LBIP */
	if(nodedata->BIP_tree != 0)
	{
		arbre_detruire(&nodedata->BIP_tree);
	}
	
	DEBUG; /* Graphe DE LBIP */
	deleteGraphe(nodedata->g2hop);
	free(nodedata->g2hop);
	free(nodedata->lastIDs);
	
    free(nodedata);
    return 0;
}

routing_methods_t methods = {rx, tx, set_header, get_header_size, get_header_real_size};



