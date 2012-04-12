/**
 *  \file   BIP_Protocole.c
 *  \brief  Broadcast Incremental Protocol
 *  \author LAOUADI Rabah
 *  \date   03-2012
 **/


#include <one_hop.h>
#include "Implementation.h"
#include <lmst.h>


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
    topo=fopen("graphLBOP","w");
    fclose(topo);
	
}

//INITIALISATION DE NOEUD DE FICHIER XML
int setnode(call_t *c, void *params) {
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
	
    nodedata->overhead = -1;
    //les packets
    nodedata->paquets   = Nullptr(list_PACKET);

    //les voisinages
    nodedata->oneHopNeighbourhood = Nullptr(listeNodes);

    nodedata->LMST_voisin = Nullptr(list);
    nodedata->LMST_intial =Nullptr(list);
    nodedata->range = -1;
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
    entitydata->debut   = time_seconds_to_nanos(10);
    entitydata->periodEVE = time_seconds_to_nanos(10);
	
	
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
    //RECUPERER LE VOSINAGE a UN SAUT
    get_one_hop(c,entitydata->eps);


    //INITIALISATION DE L'ARBRE
    get_PROTOCOLE_init(c,entitydata->eps);


    //LMST elimination
    get_lmst(c,entitydata->eps);

    /*if(c->node==0)
    {
        uint64_t at=entitydata->debut;
        scheduler_add_callback(at, c, PROTOCOLE_appelle, NULL);
    }//*/


   return 0;
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);

    struct packet_general *packetType= (struct packet_general *)(packet->data + nodedata->overhead);
    /*******************************HELLO 1 voisinage***************************/
        switch(packetType->type)
	{	
        case HELLO:
        {
                rx_hello(c,packet);
                break;
        }

        case LMST:
        {
            packet_lmst *lmstPacket = (packet_lmst *) (packet->data + nodedata->overhead);
            if(list_recherche(lmstPacket->MST,c->node))
                rx_lmst(c,packet);
            break;
        }//*/

        case LBOP:
        {
            packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
            if(list_recherche(data->destinations,c->node))
                    {
                        if(list_PACKET_recherche_tout(nodedata->paquets,data->src,data->seq)==0)
                             PROTOCOLE_reception(c,packet);
                    }

            break;
        }
        default:
                printf("J'ai recu un packet de type %d NON reconnu\n", packetType->type);
                break;
	}
}

/* ************************************************** */
/* ************************************************** */
//LA FIN DE LA SUMULATION
int unsetnode(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);

    DEBUG; // Voisinage 1 hop
      /*printf("\t1-voisinage de %d : ", c->node);
      listeNodes_affiche(nodedata->oneHopNeighbourhood);
      //printf("\t\t\t");list_affiche(nodedata->LMST_voisin);//*/
    

      /*printf("\tPaquets : %d ->",c->node);
      list_PACKET_affiche(nodedata->paquets);//*/

	
    DEBUG;//LMSt
    /*printf("***********\nlist initial %d ",c->node);
    list_affiche(nodedata->LMST_intial);
    printf("list voisin %d ",c->node);
    list_affiche(nodedata->LMST_voisin);//*/


	
    DEBUG; /*ARBRE DE LBIP*/
    /*if(c->node==0)
    {
        printf("\tARBRE DE BIP : \n");
        arbre_affiche(nodedata->tree_BIP);
    }*/
	
    DEBUG;  //GRAPH

    //liberation d'espace memoire
    //PAR USER PROTOCOLE
    list_PACKET_detruire(&nodedata->paquets);               //packets
    listeNodes_detruire(&nodedata->oneHopNeighbourhood);                     //1-HOP
    list_detruire(&nodedata->LMST_intial);
    list_detruire(&nodedata->LMST_voisin);
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
    struct nodedata *nodedata = get_node_private_data(c);
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};

    TX(&c0,packet);
}

/* *********************************************** */
int set_header( call_t *c , packet_t * packet , destination_t * dst )
{
    struct nodedata *nodedata = get_node_private_data(c);
    packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
    //augmenter le nbr d'evenement
    nodedata->nbr_evenement++;

    //Fixé le rayon
    if(nodedata->range<0)
    {
        listeNodes *tmp=nodedata->oneHopNeighbourhood;
        position_t pos1 = *get_node_position(c->node);
        double distMax = 0;
        while(tmp)
        {
            if(list_recherche(nodedata->LMST_voisin,tmp->values.node))
            {
                position_t pos2={tmp->values.x,tmp->values.y,tmp->values.z};
                double dist=distance(&pos1,&pos2);
                if(distMax<dist)    distMax=dist;
            }
            tmp=tmp->suiv;
        }
        set_range_Tr(c,distMax);
        nodedata->range=get_range_Tr(c);
        printf("Le range est fixé a %.2lf\n",get_range_Tr(c));
    }

    //remplissage de data
    data->type=LBOP;
    data->src=c->node;
    data->src_pos=*get_node_position(c->node);
    data->seq=nodedata->nbr_evenement;
    data->redirected_by=c->node;

    data->destinations=Nullptr(list);
    list_copy(&data->destinations,nodedata->LMST_voisin);

    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};

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
