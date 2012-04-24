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
#define ENDERR(x...)  { FILE *topo; topo=fopen("ERREUR","w+"); fprintf(topo,x); fclose(topo);}
#define END(x...)  { FILE *topo; topo=fopen("END","w+"); fprintf(topo,x); fclose(topo);}
#define RAYON(x...)  { FILE *topo; topo=fopen("rayon","a+"); fprintf(topo,x); fclose(topo);}
#define PR(x...)  { FILE *topo; topo=fopen("lifetime","a+"); fprintf(topo,x); fclose(topo);}


#define MAXDBL 999999999999999999999.00
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
    int nodeBroadcast;
    int debug ;
    int connexe ;
    graphe *graph;
    uint64_t  debut;           //l'instant de debut de l'application (detection de premier evenement
    uint64_t  periodEVE;       //delta temps entre chaque evenement
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

void calculeDensite(call_t *c)
{
    //NOMBRE DE NODES
    int N=get_node_count();

    double range=get_range_Tr(c);
    printf("le Range  %lf\n",range);

    //CALCULE DE DEGREE
    int Degree=0;
    int i,j,k;


    //WRL ALGO
    double W[N][N];
    double WP[N][N];

    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            W[i][j]=MAXDBL;


    double dist;
    for(i=0;i<get_node_count();i++)
        for(j=0;j<get_node_count();j++)
        {
            dist=distance(get_node_position(i),get_node_position(j));
            if(i!=j && dist<= range)
            {
                Degree++;
                W[i][j]=1;
            }
            else if(i==j)
                W[i][j]=0;
        }

    for(k=0;k<N;k++)
    {
        for(i=0;i<N;i++)
            for(j=0;j<N;j++)
                WP[i][j]=W[i][j];

        for(i=0;i<N;i++)
            for(j=0;j<N;j++)
            {
                W[i][j] = min(W[i][j],W[i][k]+W[k][j]);
            }
    }

    /*for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            printf("(%d,%d) %lf\n",i,j,W[i][j]);//*/

    //CALCUL DENSITé
    double degreeMo=(double)Degree/(double)2;
    degreeMo=degreeMo /(double)N;


    double diametre=0;
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            if(W[i][j]>diametre) diametre=W[i][j];



    double densite=degreeMo/diametre;
    printf("%lf %lf %lf\n",degreeMo,diametre,densite);

    if(diametre == MAXDBL)
    {
        printf("End Of Simulation (un node est isolé)\n");
        ENDERR("un NODE est ISOLe\n");
        //end_simulation();
        return;
    }

    END("%lf",densite);//*/

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
  entitydata->nodeBroadcast = -1;
  entitydata->debug = 0;
  entitydata->debut   = time_seconds_to_nanos(3);
  entitydata->periodEVE = time_seconds_to_nanos(1);
  entitydata->connexe = 0;

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
      if (!strcmp(param->key, "node")) {
          if (get_param_integer(param->value, &(entitydata->nodeBroadcast))) {
              goto error;
          }
      }
      if (!strcmp(param->key, "connexe")) {
          if (get_param_integer(param->value, &(entitydata->connexe))) {
              goto error;
          }
      }
  }

  FILE *por;
  por=fopen("lifetime","w");
  fclose(por);

  FILE *ray;
  ray=fopen("rayon","w");
  fclose(ray);

  //verfier la disponibilité de noued
  if(entitydata->nodeBroadcast>get_node_count())
      entitydata->nodeBroadcast = -1;

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
	printf("Unset node app %d\n",c->node);
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata =get_entity_private_data(c);



    printf("%d %d \n",c->node,list_PACKET_taille(nodedata->paquets));
    //DEBUG PAQUETs
    /*if(entitydata->debug)
    {
        printf("\tPaquets (APP): %d ->",c->node);
        list_PACKET_affiche(nodedata->paquets);//*/
    //}


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
    if(c->node==get_node_count()-1)
    {
        calculeDensite(c);
    }


    /* if the node type is source, we schedule a new callback */
    if(c->node==0 && entitydata->nodeBroadcast == -1)
    {
        call_t *inter=c;
        inter->node= get_random_integer_range(0,get_node_count()-1);
        scheduler_add_callback(entitydata->debut, inter, callmeback, NULL);//*/
    }
    else if(c->node==entitydata->nodeBroadcast)
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

    nodedata->nbr_evenement++;
    header->source = c->node;
    header->seq    = nodedata->nbr_evenement;

    list_PACKET_insert_tout(&nodedata->paquets,header->source,header->seq,c->node);

    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};
	
    if(entitydata->debug)
        printf("APP - broadcast paquet depuis (%d,%d) at %.2lf\n", header->source,header->seq , get_time_now_second());

    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};
    if (SET_HEADER(&c0, packet, &destination) == -1) {
            packet_dealloc(packet);
	    return -1;
    }

    if(entitydata->connexe)
    {
        PR("----\n");
        PR("%lf %d %d\n",get_time_now_second(),c->node,list_PACKET_taille(nodedata->paquets));
    }

    // we schedule a new callback after actualtime+period
    if(entitydata->nodeBroadcast == -1)
    {
        call_t *inter=malloc(sizeof(call_t));
        inter->entity=c->entity;
        inter->from=c->from;
        inter->node= get_random_integer_range(0,get_node_count()-1);

        while(!is_node_alive(inter->node))
            inter->node= get_random_integer_range(0,get_node_count()-1);

        uint64_t  at= get_time_now()+entitydata->periodEVE;
        scheduler_add_callback(at, inter, callmeback, NULL);
    }
    else if(c->node==entitydata->nodeBroadcast)
    {
           uint64_t  at= get_time_now()+entitydata->periodEVE;
           scheduler_add_callback(at, c, callmeback, NULL);
    }
    TX(&c0, packet);
    return 0;
}

/*int verfier(call_t *c, void *args) {

    if(!is_node_alive(c->node))
        return 0;
    struct nodedata *nodedata = get_node_private_data(c);

    printf("%d JE VERFIE at %lf",c->node,get_time_now_second());
    call_t *inter=malloc(sizeof(call_t));
    inter->entity=c->entity;
    inter->from=c->from;

    printf(" (%d %d)le dernier\n",nodedata->paquets->packet.source,nodedata->paquets->packet.seq);
    //list_PACKET_affiche(nodedata->paquets);


    int i;
    for(i=0;i<get_node_count();i++)
    {
        inter->node=i;
        struct nodedata *internodedata = get_node_private_data(inter);
        printf("%d (%d,%d)\n",i,internodedata->paquets->packet.source,internodedata->paquets->packet.seq);

        if(is_node_alive(i) && list_PACKET_recherche_tout(internodedata->paquets,c->node,nodedata->nbr_evenement)==0)
        {

            printf("%lf FORNOTRECP %d  for(%d,%d)",get_time_now_second(),i,c->node,nodedata->nbr_evenement);
            printf(" (%d,%d)\n",internodedata->paquets->packet.source,internodedata->paquets->packet.seq);
            PR("%lf FORNOTRECP %d\n",get_time_now_second(),i);
            //end_simulation();
            //return -1;
        }
    }

    return 0;
}//*/

/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    if(!is_node_alive(c->node))
        return;
    struct nodedata *nodedata = get_node_private_data(c);
    struct packet_header *header = (struct packet_header *) (packet->data + nodedata->overhead[0]);
    struct entitydata *entitydata =get_entity_private_data(c);

    list_PACKET_insert_tout(&nodedata->paquets,header->source,header->seq,packet->node);

    double range=get_range_Tr(c);
    if(range!=0)
        RAYON("%lf\n",range)
    if(entitydata->connexe)
    {
        PR("%lf %d %d\n",get_time_now_second(),c->node,list_PACKET_taille(nodedata->paquets));
    }

    packet_dealloc(packet);
}


/* ************************************************** */
/* ************************************************** */
application_methods_t methods = {rx};


