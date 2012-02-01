/**
 *  \file   filestatic.c
 *  \brief  Static routing extracted from file
 *  \author Elyes Ben Hamida and Guillaume Chelius
 *  \date   2007
 **/
#include <stdio.h>

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Static routing",
    "Elyes Ben Hamida and Guillaume Chelius",
    "0.1",
    MODELTYPE_ROUTING, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct route {
    nodeid_t dst;
    nodeid_t n_hop;
};

struct routing_header {
    nodeid_t dst;
    nodeid_t src;
};

struct entitydata {
    FILE *file;
};

struct nodedata {
    void *routes;
    int overhead;
};


/* ************************************************** */
/* ************************************************** */
unsigned long route_hash(void *key) { 
    return (unsigned long) key;
}

int route_equal(void *key0, void *key1) { 
    return (int) (key0 == key1);
}


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;
    char *filepath = NULL;

    /* default values */
    filepath = "routing.data";

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "file")) {
            filepath = param->value;
        }
    }
  
    /* open file */
    if ((entitydata->file = fopen(filepath, "r")) == NULL) {
        fprintf(stderr, "filestatic: can not open file %s in init()\n", filepath);
        goto error;
    }
    
    set_entity_private_data(c, entitydata);
    return 0;

 error:
    free(entitydata);
    return -1;
}

int destroy(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);

    if (entitydata->file != NULL) {
        fclose(entitydata->file);
    }

    free(entitydata);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int setnode(call_t *c, void *params) {
    struct entitydata *entitydata = get_entity_private_data(c);
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    char str[128];
    int id, dst, n_hop;
    
    /* extract routing table from file */
    nodedata->routes = hadas_create(route_hash, route_equal);
    
    /* extract routing table from file */
    fseek(entitydata->file, 0L, SEEK_SET);
    while (fgets(str, 128, entitydata->file) != NULL) {
        if (sscanf(str, "%d %d %d\n",  &id, &dst, &n_hop) != 3) {
            fprintf(stderr, "filestatic: unable to read route in setnode()\n");
            goto error;
        }
        
        if (id == c->node) {
            struct route *route = (struct route *) malloc(sizeof(struct route));
            route->dst = dst;
            route->n_hop = n_hop;
            hadas_insert(nodedata->routes, (void *) ((unsigned long) (route->dst)), (void *) route);
        }
    }
    
    nodedata->overhead = -1;
    set_node_private_data(c, nodedata);
    return 0;

 error:
    free(entitydata);
    return -1;
}

int unsetnode(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct route *route;
    int i = get_node_count();
    while (i--) {
        if ((route = (struct route *) hadas_get(nodedata->routes, (void *) ((unsigned long) i))) != NULL) {
            free(route);
        }
    }
    hadas_destroy(nodedata->routes);    
    free(nodedata);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    
    /* get mac header overhead */
    nodedata->overhead = GET_HEADER_SIZE(&c0);
 
    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int set_header(call_t *c, packet_t *packet, destination_t *dst) {
    struct nodedata *nodedata = get_node_private_data(c);
    struct route *route = hadas_get(nodedata->routes, (void *) ((unsigned long) (dst->id)));
    struct routing_header *header = (struct routing_header *) (packet->data + nodedata->overhead);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    destination_t n_hop;

    if (route == NULL) {
        return -1;
    }
    header->dst = dst->id;
    header->src = c->node;
    
    n_hop.id = route->n_hop;
    return SET_HEADER(&c0, packet, &n_hop);
}

int get_header_size(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    
    if (nodedata->overhead == -1) {
        call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
        nodedata->overhead = GET_HEADER_SIZE(&c0);
    }
    
    return nodedata->overhead + sizeof(struct routing_header);
}

int get_header_real_size(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    
    if (nodedata->overhead == -1) {
        call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
        nodedata->overhead = GET_HEADER_REAL_SIZE(&c0);
    }
    
    return nodedata->overhead + sizeof(struct routing_header);
}


/* ************************************************** */
/* ************************************************** */
void tx(call_t *c, packet_t *packet) {
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    
    TX(&c0, packet);
}


/* ************************************************** */
/* ************************************************** */
void forward(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    struct routing_header *header = (struct routing_header *) (packet->data + nodedata->overhead);
    struct route *route = hadas_get(nodedata->routes, (void *) ((unsigned long) (header->dst)));
    destination_t destination;

    if (route == NULL) {
        packet_dealloc(packet);
        return;
    }

    destination.id = route->n_hop;
    if (SET_HEADER(&c0, packet, (void *) &destination) == -1) {
        packet_dealloc(packet);
        return;
    }
    TX(&c0, packet);
}

void rx(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
    array_t *up = get_entity_bindings_up(c);
    int i = up->size;
    struct routing_header *header = (struct routing_header *) (packet->data + nodedata->overhead);
    
    if ((header->dst != c->node) ) {
        forward(c, packet);
        return;
    }

    while (i--) {
        call_t c_up = {up->elts[i], c->node, c->entity};
        packet_t *packet_up;
        
        if (i > 0) {
            packet_up = packet_clone(packet);         
        } else {
            packet_up = packet;
        }
        
        RX(&c_up, packet_up);
    }
}


/* ************************************************** */
/* ************************************************** */
routing_methods_t methods = {rx, 
                             tx, 
                             set_header, 
                             get_header_size,
                             get_header_real_size};
