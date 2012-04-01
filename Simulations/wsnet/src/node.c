/**
 *  \file   node.c
 *  \brief  Node support 
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include "internals.h"


/* ************************************************** */
/* ************************************************** */
node_array_t nodes = {0, NULL};
#ifdef N_DAS_O
void *location = NULL;
#endif /* N_DAS_O */


/* ************************************************** */
/* ************************************************** */
node_t *get_node_by_id(nodeid_t id) {
    return nodes.elts + id;
}


/* ************************************************** */
/* ************************************************** */
int node_init(void) {
#ifdef N_DAS_O
    if (spadas_init() == -1) {
        return -1;
    } 
#endif /* N_DAS_O */
    return 0;
}

void node_clean(void) {
    if (nodes.elts) {
        int i = nodes.size;
        
        while (i--) {
            node_t *node = get_node_by_id(i);
            if (node->private) {
                free(node->private);
            }
        }
        
        free(nodes.elts);
    }

#ifdef N_DAS_O
    if (location) {
        spadas_destroy(location);
    }
#endif /* N_DAS_O */

    return;
}


/* ************************************************** */
/* ************************************************** */
int node_bootstrap(void) {
    int i;

    for (i = 0; i < bundles.size; i++) {
        bundle_t *bundle = get_bundle_by_id(i);
        entity_t *entity;
        int j;
        
        /* check mobility entity */
        entity = get_entity_by_id(bundle->mobility);
        if (entity->model->type != MODELTYPE_MOBILITY) {
            fprintf(stderr, "entity %s is not a mobility model\n", entity->name);
            return -1;
        }
        
        /* check energy entity */
        if (bundle->energy != -1) {
            entity = get_entity_by_id(bundle->energy);
            if (entity->model->type != MODELTYPE_ENERGY) {
                fprintf(stderr, "entity %s is not an energy model\n", entity->name);
                return -1;
            }
        }
        
        /* check antenna entities */
        for (j = 0; j < bundle->antenna.size; j++) {
            entity = get_entity_by_id(bundle->antenna.elts[j]);
            if (entity->model->type != MODELTYPE_ANTENNA) {
                fprintf(stderr, "entity %s is not an antenna model\n", entity->name);
                return -1;
            }
        }
    }

    /*  set node birth */
    for (i = 0; i < nodes.size; i++) {
        node_t *node = get_node_by_id(i);
        if (node->worldsens != NODE_LOCAL) {
            continue;
        }
        scheduler_add_birth(node->birth, node->id);
    }

    return 0;
}

int nodes_create(void) {
    int i = nodes.size;
    
#ifdef N_DAS_O
    location = spadas_create(get_topology_area(), get_topology_range());
#endif /*N_DAS_O*/
    nodes.elts = (node_t *) malloc(sizeof(node_t) * nodes.size);
    
    while (i--) {
        node_t *node = get_node_by_id(i);
        node->id = i;
        node->bundle = -1;
        node->state = NODE_UNDEF;
        node->worldsens = NODE_LOCAL;
        node->birth = 0;
        node->noises = NULL;
        node->private = NULL;
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
void node_birth(nodeid_t id) {
    node_t *node = get_node_by_id(id);
    int i;
    
    if (node->state != NODE_UNDEF) {
        return;
    }
    
    /* bootstrap */
    for (i = 0; i < bundles.elts[node->bundle].entity.size; i++) {
        entity_t *entity = get_entity_by_id(bundles.elts[node->bundle].entity.elts[i]);
        call_t c = {entity->id, id, -1};
        if ((entity)->bootstrap) {
            (entity)->bootstrap(&c);
        }
    }
    
    /* set node active */
    node->state = NODE_ACTIVE;
#ifdef N_DAS_O
    spadas_insert(location, node, &(node->position));
#endif /* N_DAS_O */
}

void node_kill(nodeid_t id) {
    node_t *node = get_node_by_id(id);
    int i;

    if (node->state == NODE_DEAD) {
        return;
    }
    
    node->state = NODE_DEAD;
#ifdef N_DAS_O
    spadas_delete(location, node, &(node->position));
#endif /* N_DAS_O */
    
    PRINT_REPLAY("kill %"PRId64" %d %lf %lf %lf\n", get_time(), id, node->position.x, node->position.y, node->position.z);
    
    /* disconnect node */
    if (node->worldsens == NODE_CONNECTED) {
        node->worldsens = NODE_DISCONNECTED;
        worldsens_tx_kill(node->id);
    }

    /* monitor death */
    if (monitor_entity) {
        call_t c = {monitor_entity->id, id, -1};
        monitor_entity->methods->monitor.monitor_death(&c);
    }

    /* unset node */
    for (i = 0; i < bundles.elts[node->bundle].entity.size; i++) {
        entity_t *entity = get_entity_by_id(bundles.elts[node->bundle].entity.elts[i]);
        call_t c = {entity->id, id, -1};
        if ((entity)->unsetnode) {
            (entity)->unsetnode(&c);
        }
    }
}

int is_node_alive(nodeid_t id) {
    node_t *node = get_node_by_id(id);
    return ((node->state != NODE_DEAD) && (node->state != NODE_UNDEF));
}

int is_node_active(nodeid_t id) {
    node_t *node = get_node_by_id(id);
    return (node->state == NODE_ACTIVE);
}


/* ************************************************** */
/* ************************************************** */
void nodes_update_mobility(void) {
    int i = nodes.size;

    while (i--) {
        node_t *node = get_node_by_id(i);
        bundle_t *bundle = get_bundle_by_id(node->bundle);
        entity_t *entity = get_entity_by_id(bundle->mobility);
        call_t call = {entity->id, node->id, -1};

#ifdef N_DAS_O 
        position_t o_position;
#endif /* N_DAS_0 */
        
        if ((node->state == NODE_DEAD) || (node->state == NODE_UNDEF)) {
            continue;
        }
        
#ifdef N_DAS_O 
        o_position.x = node->position.x;
        o_position.y = node->position.y;
        o_position.z = node->position.z;
#endif /* N_DAS_0 */
        
        entity->methods->mobility.update_position(&call);
        
#ifdef N_DAS_O 
        spadas_update(location, node, &node->position, &o_position);
#endif /* N_DAS_0 */
    }
}


/* ************************************************** */
/* ************************************************** */
position_t *get_node_position(nodeid_t id) {
    node_t *node = get_node_by_id(id);
    return &(node->position);
}

double distance(position_t *position0, position_t *position1) {
    double x = position0->x - position1->x;
    double y = position0->y - position1->y;
    double z = position0->z - position1->z;

    return sqrt(x*x + y*y + z*z);
}


/* ************************************************** */
/* ************************************************** */
int get_node_count(void) {
    return nodes.size;
}
