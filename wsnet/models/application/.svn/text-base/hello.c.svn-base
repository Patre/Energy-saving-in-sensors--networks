/**
 *  \file   hello.c
 *  \brief  module for a simple HELLO protocol
 *  \author Elyes Ben Hamida
 *  \date   2009
 **/

#include <stdio.h>
#include <include/modelutils.h>
#include <inttypes.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "HELLO protocol module",
    "Elyes Ben Hamida",
    "0.1",
    MODELTYPE_APPLICATION, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct _hello_header {
   int id;
};

struct neighbor {
    int id;
    uint64_t time;
};

/* ************************************************** */
/* ************************************************** */
struct _hello_private {
  int *overhead;
  uint64_t start;
  uint64_t w;
  uint64_t d;
  int size;
  int graph;
  /* stats */
  double avg_distance;    /* average distance of discovery */
  double avg_connexity;   /* average connexity */
  double avg_neighbors;   /* average number of discovered neighbors */
  double avg_rx;          /* nbr of received hello packets */
  double avg_delivery;    /* average delivery ratio */
  
};

struct _hello_entity_private {
    int nbr_nodes;
    int **adj;
    int source;
    /* stats */
    int TX;
    int RX;
};

/* ************************************************** */
/* ************************************************** */
int callmeback(call_t *c, void *args);
int tx(call_t *c, void *args);

void init_adj(call_t *c);
void show_adj(call_t *c);
void save_adj(call_t *c);

int get_neighbors_nbr(call_t *c, int node);
double get_max_neighbor(call_t *c, int node);
double get_min_neighbor(call_t *c, int node);
int is_connex(call_t *c);
int get_nbrconnex(call_t *c);


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct _hello_entity_private *entitydata = malloc(sizeof(struct _hello_entity_private));
    int i = 0, j = 0;

    /* init neighbors table */
    entitydata->nbr_nodes = get_node_count();
    entitydata->adj       = (int **) malloc(entitydata->nbr_nodes * sizeof(int *));
    entitydata->source    = -1;
    entitydata->TX        = 0;
    entitydata->RX        = 0;

    for (i = 0; i < entitydata->nbr_nodes; i++) {
        entitydata->adj[i] = (int *) malloc(entitydata->nbr_nodes * sizeof(int));
    }

    for (i = 0; i < entitydata->nbr_nodes; i++) {
        for (j = 0; j < entitydata->nbr_nodes; j++) {
	    entitydata->adj[i][j] = 0;
	}
    }

    set_entity_private_data(c, entitydata);
    return 0;
}

int destroy(call_t *c) {
    struct _hello_entity_private *entitydata = get_entity_private_data(c);
    int i = 0;
    for (i = 0; i < entitydata->nbr_nodes; i++) {
        free(entitydata->adj[i]);
    }
    free(entitydata->adj);
    free(entitydata);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int setnode(call_t *c, void *params) {
    struct _hello_private *nodedata = malloc(sizeof(struct _hello_private));
    int i = get_entity_links_down_nbr(c);
    param_t *param;
    
    /* default values */
    nodedata->start  = 0;
    nodedata->w      = 1000000000;
    nodedata->d      = 8000000;
    nodedata->size   = 10;
    nodedata->graph  = 0;

    nodedata->avg_distance  = 0;
    nodedata->avg_connexity = 0;
    nodedata->avg_neighbors = 0;
    nodedata->avg_rx        = 0;
    nodedata->avg_delivery  = 0;

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {

        if (!strcmp(param->key, "start")) {
            if (get_param_time(param->value, &(nodedata->start))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "period")) {
            if (get_param_time(param->value, &(nodedata->w))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "size")) {
            if (get_param_integer(param->value, &(nodedata->size))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "graph")) {
            if (get_param_integer(param->value, &(nodedata->graph))) {
                goto error;
            }
        }
    }
    
    /* alloc overhead memory */
    if (i) {
        nodedata->overhead = malloc(sizeof(int) * i);
    } else {
        nodedata->overhead = NULL;
    }
    
    set_node_private_data(c, nodedata);
    return 0;
  
 error:
    free(nodedata);
    return -1;
}

int unsetnode(call_t *c) {
    struct _hello_private *nodedata = get_node_private_data(c);

    if (nodedata->overhead) {
        free(nodedata->overhead);
    }
    free(nodedata);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(call_t *c) {
    struct _hello_private *nodedata = get_node_private_data(c);
    struct _hello_entity_private *entitydata = get_entity_private_data(c);

    int i = get_entity_links_down_nbr(c);
    entityid_t *down = get_entity_links_down(c);
    uint64_t start = get_time();
    int n =0, source = 0;
    double dist = 999999, tmp = 0;
    position_t center = {get_topology_area()->x/2, get_topology_area()->y/2, get_topology_area()->z/2};

    /* get overhead */
    while (i--) {
        call_t c0 = {down[i], c->node, c->entity};
        if ((get_entity_type(&c0) != MODELTYPE_ROUTING) 
            && (get_entity_type(&c0) != MODELTYPE_MAC)) {
            nodedata->overhead[i] = 0;
        } else {
            nodedata->overhead[i] = GET_HEADER_SIZE(&c0);
        }
    }
    
    /* eventually schedule callback */
    scheduler_add_callback(start, c, callmeback, NULL);

    /* elect source node => the node which is the closest to the area center */
    if (c->node == 0) {
      n = get_node_count();
      for (i=0; i<n; i++) {
	tmp = distance(get_node_position((nodeid_t)i), &center);
	if (tmp < dist) {
	  dist = tmp;
	  source = i;
	}
      }
      entitydata->source = source;
      printf("Source election: node %d\n", entitydata->source);

    }

    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int callmeback(call_t *c, void *args) {
    struct _hello_private *nodedata = get_node_private_data(c);
    struct _hello_entity_private *entitydata =  get_entity_private_data(c);
    double time = 0;

    /* callback for HELLO transmission */
    uint64_t start = get_time() + get_random_double() * (nodedata->w - nodedata->d);
    scheduler_add_callback(start, c, tx, NULL);
 
    
    if (c->node == entitydata->source) {
      int neighbors_nbr = get_neighbors_nbr(c, c->node);
      double max_neigh  = get_max_neighbor(c, c->node);
      int connexity = get_nbrconnex(c);
      
      time = get_time() * 0.000000001;

      if (get_time() != 0) {
	fprintf(stdout, "Time=%lfs :  Source_id=%d  Neighbors_nbr=%d  MaxDistance=%lf  Nbr_Connex_components=%d  Delivery_ratio=%.3f%%\n", time, c->node, neighbors_nbr, max_neigh, connexity, ((float)entitydata->RX/entitydata->TX));
      }

      if (get_time() != 0) {
	nodedata->avg_distance  += max_neigh;
	nodedata->avg_connexity += connexity;
	nodedata->avg_neighbors += neighbors_nbr;
	nodedata->avg_delivery  += ((float)entitydata->RX/entitydata->TX);
	nodedata->avg_rx ++;
      }

      if (nodedata->graph > 0) {
	save_adj(c);
      }

      entitydata->RX       = 0;
      entitydata->TX       = 0;

      init_adj(c);
    } 

    scheduler_add_callback(get_time() + nodedata->w, c, callmeback, NULL);
    return 0;
}

/* ************************************************** */
/* ************************************************** */
int tx(call_t *c, void *args) {
    struct _hello_private *nodedata = get_node_private_data(c);
    struct _hello_entity_private *entitydata =  get_entity_private_data(c);
    
    int i = get_entity_links_down_nbr(c);
    entityid_t *down = get_entity_links_down(c);
    destination_t destination = {-1, {-1,-1,-1}};
    
    while (i--) {
      packet_t *packet = packet_create(c, nodedata->size + sizeof(struct _hello_header) + nodedata->overhead[i], -1);
        call_t c0 = {down[i], c->node, c->entity};
	
	struct _hello_header *hello = (struct _hello_header*)(packet->data+nodedata->overhead[0]);
	hello->id = c->node;

        if ((get_entity_type(&c0) == MODELTYPE_ROUTING) 
            || (get_entity_type(&c0) == MODELTYPE_MAC)) {
	  if (SET_HEADER(&c0, packet, &destination) == -1) {
            packet_dealloc(packet);
            continue;
	  }
        }
        
	TX(&c0, packet);
	entitydata->TX++;
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
    struct _hello_private *nodedata = get_node_private_data(c);
    struct _hello_entity_private *entitydata = get_entity_private_data(c);
    struct _hello_header *hello = (struct _hello_header*)(packet->data+nodedata->overhead[0]);
        
    if (entitydata->adj[c->node][hello->id] == 0) {
      entitydata->adj[c->node][hello->id] = 1;
    }

    entitydata->RX++;

    /* dealloc packet */
    packet_dealloc(packet);
}



/* ************************************************** */
/* ************************************************** */
void init_adj(call_t *c) {
    struct _hello_entity_private *entitydata =  get_entity_private_data(c);
    int i = 0, j = 0;
    for (i = 0; i < entitydata->nbr_nodes; i++) {
        for (j = 0; j < entitydata->nbr_nodes; j++) {
	    entitydata->adj[i][j] = 0;
	}
    }
}

void save_adj(call_t *c) {
    struct _hello_entity_private *entitydata =  get_entity_private_data(c);
    FILE *file = NULL;
    char file_map[100];
    double time = get_time() * 0.000001;
    int i = 0, j = 0, n = 0;
    double degree = 0;

    /* average node degree */
    for (i = 0; i < entitydata->nbr_nodes; i++) {
        n = 0;
        for (j = 0; j < entitydata->nbr_nodes; j++) {
	    if (i != j && entitydata->adj[i][j] > 0) {
	       n++;
	    }
	}
	degree += n;
    }
    
    degree = (double)(degree/(double)entitydata->nbr_nodes);

    /* create file */
    sprintf(file_map, "graph.%.0lf.%d.map", time, (int)floor(degree));
    if ((file = fopen(file_map, "w+")) == NULL) {
      fprintf(stderr, "My monitor: can not open file %s in monitor_event()\n", file_map);
      return;
    }

    for (i = 0; i < entitydata->nbr_nodes; i++) {
        fprintf(file, "%d %lf %lf", i, get_node_position(i)->x, get_node_position(i)->y);
        for (j = 0; j < entitydata->nbr_nodes; j++) {
	  if (entitydata->adj[i][j] > 0) {
	    fprintf(file, " %d", j);
	  }
	}
	fprintf(file, "\n");
    }

    fclose(file);

}

void show_adj(call_t *c) {
    struct _hello_entity_private *entitydata =  get_entity_private_data(c);
    int i = 0, j = 0;
    for (i = 0; i < entitydata->nbr_nodes; i++) {
        for (j = 0; j < entitydata->nbr_nodes; j++) {
	  printf("%d ",entitydata->adj[i][j]);
	}
	printf("\n");
    }
}

int get_neighbors_nbr(call_t *c, int node) {
    struct _hello_entity_private *entitydata =  get_entity_private_data(c);
    int i = 0, cpt = 0;

    for (i = 0; i < entitydata->nbr_nodes; i++) {
      if (i != node && entitydata->adj[node][i] == 1) {
	cpt++;
      }
    }

    return cpt;
}

double get_max_neighbor(call_t *c, int node) {
    struct _hello_entity_private *entitydata =  get_entity_private_data(c);
    int i = 0;
    double dist = 0, tmp = 0;

    for (i = 0; i < entitydata->nbr_nodes; i++) {
      if (i != node && entitydata->adj[node][i] == 1) {
	tmp = distance(get_node_position(node), get_node_position(i));
	if (tmp > dist) {
	  dist = tmp;
	}
      }
    }
    return dist;
}

double get_min_neighbor(call_t *c, int node) {
    struct _hello_entity_private *entitydata =  get_entity_private_data(c);
    int i = 0;
    double dist = 99999, tmp = 0;

    for (i = 0; i < entitydata->nbr_nodes; i++) {
      if (i != node && entitydata->adj[node][i] == 1) {
	tmp = distance(get_node_position(node), get_node_position(i));
	if (tmp < dist) {
	  dist = tmp;
	}
      }
    }
    return dist;
}

int is_connex(call_t *c) {
  struct _hello_entity_private *entitydata =  get_entity_private_data(c);
  int i = 0, j = 0, n = 0, connected = 1;
  uintptr_t  *v  = (uintptr_t *) malloc(entitydata->nbr_nodes * sizeof(int));
  void  *pile =(uintptr_t *) das_create();

  for (i=0; i<entitydata->nbr_nodes; i++) {
    v[i] = 0;
  }
  
  v[0] = 1;
  das_insert(pile, 0);

  while (das_getsize(pile) != 0) {
    n = (uintptr_t) das_pop(pile);
    v[n] = 1;
    for (j=0; j<n; j++) {
      if (entitydata->adj[n][j] == 1 && v[j] == 0) {
	das_insert(pile,(void *)(uintptr_t) j);
	v[j] = 1;
      }
    }
  }

  connected = 1;
  for (i=0; i<entitydata->nbr_nodes; i++) {
    if(v[i] == 0) {
       connected = 0;
       break;
    }
  }

  das_destroy(pile);
  free(v);
  return connected;
}


int get_nbredge(call_t *c, int node, int *v) {
  struct _hello_entity_private *entitydata =  get_entity_private_data(c);
  void *stack =(uintptr_t  * ) das_create();
  int n = 0, tmp = 0, i = 0;
  
  das_insert(stack,(void *)(uintptr_t) node);

  while(das_getsize(stack)!=0) {
    tmp = (uintptr_t) das_pop(stack);
    v[tmp] = 1;

    for (i=0; i<entitydata->nbr_nodes; i++){
      
      if (i != tmp && entitydata->adj[tmp][i] != 0) {
	n++;
	if (v[i] == 0) {
	  v[i] = 1;
	  das_insert(stack,(void *)(uintptr_t)i);
	}
      }
    }
  }
  
  return n/2;
}

int get_nbrconnex(call_t *c) {
  struct _hello_entity_private *entitydata =  get_entity_private_data(c);
  int i = 0, n = 0, cpt = 0;

  int *v  = (int *) malloc(entitydata->nbr_nodes * sizeof(int));
  
  for (i=0; i<entitydata->nbr_nodes; i++) {
    v[i] = 0;
  }

  for (i=0; i<entitydata->nbr_nodes; i++) {
      if (v[i] == 0) {
        n = get_nbredge(c, i, v);
	if (n>0) cpt++;
      } 
  }

  return cpt;
}

/* ************************************************** */
/* ************************************************** */
application_methods_t methods = {rx};
