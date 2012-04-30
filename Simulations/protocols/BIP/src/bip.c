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
    /*//REPLAY
    FILE *replay;
    replay=fopen("replay","w");
    fclose(replay);
	
    //GRAPH
    FILE *topo;
    topo=fopen("graphLBIP","w");
    fclose(topo);*/
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
	nodedata->g = malloc(sizeof(graphe));
	initGraphe(nodedata->g, c->node);
	nodedata->BIP_tree = 0;
	nodedata->lastIDs = malloc(get_node_count()*sizeof(int));
	int i;
	for(i = 0 ; i < get_node_count() ; i++)
	{
		nodedata->lastIDs[i] = -1;
	}
	
	
    set_node_private_data(c, nodedata);
	
    return 0;
}

int init(call_t *c, void *params) {
    struct protocoleData *entitydata = malloc(sizeof(struct protocoleData));
    param_t *param;
	
    /* init entity variables */
    entitydata->alpha   = 1;
    entitydata->c       = 0;
    entitydata->eps     = 0.01;
	entitydata->debug = 0;
	
	
    /* reading the "init" markup from the xml config file */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "alpha")) {
			if (get_param_integer(param->value, &(entitydata->alpha))) {
				goto error;
			}
        }
        if (!strcmp(param->key, "c")) {
			if (get_param_integer(param->value, &(entitydata->c))) {
				goto error;
			}
        }
		
        if (!strcmp(param->key, "eps")) {
            if (get_param_double(param->value, &(entitydata->eps))) {
                goto error;
            }
        }
		
        if (!strcmp(param->key, "debug")) {
            if (get_param_integer(param->value, &(entitydata->debug))) {
                goto error;
            }
        }
    }
	
    set_entity_private_data(c, entitydata);
	printf("bip : alpha : %d ; c : %d\n", entitydata->alpha, entitydata->c);
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
	
	init_graphe(c);
	
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
	if(!is_node_alive(c->node))
		return;
    struct nodedata *nodedata = get_node_private_data(c);
    struct protocoleData *entitydata = get_entity_private_data(c);
	array_t *up = get_entity_bindings_up(c);
	
    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
	
	switch(data->type)
	{	
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
				
				//init_bip_tree(c, data->src);
				call_t cSrc = {-1,-1,-1};
				cSrc.entity = c->entity;
				cSrc.node = data->src;
				struct nodedata *nodedataSrc = get_node_private_data(&cSrc);
				setRangeToFarestNeighbour(c, nodedata->g, nodedataSrc->BIP_tree);
				
				/*if(entitydata->debug)
				{
					printf("%d recoit un paquet, voici son arbre de racine %d :\n", c->node, data->src);
					arbre_affiche(nodedataSrc->BIP_tree);
				}*/
				array_t *down = get_entity_bindings_down(c);
				list* fils = 0;
				arbre_get_fils(&fils, nodedataSrc->BIP_tree, c->node);
				if(fils != 0)
				{
					// relayer le paquet
					destination_t dst = {-1,{-1,-1,-1}};
					data->pred = c->node;
					call_t c_down = {down->elts[0], c->node, c->entity};
					SET_HEADER(&c_down, packet, &dst);
					tx(c, packet);
				}
			}
			else
			{
				printf("Message non broadcaste pas traite ... TODO\n");
			}
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
    struct protocoleData *entitydata = get_entity_private_data(c);
    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
	
	//if(entitydata->debug)
		printf("BIP - Paquet de type %d envoye par %d avec range = %.1lf (at %lf s).\n", data->type, c->node, get_range_Tr(c), get_time_now_second());
	
	//retransmettre
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    TX(&c0,packet);
}

int set_header( call_t *c , packet_t * packet , destination_t * dst )
{
    struct nodedata *nodedata = get_node_private_data(c);
    struct protocoleData *entitydata = get_entity_private_data(c);
	
    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};
	
	init_bip_tree(c, c->node);
	if(entitydata->debug)
	{
		printf("Arbre de BIP de %d : \n", c->node);
		arbre_affiche(nodedata->BIP_tree);
	}
	
	// initialisation des donnees de routage du paquet
    packet_PROTOCOLE *header = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);

    header->type = APP;
    header->src = c->node;
	header->dst = dst->id;
	header->pred = c->node;
	header->id = last_id++;
	
	
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
	if(nodedata->BIP_tree != 0)
	{
		arbre_detruire(&nodedata->BIP_tree);
	}
	
	deleteGraphe(nodedata->g);
	free(nodedata->g);
	free(nodedata->lastIDs);
	
    free(nodedata);
    return 0;
}

routing_methods_t methods = {rx, tx, set_header, get_header_size, get_header_real_size};



