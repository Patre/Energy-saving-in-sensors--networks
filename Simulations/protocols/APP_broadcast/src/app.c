/**
 *  \file   app.c
 *  \brief  une application pour les protocole de broadcast
 *  \author LAOUADI Rabah
 *  \date   mars 2012
 **/
#include <stdio.h>
#include <include/modelutils.h>

#include <time_wsnet.h>
#include <list_paquet.h>
/* ************************************************** */
/* ************************************************** */

/* Defining module informations*/
model_t model =  {
    "Broadcast Application",
    "LAOUADI RABAH",
    "0.1",
    MODELTYPE_APPLICATION, 
    {NULL, 0}
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
    int debug ;
    uint64_t  debut;           //l'instant de debut de l'application (detection de premier evenement
    uint64_t  periodEVE;       //delta temps entre chaque evenement
};


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


/* ************************************************** */
/* ************************************************** */

/* Here we read the entity variables from the xml config file*/
int init(call_t *c, void *params) {
  struct entitydata *entitydata = malloc(sizeof(struct entitydata));
  param_t *param;

  /* init entity variables */
  entitydata->debug = 0;
  entitydata->debut   = time_seconds_to_nanos(3);
  entitydata->periodEVE = time_seconds_to_nanos(1);

  /* reading the "init" markup from the xml config file */
  das_init_traverse(params);
  while ((param = (param_t *) das_traverse(params)) != NULL) {
      if (!strcmp(param->key, "debut")) {
                      if (get_param_time(param->value, &(entitydata->debut))) {
                              goto error;
                      }
      }

      if (!strcmp(param->key, "period")) {
          if (get_param_time(param->value, &(entitydata->periodEVE))) {
              goto error;
          }
      }
      if (!strcmp(param->key, "debug")) {
          if (get_param_integer(param->value, &(entitydata->debug))) {
              goto error;
          }
      }
  }

  /*if(c->node == 0)
  {
      get_node_position(c)->x=50;
      get_node_position(c)->y=50;
      get_node_position(c)->z=0;
  }*/

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
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata =get_entity_private_data(c);

    //DEBUG PAQUETs
    if(entitydata->debug)
    {
        printf("\tPaquets (APP): %d ->",c->node);
        list_PACKET_affiche(nodedata->paquets);//*/
    }



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
    
    /* if the node type is source, we schedule a new callback */
    if(c->node==0)
        scheduler_add_callback(entitydata->debut, c, callmeback, NULL);//*/

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

    /* broadcast a new data packet */
    packet_t *packet = packet_create(c, nodedata->overhead[0] + sizeof(struct packet_header), -1);
    struct packet_header *header = (struct packet_header *) (packet->data + nodedata->overhead[0]);

    header->source = c->node;
    header->seq    = nodedata->nbr_evenement++;

    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};
	
    if(entitydata->debug)printf("APP - broadcast paquet depuis (%d,%d) at %.2lf\n", header->source,header->seq , get_time_now_second());

    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};
    if (SET_HEADER(&c0, packet, &destination) == -1) {
            packet_dealloc(packet);
	    return -1;
    }

    TX(&c0, packet);

    // we schedule a new callback after actualtime+period
    if(c->node == 0)
    {
           uint64_t  at= get_time_next(entitydata->debut,entitydata->periodEVE,get_time_now());
           scheduler_add_callback(at, c, callmeback, NULL);

    }
    return 0;
}



/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct packet_header *header = (struct packet_header *) (packet->data + nodedata->overhead[0]);


    //printf("APP - paquet recu par %d depuis %d at %.2lf. Contenu : %d\n", c->node, header->source, get_time_now_second(), header->seq);
    list_PACKET_insert_tout(&nodedata->paquets,header->source,header->seq,packet->node);

    packet_dealloc(packet);
}


/* ************************************************** */
/* ************************************************** */
application_methods_t methods = {rx};
