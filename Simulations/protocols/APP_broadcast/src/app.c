/**
 *  \file   app.c
 *  \brief  une application pour les protocole de broadcast
 *  \author LAOUADI Rabah
 *  \date   mars 2012
 **/
#include <stdio.h>
#include <include/modelutils.h>
#include <graphe.h>
#include <time_wsnet.h>
#include <list_paquet.h>

/* ************************************************** */
/* ************************************************** */
/*#define ENDERR(x...)  { FILE *topo; topo=fopen("ERREUR","w+"); fprintf(topo,x); fclose(topo);}
#define END(x...)  { FILE *topo; topo=fopen("END","w+"); fprintf(topo,x); fclose(topo);}
#define RAYON(x...)  { FILE *topo; topo=fopen("rayon","a+"); fprintf(topo,x); fclose(topo);}
#define PR(x...)  { FILE *topo; topo=fopen("lifetime","a+"); fprintf(topo,x); fclose(topo);}*/


/* Defining module informations*/
model_t model =  {
    "Broadcast Application",
    "LAOUADI RABAH",
    "0.1",
    MODELTYPE_APPLICATION, 
    {NULL, 0}
};

struct macnodedata {
    void *buffer;
    double range;
#ifdef ONE_PACKET_AT_A_TIME
    int scheduler;
#endif
};




/* ************************************************** */
/* ************************************************** */

/* Node private data */
struct nodedata {
  int *overhead;

  //Liste of packet Recus
  list_PACKET *paquets;

  //Nombre d'evenement
  int nbr_evenement;
	
};

/* ************************************************** */
/* ************************************************** */
/* VARIABLE de L'APPLICATION LBIP */

/* Entity private data */
struct entitydata {
    int debug;
	int period;
    uint64_t  debut;           //l'instant de debut de l'application (detection de premier evenement
	FILE* broadcastingNodes;
};

/* ************************************************** */
/* ************************************************** */

double get_range_Tr(call_t *c)
{
    array_t *mac=get_mac_entities(c);
    call_t c0 = {mac->elts[0], c->node, c->entity};
    struct macnodedata* macdata = get_node_private_data(&c0);
    return macdata->range;
}

double min(double x,double y)
{
    if(x>y)
        return y;
    else
        return x;

}

/* ************************************************** */
/* ************************************************** */

/* Data Packet header */
struct packet_header {
  int  source;
  int  seq;
};

/* ************************************************** */
/* ************************************************** */
int callmeback(call_t *c, void *args);
int verfier(call_t *c, void *args);


/* ************************************************** */
/* ************************************************** */

/* Here we read the entity variables from the xml config file*/
int init(call_t *c, void *params) {
  struct entitydata *entitydata = malloc(sizeof(struct entitydata));
  param_t *param;

  /* init entity variables */
  entitydata->debug = 0;
	entitydata->debut = time_seconds_to_nanos(3);
	entitydata->period = time_seconds_to_nanos(2);

  /* reading the "init" markup from the xml config file */
  das_init_traverse(params);
  while ((param = (param_t *) das_traverse(params)) != NULL) {
      if (!strcmp(param->key, "debut")) {
                      if (get_param_time(param->value, &(entitydata->debut))) {
                              goto error;
                      }
      }
      if (!strcmp(param->key, "debug")) {
          if (get_param_integer(param->value, &(entitydata->debug))) {
              goto error;
          }
      }
      if (!strcmp(param->key, "period")) {
          if (get_param_integer(param->value, &(entitydata->period))) {
              goto error;
          }
		  entitydata->period = time_seconds_to_nanos(entitydata->period);
      }
  }
	char topopath[1024];
	if(getenv("TOPOPATH") != NULL)
		strcpy(topopath, getenv("TOPOPATH"));
	else
		strcpy(topopath, ".");
	strcat(topopath, "/topologie.txt");
	printf("topopath : %s\n", topopath);
	entitydata->broadcastingNodes = fopen(topopath, "r");
	if(entitydata->broadcastingNodes == NULL)
	{
		printf("Fichier de topologie manquant.\n");
		end_simulation();
	}

  set_entity_private_data(c, entitydata);
  return 0;


error:
    free(entitydata);
    return -1;

}

int destroy(call_t *c) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */

/* Here we read the node variables from the xml config file*/
int setnode(call_t *c, void *params) {
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    int i = get_entity_links_down_nbr(c);

    /* default values */
    nodedata->nbr_evenement = 0;
    nodedata->paquets = Nullptr(list_PACKET);


    /* alloc overhead memory */
    if (i) {
        nodedata->overhead = malloc(sizeof(int) * i);
    } else {
        nodedata->overhead = NULL;
    }
    
    set_node_private_data(c, nodedata);
    return 0;

 }

int unsetnode(call_t *c) {
	//printf("Unset node app %d\n",c->node);
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata =get_entity_private_data(c);

    //printf("%d %d \n",c->node,list_PACKET_taille(nodedata->paquets));
    //DEBUG PAQUETs
    /*if(entitydata->debug)
    {
        printf("\tPaquets (APP): %d ->",c->node);
        list_PACKET_affiche(nodedata->paquets);
    }*/
	if(entitydata->debug)
		printf("Nombre de paquets recus par %d : %d\n", c->node, list_PACKET_taille(nodedata->paquets));

    list_PACKET_detruire(&nodedata->paquets);
    if (nodedata->overhead) {
        free(nodedata->overhead);
    }
    free(nodedata);
	
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata =get_entity_private_data(c);

    int i = get_entity_links_down_nbr(c);
    entityid_t *down = get_entity_links_down(c);
  
    while (i--) {
        call_t c0 = {down[i], c->node, c->entity};
        
        if ((get_entity_type(&c0) != MODELTYPE_ROUTING) 
            && (get_entity_type(&c0) != MODELTYPE_MAC)) {
            nodedata->overhead[i] = 0;
        } else {
            nodedata->overhead[i] = GET_HEADER_SIZE(&c0);
        }
    }
	
	if(c->node == 0)
	{
		call_t c0 = {c->entity,-1,c->from};
		int node;
		float density;
		fscanf(entitydata->broadcastingNodes, "%d", &node);
		fscanf(entitydata->broadcastingNodes, "%f", &density);
		
		if(fscanf(entitydata->broadcastingNodes, "%d", &node) != EOF)
		{
			c0.node = node;
			scheduler_add_callback(entitydata->debut, &c0, callmeback, NULL);
		}
	}
	
    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int callmeback(call_t *c, void *args) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata =get_entity_private_data(c);
	
	call_t c_callback = {c->entity,-1,c->from};
	int node = -1;
	
	/*do{
		if(fscanf(entitydata->broadcastingNodes, "%d", &node) != EOF)
		{
			c_callback.node = node;
		}
	} while(node == -1 || !is_node_alive(node));
	if(node != -1)
		scheduler_add_callback(get_time_now()+entitydata->period, &c_callback, callmeback, NULL);*/
	
	
    /* broadcast a new data packet */
    packet_t *packet = packet_create(c, nodedata->overhead[0] + sizeof(struct packet_header), -1);
    struct packet_header *header = (struct packet_header *) (packet->data + nodedata->overhead[0]);

    nodedata->nbr_evenement++;
    header->source = c->node;
    header->seq    = nodedata->nbr_evenement;

    list_PACKET_insert_tout(&nodedata->paquets,header->source,header->seq,c->node);

    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};
	
    if(entitydata->debug)
        printf("\nAPP - broadcast paquet %d depuis %d at %.2lf\n", header->seq, header->source , get_time_now_second());

    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};
    if (SET_HEADER(&c0, packet, &destination) == -1) {
            packet_dealloc(packet);
	    return -1;
    }

    TX(&c0, packet);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    if(!is_node_alive(c->node))
        return;
    struct nodedata *nodedata = get_node_private_data(c);
    struct packet_header *header = (struct packet_header *) (packet->data + nodedata->overhead[0]);
    struct entitydata *entitydata =get_entity_private_data(c);
	
	/*if(entitydata->debug)
		printf("APP - paquet %d recu par %d depuis %d at %.2lf\n", header->seq, c->node,header->source , get_time_now_second());*/
    list_PACKET_insert_tout(&nodedata->paquets,header->source,header->seq,packet->node);
	

    packet_dealloc(packet);
}


/* ************************************************** */
/* ************************************************** */
application_methods_t methods = {rx};


