/**
 *  \file   node.h
 *  \brief  Node declarations
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#ifndef __node__
#define __node__

#include <glib.h>
#include <gmodule.h>

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
#define NODE_UNDEF        0
#define NODE_DEAD         1
#define NODE_ACTIVE       2

#define NODE_LOCAL        0
#define NODE_DISCONNECTED 1
#define NODE_CONNECTED    2


/* ************************************************** */
/* ************************************************** */
typedef struct _node {
    nodeid_t id;
    bundleid_t bundle;
    int state;
    position_t position;
    int worldsens;
    uint64_t birth;

    void **private;
    void *noises;
} node_t;


/* ************************************************** */
/* ************************************************** */
typedef struct _node_array {
    int size;
    node_t *elts;
} node_array_t;

extern node_array_t nodes;


/* ************************************************** */
/* ************************************************** */
#ifdef N_DAS_O
extern void *location;
#endif //N_DAS_O
int node_init(void);
void node_clean(void);
int node_bootstrap(void);


/* ************************************************** */
/* ************************************************** */
int nodes_create(void);
void nodes_update_mobility(void);
int is_node_alive(nodeid_t node);
void node_birth(nodeid_t id);

/* ************************************************** */
/* ************************************************** */
node_t *get_node_by_id(nodeid_t id);


#endif //__node__
