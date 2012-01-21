/**
 *  \file   lbdd_sensor.c
 *  \brief  LBDD: the Line-Based Data Dissemination protocol (Elyes Ben Hamida and Guillaume Chelius, A Line-Based Data Dissemination protocol for Wireless Sensor Networks with Mobile Sink, ICC 2008)
 *  \author Elyes Ben Hamida and Guillaume Chelius
 *  \date   2008
 **/
#include <include/modelutils.h>
#include "../data_d_common.h"


/* these macros turn ON logging */

#define LOG_APPLICATION_DISSEMINATION
//#define LOG_APPLICATION_DISSEMINATION_ROUTING

#define LOG_APPLICATION_REQUEST
//#define LOG_APPLICATION_REQUEST_ROUTING

#define LOG_APPLICATION_RESPONSE
//#define LOG_APPLICATION_RESPONSE_ROUTING


/* this macro tunes the mode of the packet transmission: BROADCAST or UNICAST */
#define USE_BROADCAST_COMMUNICATIONS

/* this macro allows to check the node state during the next-hop computation */
//#define CHECK_ACTIVE_NODE


/**************************************************************************/
/**************************************************************************/

model_t model =  {
    "LBDD: the Line-Based Data Dissemination protocol",
    "Elyes Ben Hamida",
    "0.1",
    MODELTYPE_APPLICATION, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */

/* Defining global variables */

#define LBDD_HELLO_TYPE    10
#define LBDD_DATA_TYPE     11
#define LBDD_REQUEST_TYPE  12
#define LBDD_RESPONSE_TYPE 13

#define INLINE_NODE 1
#define SENSOR_NODE 3

#define DIRECTION_NONE  '0'
#define DIRECTION_UP    'U'
#define DIRECTION_DOWN  'D'


/* ************************************************** */
/* ************************************************** */

/* Defining packet types */

struct lbdd_hello_p {
  int         type;      /* packet type */
  nodeid_t    src;       /* node id */
  position_t  position;  /* node position */
};

struct lbdd_data_p {
  int type;              /* packet type */
  int n_hop;             /* nexthop ID */
  int source;            /* source ID */
  int metadata;          /* type of the disseminated data */
  int d_seq;             /* sequence number of the data */ 
  int d_value;           /* value of the disseminated data */
  int hop;               /* for stats: hop number from the source */
  uint64_t time;         /* for stats: data birth time */
};

struct lbdd_request_p {
  int type;               /* packet type */
  int n_hop;              /* nexthop ID */
  int sink;               /* sink ID */
  position_t position;    /* home node position */
  int r_seq;              /* sequence number of the query */ 
  int metadata;           /* metadata of the requested data */ 
  int d_seq;              /* sequence number of the requested data */
  char direction;         /* direction of the query */
  int group_id;           /* group id for the query propagation */
};

struct lbdd_response_p {
  int type;  
  int n_hop;
  int sink;
  int source;
  position_t position;
  int r_seq;
  int metadata;
  int d_seq;
  int d_value;
  int hop;               /* for stats: hop number from the source node */
  uint64_t time;         /* for stats: data birth time */
};

/* ************************************************** */
/* ************************************************** */

/* Defining data type for neighbors table */
struct lbdd_neighbor {
    int id;
    position_t position;                         
    uint64_t time;
};

/**************************************************************************/
/**************************************************************************/

/* Node data */
struct nodedata {
    /* Global variables*/
    int overhead;
    int type;
    int group_id;

    /* Hello protocol */
    uint64_t h_start;
    uint64_t h_timeout;	
    uint64_t h_period;	
    int      h_nbr;

    /* storage */
    void *neighbors;
    int s_seq[MAX_SOURCE];
    int r_seq[MAX_SINK];
    int d_source[MAX_METADATA];
    int d_value[MAX_METADATA];
    int d_seq[MAX_METADATA];
    int d_hop[MAX_METADATA];
    uint64_t d_time[MAX_METADATA];        

};

/* Entity data */
struct entitydata {
  /* Virtual structure parameters */
  double line_width;
  double group_size;
  int replication;
  /* stats */
  int TX_hello;
  int TX_data;
  int TX_query;
  int TX_response;
  int TX_sensor_adv;
  int TX_sensor_data;
  int TX_home_adv;
};



/**************************************************************************/
/**************************************************************************/
int hello_callback(call_t *c, void *args);
struct lbdd_neighbor *lbdd_next_hop(call_t *c, position_t *position);
struct lbdd_neighbor *lbdd_inside_next_hop(call_t *c, position_t *position);

int lbdd_is_nearest(call_t *c, position_t *position);

void rx_lbdd_hello(call_t *c, packet_t *packet);
void rx_lbdd_data(call_t *c, packet_t *packet);
void rx_lbdd_request(call_t *c, packet_t *packet);
void rx_lbdd_response(call_t *c, packet_t *packet);

void rx_source_adv(call_t *c, packet_t *packet);
void rx_source_data(call_t *c, packet_t *packet);
void rx_sink_adv(call_t *c, packet_t *packet);


void lbdd_stats(call_t *c);

/**************************************************************************/
/**************************************************************************/
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;

    position_t *area = get_topology_area();

    entitydata->line_width  = area->x / 10;
    entitydata->group_size  = area->y / 10;
    entitydata->replication = 0; 

    entitydata->TX_hello       = 0;
    entitydata->TX_data        = 0;
    entitydata->TX_query       = 0;
    entitydata->TX_response    = 0;
    entitydata->TX_sensor_adv  = 0;
    entitydata->TX_sensor_data = 0;
    entitydata->TX_home_adv    = 0;

    /* get entity parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "line-width")) {
	  if (get_param_double_range(param->value, &(entitydata->line_width), 0, area->x)) {
	      fprintf(stderr, "lbdd_sensor: error while reading the line-width parameter (%s) !\n", param->key);
	      goto error;
	  }
        }
        if (!strcmp(param->key, "group-size")) {
	  if (get_param_double_range(param->value, &(entitydata->group_size), 0, area->y)) {
	      fprintf(stderr, "lbdd_sensor: error while reading the group-size parameter (%s) !\n", param->key);
	      goto error;
	  }
        }
        if (!strcmp(param->key, "replication")) {
	  if (get_param_integer(param->value, &(entitydata->replication))) {
	      fprintf(stderr, "lbdd_sensor: error while reading the replication parameter (%s) !\n", param->key);
	      goto error;
	  }
        }
    }
  
    set_entity_private_data(c, entitydata);
    return 0;

 error:
    free(entitydata);
    return -1;    
}

int destroy(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);

    long int total = entitydata->TX_hello + entitydata->TX_data + entitydata->TX_query + entitydata->TX_response + entitydata->TX_sensor_adv + entitydata->TX_sensor_data + entitydata->TX_home_adv;

    double total_virtual = entitydata->TX_hello*0.5 + entitydata->TX_data + entitydata->TX_query + entitydata->TX_response + entitydata->TX_sensor_adv*0.5 + entitydata->TX_sensor_data + entitydata->TX_home_adv*0.5;

    long int total_octets = entitydata->TX_hello*sizeof(struct lbdd_hello_p) + entitydata->TX_data*sizeof(struct lbdd_data_p) + entitydata->TX_query*sizeof(struct lbdd_request_p) + entitydata->TX_response*sizeof(struct lbdd_response_p) + entitydata->TX_sensor_adv*sizeof(struct sensor_adv_p) + entitydata->TX_sensor_data*sizeof(struct sensor_data_p) + entitydata->TX_home_adv*sizeof(struct home_adv_p);

    printf("[ENERGY] HELLO %d DATA %d QUERY %d RESPONSE %d SENSOR_ADV %d SENSOR_DATA %d HOME_ADV %d TOTAL %ld TOTAL_VIRTUAL %lf TOTAL_OCTETS %ld\n", entitydata->TX_hello, entitydata->TX_data, entitydata->TX_query, entitydata->TX_response, entitydata->TX_sensor_adv, entitydata->TX_sensor_data, entitydata->TX_home_adv, total, total_virtual, total_octets);


    free(entitydata);
    return 0;
}


/**************************************************************************/
/**************************************************************************/
int setnode(call_t *c, void *params) {
    struct entitydata *entitydata = get_entity_private_data(c);
    struct nodedata *nodedata     = malloc(sizeof(struct nodedata));
    param_t *param;

    position_t *position = get_node_position(c->node);
    position_t *area     = get_topology_area();

    int i = MAX_METADATA;
    int j = MAX_SOURCE;
    int k = MAX_SINK;
    
    /* default values */
    nodedata->h_start   = 0;
    nodedata->h_period  = 1000000000;	
    nodedata->h_timeout = nodedata->h_period * 2.5;
    nodedata->h_nbr     = -1;
    nodedata->neighbors = das_create();

    while (i--) {
        nodedata->d_source[i] = -1;
        nodedata->d_value[i]  = -1;
        nodedata->d_seq[i]    = -1;
        nodedata->d_hop[i]    = -1;
        nodedata->d_time[i]   = -1;
    }
    while (j--) {
        nodedata->s_seq[j] = -1;        
    }
    while (k--) {
        nodedata->r_seq[k] = -1;        
    }

    /* determining node type and group id */
    if ( (position->x >= (area->x-entitydata->line_width)/2.0) && (position->x <= (area->x+entitydata->line_width)/2.0) ) {
        nodedata->type     = INLINE_NODE;
	nodedata->group_id = (int)(position->y/entitydata->group_size);
    }
    else {
        nodedata->type     = SENSOR_NODE;
        nodedata->group_id = -1;
    }

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "hello-start")) {
	  if (get_param_time(param->value, &(nodedata->h_start))) {
	        fprintf(stderr,"[LBDD] Error while reading the hello-start parameter ! (%s)\n",param->value);
                goto error;
	  }
        }
        if (!strcmp(param->key, "hello-period")) {
	  if (get_param_time(param->value, &(nodedata->h_period))) {
	        fprintf(stderr,"[LBDD] Error while reading the hello-period parameter ! (%s)\n",param->value);
                goto error;
	  }
        }
        if (!strcmp(param->key, "hello-timeout")) {
	  if (get_param_time(param->value, &(nodedata->h_timeout))) {
	        fprintf(stderr,"[LBDD] Error while reading the hello-timeout parameter ! (%s)\n",param->value);
                goto error;
	  }
        }
        if (!strcmp(param->key, "hello-nbr")) {
	  if (get_param_integer(param->value, &(nodedata->h_nbr))) {
	        fprintf(stderr,"[LBDD] Error while reading the hello-nbr parameter ! (%s)\n",param->value);
                goto error;
	  }
        }
    }

    set_node_private_data(c, nodedata);
    return 0;

 error:
    free(nodedata);
    return -1;

}

int unsetnode(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct lbdd_neighbor *neighbor;

    //lbdd_stats(c);

    while ((neighbor = (struct lbdd_neighbor *) das_pop(nodedata->neighbors)) != NULL) {
        free(neighbor);
    }
    das_destroy(nodedata->neighbors);

    free(nodedata);
    return 0;
}


/**************************************************************************/
/**************************************************************************/
int bootstrap(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};
    uint64_t schedule = get_time() + nodedata->h_start + get_random_double() * nodedata->h_period;

    /* get overhead */
    if ((get_entity_type(&c0) != MODELTYPE_ROUTING) 
        && (get_entity_type(&c0) != MODELTYPE_MAC)) {
        nodedata->overhead = 0;
    } else {
        nodedata->overhead = GET_HEADER_SIZE(&c0);
    }

    /* scheduler first hello */
    if (nodedata->h_nbr == -1 || nodedata->h_nbr > 0) {
      scheduler_add_callback(schedule, c, hello_callback, NULL);
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */

int lbdd_is_nearest(call_t *c, position_t *sink_position) {
  struct nodedata *nodedata = get_node_private_data(c);
  struct lbdd_neighbor *neighbor = NULL;
  double dist = distance(get_node_position(c->node), sink_position);

  das_init_traverse(nodedata->neighbors);    

  while ((neighbor = (struct lbdd_neighbor *) das_traverse(nodedata->neighbors)) != NULL) {
    if ((distance(&(neighbor->position), sink_position) < dist) && (is_node_alive(neighbor->id))) {
      return 0;
    }
  }
    
  return 1;
}

int is_inside_line(call_t *c, position_t *position) {
  struct entitydata *entitydata = get_entity_private_data(c);
  position_t *area     = get_topology_area();

  if ( (position->x >= (area->x-entitydata->line_width)/2.0) && (position->x <= (area->x+entitydata->line_width)/2.0)) return 1;
  else return 0;
}

struct lbdd_neighbor *lbdd_inside_next_hop(call_t *c, position_t *position) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct lbdd_neighbor *neighbor = NULL, *n_hop = NULL;
    double dist = distance(get_node_position(c->node), position);
    double d = dist;
    uint64_t clock = get_time();

    /* parse neighbors */
    das_init_traverse(nodedata->neighbors);    

    while ((neighbor = (struct lbdd_neighbor *) das_traverse(nodedata->neighbors)) != NULL) {

#ifdef CHECK_ACTIVE_NODE
      if ( !is_node_alive(neighbor->id) || (is_inside_line(c, &(neighbor->position)) == 0) 
	   || ((nodedata->h_timeout > 0) && (clock - neighbor->time) >= nodedata->h_timeout) ) {
            continue;
        }
#else
      if ( (is_inside_line(c, &(neighbor->position)) == 0) ||
	   ((nodedata->h_timeout > 0) && (clock - neighbor->time) >= nodedata->h_timeout)) {
            continue;
        }
#endif
        
        /* choose next hop */
        if ((d = distance(&(neighbor->position), position)) < dist) {
            dist = d;
            n_hop = neighbor;
        }
    }    
    
    if (n_hop == NULL) return NULL; //lbdd_next_hop(c,position);
    else return n_hop;
}


/* Greedy geographic routing => computing the nexthop */ 
struct lbdd_neighbor *lbdd_next_hop(call_t *c, position_t *position) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct lbdd_neighbor *neighbor = NULL, *n_hop = NULL;
    double dist = distance(get_node_position(c->node), position);
    double d = dist;
    uint64_t clock = get_time();

    /* parse neighbors */
    das_init_traverse(nodedata->neighbors);    

    while ((neighbor = (struct lbdd_neighbor *) das_traverse(nodedata->neighbors)) != NULL) {

#ifdef CHECK_ACTIVE_NODE
        if ( !is_node_alive(neighbor->id) || ((nodedata->h_timeout > 0) && (clock - neighbor->time) >= nodedata->h_timeout) ) {
            continue;
        }
#else
        if ((nodedata->h_timeout > 0) && (clock - neighbor->time) >= nodedata->h_timeout ) {
            continue;
        }
#endif
        
        /* choose next hop */
        if ((d = distance(&(neighbor->position), position)) < dist) {
            dist = d;
            n_hop = neighbor;
        }
    }    
    
    return n_hop;
}

int neighbor_timeout(void *data, void *arg) {
    call_t *c = (call_t *) arg;
    struct nodedata *nodedata = get_node_private_data(c);
    struct lbdd_neighbor *neighbor = (struct lbdd_neighbor *) data;

    if ((get_time() - neighbor->time) > nodedata->h_timeout) {
        return 1;
    }

    return 0;
}

/* Periodic exchange of hello packets */
int hello_callback(call_t *c, void *args) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata = get_entity_private_data(c);

    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};

    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};

    packet_t *packet = packet_create(c, nodedata->overhead + sizeof(struct lbdd_hello_p), -1);
    struct lbdd_hello_p *hello = (struct lbdd_hello_p *) (packet->data + nodedata->overhead);
    position_t *pos = get_node_position(c->node);

    /* set mac header */
    if (SET_HEADER(&c0, packet, &destination) == -1) {
        packet_dealloc(packet);
        return -1;
    }
    
    /* set header */
    hello->type       = LBDD_HELLO_TYPE;
    hello->src        = c->node;
    hello->position.x = pos->x;
    hello->position.y = pos->y;
    hello->position.z = pos->z;

    TX(&c0, packet);	   
    entitydata->TX_hello++;
    
    /* check neighbors timeout */
    if (nodedata->h_timeout > 0) {
        das_selective_delete(nodedata->neighbors, neighbor_timeout, (void *) c);
    }
    
    /* schedules hello */
    if (nodedata->h_nbr > 0) {
        nodedata->h_nbr --;
    }

    if (nodedata->h_nbr == -1 || nodedata->h_nbr > 0) {
      scheduler_add_callback(get_time() + nodedata->h_period, c, hello_callback, NULL);
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
   struct nodedata *nodedata = get_node_private_data(c);
   struct data_d_header *header = (struct data_d_header *) (packet->data + nodedata->overhead);
    
   switch (header->type) {       

   case LBDD_HELLO_TYPE:
        /* for us */
       rx_lbdd_hello(c, packet);
       break;       

   case LBDD_DATA_TYPE:
        /* for us */
       rx_lbdd_data(c, packet);
       break;       
   case LBDD_REQUEST_TYPE:
        /* for us */
       rx_lbdd_request(c, packet);
       break;       
   case LBDD_RESPONSE_TYPE:
        /* for us */
       rx_lbdd_response(c, packet);
       break;       

   case SOURCE_ADV_TYPE:
        /* for us */
        rx_source_adv(c, packet);
        break;
   case SOURCE_DATA_TYPE:
        /* for us */
        rx_source_data(c, packet);
        break;
   case SINK_ADV_TYPE:
        /* for us */
       rx_sink_adv(c, packet);
       break;

   default:
       /* not for us */
       //printf("Node %d received an unknown packet type (%d) !!!\n", c->node, header->type);
       packet_dealloc(packet);
       break;
   }
   
    return;
}


/* ************************************************** */
/* ************************************************** */
void rx_lbdd_hello(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct lbdd_hello_p *hello = (struct lbdd_hello_p *) (packet->data + nodedata->overhead);
    struct lbdd_neighbor *neighbor;

   /* check for existing neighbors */
    das_init_traverse(nodedata->neighbors);      
    while ((neighbor = (struct lbdd_neighbor *) das_traverse(nodedata->neighbors)) != NULL) {
        if (neighbor->id == hello->src) {
            neighbor->position.x = hello->position.x;
            neighbor->position.y = hello->position.y;
            neighbor->position.z = hello->position.z;
            neighbor->time       = get_time();
            packet_dealloc(packet);
            return;
        }
    }  

    /* new neighbor */
    neighbor = (struct lbdd_neighbor *) malloc(sizeof(struct lbdd_neighbor));
    neighbor->id         = hello->src;
    neighbor->position.x = hello->position.x;
    neighbor->position.y = hello->position.y;
    neighbor->position.z = hello->position.z;
    neighbor->time       = get_time();
    das_insert(nodedata->neighbors, (void *) neighbor);
    packet_dealloc(packet);
    return;
}


/* ************************************************** */
/* ************************************************** */
void rx_source_adv(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata = get_entity_private_data(c);
    struct source_adv_p *source = (struct source_adv_p *) (packet->data + nodedata->overhead);
    struct sensor_adv_p *sensor;
    packet_t *packet0;
    destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};

    /* check adv sequence */
    if (source->s_seq <= nodedata->s_seq[source->source]) {
        /* old request */
        packet_dealloc(packet);
        return;
    }
    nodedata->s_seq[source->source] = source->s_seq;
    
    /* reply */
    packet0 = packet_create(c, nodedata->overhead + sizeof(struct sensor_adv_p), -1);
    sensor = (struct sensor_adv_p *) (packet0->data + nodedata->overhead);
    if (SET_HEADER(&c0, packet0, &dst) == -1) {
        packet_dealloc(packet);
        packet_dealloc(packet0);
        return;
    }
    sensor->type   = SENSOR_ADV_TYPE;
    sensor->sensor = c->node;
    sensor->source = source->source;
    sensor->s_seq  = source->s_seq;

    TX(&c0, packet0);
    entitydata->TX_sensor_adv++;

    packet_dealloc(packet);
    return;
}


/**************************************************************************/
/**************************************************************************/

/* received a DATA report from a source node => forwarding towards the rendez-vous area */
void rx_source_data(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata = get_entity_private_data(c);
    struct source_data_p *data = (struct source_data_p *) (packet->data + nodedata->overhead);
    position_t rdv_position;
    struct lbdd_neighbor *n_hop;

    /* check sensor */
    if (data->sensor != c->node) {
        /* not for us */
        if (nodedata->type == INLINE_NODE && data->d_seq > nodedata->d_seq[data->metadata]) {
	    nodedata->d_seq[data->metadata]    = data->d_seq;
	    nodedata->d_source[data->metadata] = data->source;
	    nodedata->d_value[data->metadata]  = data->d_value;
	    nodedata->d_hop[data->metadata]    = 1;
	    nodedata->d_time[data->metadata]   = get_time();
	}
        packet_dealloc(packet);
        return;
    }
    
   /* get the RENDEZ-VOUS AREA position */
    rdv_position.x = (get_topology_area())->x/2;
    rdv_position.y = (get_node_position(c->node))->y;
    rdv_position.z = -1;

    /* get next hop */
    n_hop = lbdd_next_hop(c, &rdv_position);
    
    /* if I am the storing node */
    if (n_hop == NULL) {

#ifdef LOG_APPLICATION_DISSEMINATION
      if (nodedata->type == INLINE_NODE) printf("[INLINE] %d (%.2lf,%.2lf,%.2lf) : no path towards (%.2lf,%.2lf,%.2lf) => storing data locally (%d:%d:%d:%d)  hop=1  delay=0 ms \n", c->node, (get_node_position(c->node))->x, (get_node_position(c->node))->y, (get_node_position(c->node))->z, rdv_position.x, rdv_position.y, rdv_position.z, data->source, data->d_value, data->d_seq, data->metadata);
      else printf("[Node] %d (%.2lf,%.2lf,%.2lf) : no path towards (%.2lf,%.2lf,%.2lf) => storing data locally (%d:%d:%d:%d)  hop=1  delay=0 ms \n", c->node, (get_node_position(c->node))->x, (get_node_position(c->node))->y, (get_node_position(c->node))->z, rdv_position.x, rdv_position.y, rdv_position.z, data->source, data->d_value, data->d_seq, data->metadata);
#endif
        /* store information */
        if (data->d_seq <= nodedata->d_seq[data->metadata]) {
            /* old data */
            packet_dealloc(packet);
            return;
        }
        nodedata->d_seq[data->metadata]    = data->d_seq;
        nodedata->d_source[data->metadata] = data->source;
        nodedata->d_value[data->metadata]  = data->d_value;
        nodedata->d_hop[data->metadata]    = 1;
        nodedata->d_time[data->metadata]   = get_time();

        packet_dealloc(packet);
        return;
    } else {

#ifdef LOG_APPLICATION_DISSEMINATION_ROUTING
      printf("Node %d : forwarding DATA(%d,%d) to node %d \n", c->node, data->metadata, data->d_seq, n_hop->id);
#endif
        /* create lbdd data packet */

#ifdef USE_BROADCAST_COMMUNICATIONS
        destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
        destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif

        packet_t *packet0 = packet_create(c, nodedata->overhead + sizeof(struct lbdd_data_p), -1);
        struct lbdd_data_p *disseminate =  (struct lbdd_data_p *) (packet0->data + nodedata->overhead);
        entityid_t *down = get_entity_links_down(c);
        call_t c0 = {down[0], c->node, c->entity};
        
        /* forward data */
        if (SET_HEADER(&c0, packet0, &dst) == -1) {
            packet_dealloc(packet0);
            packet_dealloc(packet);
            return;
        }
        disseminate->type     = LBDD_DATA_TYPE;
        disseminate->n_hop    = n_hop->id;
        disseminate->source   = data->source;
        disseminate->metadata = data->metadata;
        disseminate->d_seq    = data->d_seq;
        disseminate->d_value  = data->d_value;
        disseminate->hop      = 2;
        disseminate->time     = get_time();

        TX(&c0, packet0);
	entitydata->TX_data++;

        packet_dealloc(packet);
        return;
    }

    return;
}


/**************************************************************************/
/**************************************************************************/

/* Received a DATA report from a sensor */
void rx_lbdd_data(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata = get_entity_private_data(c);
    struct lbdd_data_p *data = (struct lbdd_data_p *) (packet->data + nodedata->overhead);
    position_t rdv_position;
    struct lbdd_neighbor *n_hop;

    /* check sensor */
    if (data->n_hop != c->node) {
        if (nodedata->type == INLINE_NODE && data->d_seq > nodedata->d_seq[data->metadata]) {
	    nodedata->d_seq[data->metadata]    = data->d_seq;
	    nodedata->d_source[data->metadata] = data->source;
	    nodedata->d_value[data->metadata]  = data->d_value;
	    nodedata->d_hop[data->metadata]    = data->hop;
	    nodedata->d_time[data->metadata]   = data->time;
	}
        /* not for us */
        packet_dealloc(packet);
        return;
    }

    /* get the RENDEZ-VOUS AREA position */
    rdv_position.x = (get_topology_area())->x/2;
    rdv_position.y = (get_node_position(c->node))->y;
    rdv_position.z = -1;

    /* get next hop */
    n_hop = lbdd_next_hop(c, &rdv_position);
    
    /* if I am the storing node */
    if (n_hop == NULL) {
      
#ifdef LOG_APPLICATION_DISSEMINATION
        if (nodedata->type == INLINE_NODE) printf("[INLINE] %d (%.2lf,%.2lf,%.2lf) : no path towards (%.2lf,%.2lf,%.2lf) => storing data locally (%d:%d:%d:%d)  hop=%d\n", c->node, (get_node_position(c->node))->x, (get_node_position(c->node))->y, (get_node_position(c->node))->z, rdv_position.x, rdv_position.y, rdv_position.z, data->source, data->d_value, data->d_seq, data->metadata, data->hop);
	else printf("[Node] %d (%.2lf,%.2lf,%.2lf) : no path towards (%.2lf,%.2lf,%.2lf) => storing data locally (%d:%d:%d:%d)  hop=%d\n", c->node, (get_node_position(c->node))->x, (get_node_position(c->node))->y, (get_node_position(c->node))->z, rdv_position.x, rdv_position.y, rdv_position.z, data->source, data->d_value, data->d_seq, data->metadata, data->hop);
#endif
        /* store information */
        if (data->d_seq <= nodedata->d_seq[data->metadata]) {
            /* old data */
            packet_dealloc(packet);
            return;
        }
	
        nodedata->d_seq[data->metadata]    = data->d_seq;
        nodedata->d_source[data->metadata] = data->source;
        nodedata->d_value[data->metadata]  = data->d_value;
        nodedata->d_hop[data->metadata]    = data->hop;
        nodedata->d_time[data->metadata]   = data->time;

        packet_dealloc(packet);
        return;
    } else {

#ifdef LOG_APPLICATION_DISSEMINATION_ROUTING
      printf("Node %d : forwarding DATA(%d,%d) to node %d \n", c->node, data->metadata, data->d_seq, n_hop->id);
#endif
        /* forward lbdd data packet */

#ifdef USE_BROADCAST_COMMUNICATIONS
        destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
        destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif

        entityid_t *down = get_entity_links_down(c);
        call_t c0 = {down[0], c->node, c->entity};
        
        /* forward data */
        if (SET_HEADER(&c0, packet, &dst) == -1) {
            packet_dealloc(packet);
            return;
        }

        data->n_hop = n_hop->id;
	data->hop++;

        TX(&c0, packet);
	entitydata->TX_data++;

        return;
    }

    return;
}


/* ************************************************** */
/* ************************************************** */

/* Received a request from a sink */
void rx_sink_adv(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata = get_entity_private_data(c);
    struct sink_adv_p *sink = (struct sink_adv_p *) (packet->data + nodedata->overhead);
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};
            
    /* starts home node election */
    if (sink->home == -1) {

        packet_t *packet0;
        struct home_adv_p *adv;
        destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
        
        /* check request sequence */
        if (sink->r_seq <= nodedata->r_seq[sink->sink]) {
            /* old request */
            packet_dealloc(packet);
            return;
        }
        nodedata->r_seq[sink->sink] = sink->r_seq;

	packet0 = packet_create(c, nodedata->overhead + sizeof(struct home_adv_p), -1);
	adv = (struct home_adv_p *) (packet0->data + nodedata->overhead);
	if (SET_HEADER(&c0, packet0, &dst) == -1) {
	  packet_dealloc(packet);
	  packet_dealloc(packet0);
	  return;
	}
	adv->type   = HOME_ADV_TYPE;
	adv->sensor = c->node;
	adv->sink   = sink->sink;
	adv->r_seq  = sink->r_seq;
	
	TX(&c0, packet0);	
	entitydata->TX_home_adv++;

        packet_dealloc(packet);
        return;
    } else if (sink->home == c->node) {
        /* sends the request towards the hash location */
        position_t rdv_position;
	struct lbdd_neighbor *n_hop;
      
	/* get the RENDEZ-VOUS AREA position */
	rdv_position.x = (get_topology_area())->x/2;
	rdv_position.y = (get_node_position(c->node))->y;
	rdv_position.z = -1;

	/* get the nexthop towards the RDV area */
        n_hop = lbdd_next_hop(c, &rdv_position);
        
        /* if I am the storing node */
        if (n_hop == NULL) {
            packet_t *packet0;
            struct sensor_data_p *data;
            destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};

            /* check wether we have the data */
            if (sink->d_seq > nodedata->d_seq[sink->metadata]) {
                packet_dealloc(packet);
                return;
            }
            
#ifdef LOG_APPLICATION_REQUEST
	    printf("Node %d : broadcasting DATA REPLY (%d,%d,%d) to sink %d \n", c->node, nodedata->d_source[sink->metadata], sink->metadata, nodedata->d_seq[sink->metadata], sink->sink);
#endif

            /* reply */
            packet0 = packet_create(c, nodedata->overhead + sizeof(struct sensor_data_p), -1);
            data = (struct sensor_data_p *) (packet0->data + nodedata->overhead);
            if (SET_HEADER(&c0, packet0, &dst) == -1) {
                packet_dealloc(packet);
                packet_dealloc(packet0);
                return;
            }
            data->type     = SENSOR_DATA_TYPE;
            data->metadata = sink->metadata;
            data->sink     = sink->sink;
            data->r_seq    = sink->r_seq;
            data->source   = nodedata->d_source[sink->metadata];
            data->d_seq    = nodedata->d_seq[sink->metadata];
            data->d_value  = nodedata->d_value[sink->metadata];
            TX(&c0, packet0);
	    entitydata->TX_sensor_data++;

            packet_dealloc(packet);
            return;
        } else {      

#ifdef LOG_APPLICATION_REQUEST_ROUTING
	  printf("Node %d (%lf,%lf) : forwarding REQUEST(sink=%d,%d,%d) to node %d (%lf,%lf)\n", c->node, get_node_position(c->node)->x, get_node_position(c->node)->y, sink->sink, sink->metadata, sink->r_seq, n_hop->id, get_node_position(n_hop->id)->x, get_node_position(n_hop->id)->y);
#endif

#ifdef USE_BROADCAST_COMMUNICATIONS
	  destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	  destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif

	  packet_t *packet0 = packet_create(c, nodedata->overhead + sizeof(struct lbdd_request_p), -1);
	  struct lbdd_request_p *request =  (struct lbdd_request_p *) (packet0->data + nodedata->overhead);
	  position_t *pos = get_node_position(c->node);

	  /* create request */
	  if (SET_HEADER(&c0, packet0, &dst) == -1) {
	    packet_dealloc(packet);
	    packet_dealloc(packet0);
	    return;
	  }
	  request->type       = LBDD_REQUEST_TYPE;
	  request->n_hop      = n_hop->id;
	  request->sink       = sink->sink;
	  request->r_seq      = sink->r_seq;
	  request->metadata   = sink->metadata;
	  request->d_seq      = sink->d_seq;
	  request->position.x = pos->x;
	  request->position.y = pos->y;
	  request->position.z = pos->z;
	  request->direction  = DIRECTION_NONE;
	  request->group_id   = -1; 
	
	  TX(&c0, packet0);    
	  entitydata->TX_query++;

	  packet_dealloc(packet);
	  return;
        }
    } else {
        packet_dealloc(packet);
        return;
    }
}


/**************************************************************************/
/**************************************************************************/

/* received a request from a sensor */
void rx_lbdd_request(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata = get_entity_private_data(c);

    struct lbdd_request_p *request = (struct lbdd_request_p *) (packet->data + nodedata->overhead);
    position_t rdv_position;
    struct lbdd_neighbor *n_hop;
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};

    /* are we the next hop */
    if (request->n_hop != c->node) {
        packet_dealloc(packet);
        return;
    }
    
    /* ################################## */  
    /* we are inside the RENDEZ-VOUS AREA */
    if (nodedata->type == INLINE_NODE) {
	
      //printf("node %d: INSIDE\n",c->node);

      /* do we have the requested data (or some more recent data) ? */
      if (request->d_seq <= nodedata->d_seq[request->metadata]) {
	
        /* get next hop to home node */
        struct lbdd_neighbor *p_hop = lbdd_next_hop(c, &(request->position));   
     
        if (p_hop == NULL) {
	  packet_t *packet0;
	  struct sensor_data_p *data;
	  destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};

	  /* reply */
	  packet0 = packet_create(c, nodedata->overhead + sizeof(struct sensor_data_p), -1);
	  data = (struct sensor_data_p *) (packet0->data + nodedata->overhead);
	  if (SET_HEADER(&c0, packet0, &dst) == -1) {
	    packet_dealloc(packet);
	    packet_dealloc(packet0);
	    return;
	  }
	  data->type     = SENSOR_DATA_TYPE;
	  data->metadata = request->metadata;
	  data->sink     = request->sink;
	  data->r_seq    = request->r_seq;
	  data->source   = nodedata->d_source[request->metadata];
	  data->d_seq    = nodedata->d_seq[request->metadata];
	  data->d_value  = nodedata->d_value[request->metadata];
	  data->delay    = nodedata->d_time[request->metadata];
	  data->hop      = nodedata->d_hop[request->metadata];

	  TX(&c0, packet0);
	  entitydata->TX_sensor_data++;

	  packet_dealloc(packet);
	  return;
        } 
	else {
	  packet_t *packet0;
	  struct lbdd_response_p *response;

#ifdef USE_BROADCAST_COMMUNICATIONS
	  destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	  destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif
            
	  /* reply */
	  packet0 = packet_create(c, nodedata->overhead + sizeof(struct lbdd_response_p), -1);
	  response = (struct lbdd_response_p *) (packet0->data + nodedata->overhead);
	  if (SET_HEADER(&c0, packet0, &dst) == -1) {
	    packet_dealloc(packet);
	    packet_dealloc(packet0);
	    return;
	  }
	  response->type       = LBDD_RESPONSE_TYPE;
	  response->n_hop      = p_hop->id;
	  response->metadata   = request->metadata;
	  response->sink       = request->sink;
	  response->r_seq      = request->r_seq;
	  response->source     = nodedata->d_source[request->metadata];
	  response->d_seq      = nodedata->d_seq[request->metadata];
	  response->d_value    = nodedata->d_value[request->metadata];
	  response->time       = nodedata->d_time[request->metadata];
	  response->hop        = nodedata->d_hop[request->metadata];
	  response->position.x = request->position.x;
	  response->position.y = request->position.y;
	  response->position.z = request->position.z;

	  TX(&c0, packet0);
	  entitydata->TX_response++;

	  packet_dealloc(packet);
	  return;
        }

      }
      
      /* ********************************************** */
      /* propagates query towards UP and DOWN direction */
      else if (request->direction == DIRECTION_NONE) {
	
	/* is there a nexthop towards the group leader */
	rdv_position.x = (get_topology_area())->x/2;
	rdv_position.y = (get_node_position(c->node))->y;
	rdv_position.z = -1;
	
	n_hop = lbdd_inside_next_hop(c, &rdv_position);    

	/* forwards query towards group leader */
	if (n_hop != NULL) {

#ifdef LOG_APPLICATION_REQUEST
	  printf("[INLINE] Node %d : received a request from sink %d (%.2lf,%.2lf) seq=%d => sending request towards group leader \n", c->node, request->sink, request->position.x, request->position.y, request->r_seq);
#endif

#ifdef USE_BROADCAST_COMMUNICATIONS
          destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	  destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif

	  if (SET_HEADER(&c0, packet, &dst) == -1) {
	    packet_dealloc(packet);
	    return;
	  }

#ifdef LOG_APPLICATION_REQUEST
	  printf("node %d: forwards towards group leader via node %d \n",c->node, n_hop->id);
#endif

	  request->n_hop = n_hop->id;
	  TX(&c0, packet);  
	  entitydata->TX_query++;

	} 
	/* propagates query towards north and south direction */
	else {

	  int group = (int)(get_node_position(c->node))->y / entitydata->group_size;
	  position_t up = {(get_topology_area())->x/2.0, ((group-1)*entitydata->group_size)+entitydata->group_size/2.0, -1};
	  position_t down = {(get_topology_area())->x/2.0, ((group+1)*entitydata->group_size)+entitydata->group_size/2.0, -1};

#ifdef LOG_APPLICATION_REQUEST
	  printf("[INLINE] Node %d (%lf,%lf) group=%d  =>  Propagation   UP:(%lf,%lf)  DOWN:(%lf,%lf)\n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y,group,up.x,up.y,down.x,down.y); 
#endif

	  /* transmits packet towards UP and DOWN direction */
	  if ( (up.y >=0 && up.y <=(get_topology_area())->y) && (down.y >=0 && down.y <=(get_topology_area())->y)) {

	    /* forwards packet to up direction */
	    n_hop = lbdd_inside_next_hop(c, &up); 

	    if (n_hop != NULL) {
	      request->direction = DIRECTION_UP;
	      request->group_id  = (int)(up.y / entitydata->group_size);
	      request->n_hop     = n_hop->id;

#ifdef USE_BROADCAST_COMMUNICATIONS
              destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	      destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif
	      
	      if (SET_HEADER(&c0, packet, &dst) == -1) {
		fprintf(stderr,"[LBDD] SET_HEADER errors...\n");
		packet_dealloc(packet);
		return;
	      }
	      request->n_hop = n_hop->id;

#ifdef LOG_APPLICATION_REQUEST
	      printf("[INLINE] Node %d (%lf,%lf) =>  Propagation UP %d \n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y, n_hop->id); 
#endif

	      TX(&c0, packet); 
	      entitydata->TX_query++;

	    } 
	    else {
#ifdef LOG_APPLICATION_REQUEST_ROUTING
	      printf("[INLINE] Node %d (%lf,%lf) group=%d : No path to UP (%lf,%lf)\n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y, group, up.x,up.y);
#endif
	      packet_dealloc(packet);
	    } 

	    /* forwards packet to down direction */
	    n_hop = lbdd_inside_next_hop(c, &down); 

	    if (n_hop != NULL) {
	      packet_t *packet_down = packet_clone(packet);
	      request = (struct lbdd_request_p *) (packet_down->data + nodedata->overhead);
	      
	      request->direction = DIRECTION_DOWN;
	      request->group_id  = (int)(down.y / entitydata->group_size);
	      request->n_hop     = n_hop->id;

#ifdef USE_BROADCAST_COMMUNICATIONS
              destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	      destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif

	      if (SET_HEADER(&c0, packet_down, &dst) == -1) {
		fprintf(stderr,"[LBDD] SET_HEADER errors...\n");
		packet_dealloc(packet_down);
		return;
	      }
	      request->n_hop = n_hop->id;

#ifdef LOG_APPLICATION_REQUEST
	      printf("[INLINE] Node %d (%lf,%lf) =>  Propagation DOWN %d \n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y, n_hop->id); 
#endif

	      TX(&c0, packet_down); 
	      entitydata->TX_query++;

	    } 
	    else {
#ifdef LOG_APPLICATION_REQUEST_ROUTING
	      printf("[INLINE] Node %d (%lf,%lf) group=%d : No path to DOWN (%lf,%lf)\n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y, group,up.x,up.y);
#endif
	    } 

	  }
	  /* transmits the packet towards the UP direction */
	  else if (up.y >=0 && up.y <=(get_topology_area())->y) {
	    
	    n_hop = lbdd_inside_next_hop(c, &up); 

	    if (n_hop != NULL) {
	      request->direction = DIRECTION_UP;
	      request->group_id  = (int)(up.y / entitydata->group_size);
	      request->n_hop     = n_hop->id;

#ifdef USE_BROADCAST_COMMUNICATIONS
              destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	      destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif

	      if (SET_HEADER(&c0, packet, &dst) == -1) {
		fprintf(stderr,"[LBDD] SET_HEADER errors...\n");
		packet_dealloc(packet);
		return;
	      }
	      request->n_hop = n_hop->id;

#ifdef LOG_APPLICATION_REQUEST
	      printf("[INLINE] Node %d (%lf,%lf) =>  Propagation UP1 %d \n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y, n_hop->id); 
#endif

	      TX(&c0, packet); 
	      entitydata->TX_query++;

	    } 
	    else {

#ifdef LOG_APPLICATION_REQUEST_ROUTING
	      printf("[INLINE] Node %d (%lf,%lf) group=%d : No path to UP1 (%lf,%lf) \n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y, group,up.x,up.y);
#endif
	      packet_dealloc(packet);
	    } 

	  }
	  /* transmits the packet towards the DOWN direction */
	  else if (down.y >=0 && down.y <=(get_topology_area())->y) {
	    
	    n_hop = lbdd_inside_next_hop(c, &down); 

	    if (n_hop != NULL) {
	      request->direction = DIRECTION_DOWN;
	      request->group_id  = (int)(down.y / entitydata->group_size);
	      request->n_hop     = n_hop->id;

#ifdef USE_BROADCAST_COMMUNICATIONS
              destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	      destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif
	      
	      if (SET_HEADER(&c0, packet, &dst) == -1) {
		fprintf(stderr,"[LBDD] SET_HEADER errors...\n");
		packet_dealloc(packet);
		return;
	      }
	      request->n_hop = n_hop->id;

#ifdef LOG_APPLICATION_REQUEST
	      printf("[INLINE] Node %d (%lf,%lf) => Propagation DOWN1 %d \n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y, n_hop->id); 
#endif

	      TX(&c0, packet); 
	      entitydata->TX_query++;

	    } 
	    else {
#ifdef LOG_APPLICATION_REQUEST_ROUTING
	      printf("[INLINE] Node %d (%lf,%lf) group=%d : No path to DOWN1 (%lf,%lf) \n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y, group,down.x,down.y);
#endif
	      packet_dealloc(packet);
	    } 

	  }
	  else {
	    /* should not happen */
	    packet_dealloc(packet);
	  }

	  return;
	}

      } 

      /* ************************************* */
      /* propagates query towards UP direction */
      else if (request->direction == DIRECTION_UP) {

#ifdef LOG_APPLICATION_REQUEST
	printf("[INLINE] Node %d (%lf,%lf) group=%d UP \n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y,(int)(get_node_position(c->node)->y/entitydata->group_size));
#endif

	position_t pos = {(get_topology_area())->x/2.0, (request->group_id*entitydata->group_size)+entitydata->group_size/2.0, -1};
	n_hop = lbdd_inside_next_hop(c, &pos);    

	/* forwards request towards the group leader */
	if (n_hop != NULL) {

#ifdef USE_BROADCAST_COMMUNICATIONS
	  destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	  destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif

	  if (SET_HEADER(&c0, packet, &dst) == -1) {
	    fprintf(stderr,"[LBDD] SET_HEADER errors...\n");
	    packet_dealloc(packet);
	    return;
	  }
	  request->n_hop = n_hop->id;

#ifdef LOG_APPLICATION_REQUEST
	  printf("Node %d received packet UP fowarding to leader via %d\n",c->node,n_hop->id);
#endif

	  TX(&c0, packet);  
	  entitydata->TX_query++;

	}
	/* forwards request towards the upper group leader */
	else if ( (request->group_id-1) >= 0) {
	  request->group_id --;
	  position_t up = {(get_topology_area())->x/2.0, (request->group_id*entitydata->group_size)+entitydata->group_size/2.0, -1};
	  n_hop = lbdd_inside_next_hop(c, &up); 
	  
	  if (n_hop != NULL) {
	    request->n_hop     = n_hop->id;

#ifdef USE_BROADCAST_COMMUNICATIONS
	    destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	    destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif
	    
	    if (SET_HEADER(&c0, packet, &dst) == -1) {
	      fprintf(stderr,"[LBDD] SET_HEADER errors...\n");
	      packet_dealloc(packet);
	      return;
	    }
	    request->n_hop = n_hop->id;

#ifdef LOG_APPLICATION_REQUEST
	    printf("Node %d received packet UP fowarding to upper leader via %d\n",c->node,n_hop->id);
#endif

	    TX(&c0, packet); 
	    entitydata->TX_query++;

	  } 
	  else {
#ifdef LOG_APPLICATION_REQUEST_ROUTING
	    printf("[INLINE] Node %d (%lf,%lf) group=%d : No path to UP (%lf,%lf) \n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y,(int)(get_node_position(c->node)->y/entitydata->group_size),up.x,up.y);
#endif
	    packet_dealloc(packet);
	  } 
	  
	}
	else {
	  packet_dealloc(packet);
	}
	
	return;
      } 
      /* ************************************* */
      /* propagates query towards DOWN direction */
      else if (request->direction == DIRECTION_DOWN) {

#ifdef LOG_APPLICATION_REQUEST
	//printf("[INLINE] Node %d (%lf,%lf) group=%d DOWN \n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y,(int)(get_node_position(c->node)->y/entitydata->group_size));
	printf("Node %d (%lf,%lf) received packet DOWN\n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y);
#endif
	position_t pos = {(get_topology_area())->x/2.0, (request->group_id*entitydata->group_size)+entitydata->group_size/2.0, -1};
	n_hop = lbdd_inside_next_hop(c, &pos);    

	/* forwards request towards the group leader */
	if (n_hop != NULL) {

#ifdef USE_BROADCAST_COMMUNICATIONS
	  destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	  destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif

	  if (SET_HEADER(&c0, packet, &dst) == -1) {
	    fprintf(stderr,"[LBDD] SET_HEADER errors...\n");
	    packet_dealloc(packet);
	    return;
	  }
	  request->n_hop = n_hop->id;

#ifdef LOG_APPLICATION_REQUEST
	  printf("Node %d received packet DOWN fowarding to leader via %d\n",c->node,n_hop->id);
#endif

	  TX(&c0, packet);  
	  entitydata->TX_query++;

	}
	/* forwards request towards the upper group leader */
	else if ( (request->group_id+1) <= (get_topology_area())->y) {
	  request->group_id ++;
	  position_t down = {(get_topology_area())->x/2.0, (request->group_id*entitydata->group_size)+entitydata->group_size/2.0, -1};
	  n_hop = lbdd_inside_next_hop(c, &down); 
	  
	  if (n_hop != NULL) {
	    request->n_hop     = n_hop->id;

#ifdef USE_BROADCAST_COMMUNICATIONS
	    destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	    destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif
	    
	    if (SET_HEADER(&c0, packet, &dst) == -1) {
	      fprintf(stderr,"[LBDD] SET_HEADER errors...\n");
	      packet_dealloc(packet);
	      return;
	    }
	    request->n_hop = n_hop->id;

#ifdef LOG_APPLICATION_REQUEST
	    printf("Node %d received packet DOWN fowarding to down leader via %d\n",c->node,n_hop->id);
#endif

	    TX(&c0, packet); 
	    entitydata->TX_query++;

	  } 
	  else {
#ifdef LOG_APPLICATION_REQUEST_ROUTING
	    printf("[INLINE] Node %d (%lf,%lf) group=%d : No path to DOWN (%lf,%lf) \n",c->node,(get_node_position(c->node))->x,(get_node_position(c->node))->y,(int)(get_node_position(c->node)->y/entitydata->group_size),down.x,down.y);
#endif
	    packet_dealloc(packet);
	  } 
	  
	}
	else {
	  packet_dealloc(packet);
	}
	
	return;
      } 
    }

    /* ########################################### */  
    /* forwards query towards the RENDEZ-VOUS AREA */
    else {
      /* get the RENDEZ-VOUS AREA position */
      rdv_position.x = (get_topology_area())->x/2;
      rdv_position.y = (get_node_position(c->node))->y;
      rdv_position.z = -1;

      n_hop = lbdd_next_hop(c, &rdv_position);    

      /* if I am the storing node */
      if (n_hop == NULL) {
        /* get next hop to home node */
        struct lbdd_neighbor *p_hop = lbdd_next_hop(c, &(request->position));
        
        /* check wether we have the data */
        if (request->d_seq > nodedata->d_seq[request->metadata]) {
	  packet_dealloc(packet);
	  return;
        }
        
	/* broadcasts the data to the sink */
        if (p_hop == NULL) {
	  packet_t *packet0;
	  struct sensor_data_p *data;
	  destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};

	  /* reply */
	  packet0 = packet_create(c, nodedata->overhead + sizeof(struct sensor_data_p), -1);
	  data = (struct sensor_data_p *) (packet0->data + nodedata->overhead);
	  if (SET_HEADER(&c0, packet0, &dst) == -1) {
	    packet_dealloc(packet);
	    packet_dealloc(packet0);
	    return;
	  }
	  data->type     = SENSOR_DATA_TYPE;
	  data->metadata = request->metadata;
	  data->sink     = request->sink;
	  data->r_seq    = request->r_seq;
	  data->source   = nodedata->d_source[request->metadata];
	  data->d_seq    = nodedata->d_seq[request->metadata];
	  data->d_value  = nodedata->d_value[request->metadata];
	  TX(&c0, packet0);
	  entitydata->TX_sensor_data++;

	  packet_dealloc(packet);
	  return;
        } 
	/* sends the response back to the home-node */
	else {
	  packet_t *packet0;
	  struct lbdd_response_p *response;

#ifdef USE_BROADCAST_COMMUNICATIONS
	  destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	  destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif
            
	  /* reply */
	  packet0 = packet_create(c, nodedata->overhead + sizeof(struct lbdd_response_p), -1);
	  response = (struct lbdd_response_p *) (packet0->data + nodedata->overhead);
	  if (SET_HEADER(&c0, packet0, &dst) == -1) {
	    packet_dealloc(packet);
	    packet_dealloc(packet0);
	    return;
	  }

	  response->type       = LBDD_RESPONSE_TYPE;
	  response->n_hop      = p_hop->id;
	  response->metadata   = request->metadata;
	  response->sink       = request->sink;
	  response->r_seq      = request->r_seq;
	  response->source     = nodedata->d_source[request->metadata];
	  response->d_seq      = nodedata->d_seq[request->metadata];
	  response->d_value    = nodedata->d_value[request->metadata];
	  response->position.x = request->position.x;
	  response->position.y = request->position.y;
	  response->position.z = request->position.z;

	  TX(&c0, packet0);
	  entitydata->TX_response++;

	  packet_dealloc(packet);
	  return;
        }
      } else {

#ifdef LOG_APPLICATION_REQUEST_ROUTING
	printf("Node %d (%lf,%lf) : forwarding REQUEST(sink=%d,%d,%d) to node %d (RDV %lf,%lf)\n", c->node, (get_node_position(c->node))->x,(get_node_position(c->node))->y, request->sink, request->metadata, request->r_seq, n_hop->id,rdv_position.x,rdv_position.y);
#endif

        /* forward packet */
#ifdef USE_BROADCAST_COMMUNICATIONS
	destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif
        
        /* forward data */
        if (SET_HEADER(&c0, packet, &dst) == -1) {
	  packet_dealloc(packet);
	  return;
        }
        request->n_hop = n_hop->id;
        
	TX(&c0, packet);    
	entitydata->TX_query++;
      }
    }

    return;
}


/**************************************************************************/
/**************************************************************************/
void rx_lbdd_response(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata = get_entity_private_data(c);
    struct lbdd_response_p *response = (struct lbdd_response_p *) (packet->data + nodedata->overhead);
    struct lbdd_neighbor *n_hop;
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};

    /* are we the next hop */
    if (response->n_hop != c->node) {
        packet_dealloc(packet);
        return;
    }

    /* get next hop */
    n_hop = lbdd_next_hop(c, &(response->position));
    
    /* if I am the home node */
    if (n_hop == NULL) {

#ifdef LOG_APPLICATION_RESPONSE
      printf("[HOME] node %d : broadcasting received DATA to (sink=%d,%d,%d)\n", c->node, response->sink, response->metadata, response->r_seq);
#endif
        packet_t *packet0;
        struct sensor_data_p *data;
	destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
        
        /* reply */
        packet0 = packet_create(c, nodedata->overhead + sizeof(struct sensor_data_p), -1);
        data = (struct sensor_data_p *) (packet0->data + nodedata->overhead);
        if (SET_HEADER(&c0, packet0, &dst) == -1) {
            packet_dealloc(packet);
            packet_dealloc(packet0);
            return;
        }
        data->type     = SENSOR_DATA_TYPE;
        data->metadata = response->metadata;
        data->sink     = response->sink;
        data->r_seq    = response->r_seq;
        data->source   = response->source;
        data->d_seq    = response->d_seq;
        data->d_value  = response->d_value;
	data->delay    = (get_time()-response->time)*0.000001;
	data->hop      = response->hop;

        TX(&c0, packet0);
	entitydata->TX_sensor_data++;

        packet_dealloc(packet);
        return;
    } else {

#ifdef LOG_APPLICATION_RESPONSE_ROUTING
	printf("Node %d : forwarding REPLAY to (sink=%d,%d,%d) via node %d \n", c->node, response->sink, response->metadata, response->r_seq, n_hop->id);
#endif
        /* forward packet */

#ifdef USE_BROADCAST_COMMUNICATIONS
	destination_t dst = {BROADCAST_ADDR, {-1, -1, -1}};
#else
	destination_t dst = {n_hop->id, {-1, -1, -1}};
#endif

        /* forward data */
        if (SET_HEADER(&c0, packet, &dst) == -1) {
            packet_dealloc(packet);
            return;
        }
        response->n_hop = n_hop->id;
	response->hop++;

        TX(&c0, packet);    
	entitydata->TX_response++;
    }

    return;
}

/* ************************************************** */
/* ************************************************** */
void lbdd_stats(call_t *c) {
   struct nodedata *nodedata = get_node_private_data(c);
   position_t *position = get_node_position(c->node);

   if (nodedata->type == INLINE_NODE) {
     printf("INLINE node %d (%lf,%lf,%lf)  Group_id=%d   Neighbors=%d\n", c->node, position->x, position->y, position->z, nodedata->group_id, das_getsize(nodedata->neighbors));
   } else {
     printf("SENSOR node %d (%lf,%lf,%lf)  Neighbors=%d\n", c->node, position->x, position->y, position->z, das_getsize(nodedata->neighbors));

   }
}


/* ************************************************** */
/* ************************************************** */
application_methods_t methods = {rx};

