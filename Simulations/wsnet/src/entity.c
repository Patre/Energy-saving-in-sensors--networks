/**
 *  \file   entity.c
 *  \brief  Entity module
 *  \author Guillaume Chelius
 *  \date   2007
 */
#include "internals.h"


/* ************************************************** */
/* ************************************************** */
entity_array_t entities = {0, NULL};


/* ************************************************** */
/* ************************************************** */
entityid_t get_entityid_by_name(char *name) {
    int i = entities.size;
    while (i--)
	if (!strcmp(name, (entities.elts + i)->name))
	    break;
    return i;
}

entity_t *get_entity_by_id(entityid_t id) {
    return entities.elts + id;
}
 
entity_t *get_entity_by_name(char *name) {
    entityid_t id = get_entityid_by_name(name);
    return (id < 0) ? NULL : get_entity_by_id(id);
}
 
char *get_entity_name(call_t *c) {
    entity_t *entity = entities.elts + c->entity;
    return entity->name;
}
 
 
 
/* ************************************************** */
/* ************************************************** */
int entity_init(void) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int entity_bootstrap(void) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void entity_clean(void) {
    int i;

    for (i = 0; i < entities.size; i++) {
        entity_t *entity = get_entity_by_id(i);

        if (entity->library.name) {
            free(entity->library.name);
        }

        if (entity->library.file) {
            g_free(entity->library.file);
        }

        if (entity->name) {
            free(entity->name);
        }

        if (entity->bundles.elts) {
            free(entity->bundles.elts);
        }
        if (entity->library.module) {
            g_module_close(entity->library.module);
        }
    }

    if (entities.size) {
        g_free(entities.elts);
    }
}


/* ************************************************** */
/* ************************************************** */
void print_entity(entity_t *entity) {
    fprintf(stderr, "\nEntity '%s' (%d)\n",    entity->name, entity->id);
    fprintf(stderr, "    using model  : %s\n", entity->library.name);
    fprintf(stderr, "    using plugin : %s\n", entity->library.file);
    fprintf(stderr, "    author       : %s\n", entity->model->author);
    fprintf(stderr, "    version      : %s\n", entity->model->version);
    fprintf(stderr, "    description  : %s\n", entity->model->oneline);                
}


/* ************************************************** */
/* ************************************************** */
void *get_entity_private_data(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    return entity->private;
}

void set_entity_private_data(call_t *c, void *data) {
    entity_t *entity = get_entity_by_id(c->entity);
    entity->private = data;
}

void *get_node_private_data(call_t *c) {
    node_t *node = get_node_by_id(c->node);
    entity_t *entity = get_entity_by_id(c->entity);

    return node->private[entity->bundles.elts[node->bundle]];
}

void set_node_private_data(call_t *c, void *data) {
    node_t *node = get_node_by_id(c->node);
    entity_t *entity = get_entity_by_id(c->entity);

    node->private[entity->bundles.elts[node->bundle]] = data;
}


/* ************************************************** */
/* ************************************************** */
int get_entity_type(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    return entity->model->type;
}


/* ************************************************** */
/* ************************************************** */
array_t *get_entity_bindings_up(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    node_t   *node   = get_node_by_id(c->node);
    bundle_t *bundle = get_bundle_by_id(node->bundle);
    
    return bundle->up + entity->bundles.elts[bundle->id];
}

array_t *get_entity_bindings_down(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    node_t   *node   = get_node_by_id(c->node);
    bundle_t *bundle = get_bundle_by_id(node->bundle);
    
    return bundle->down + entity->bundles.elts[bundle->id];
}


/* ************************************************** */
/* ************************************************** */

static inline bundle_t *get_bundle_for_node(call_t *c) {
    node_t *node = get_node_by_id(c->node);
    return get_bundle_by_id(node->bundle);
}


array_t *get_application_entities(call_t *c) {
    return &get_bundle_for_node(c)->application;
}

array_t *get_routing_entities(call_t *c) {
    return &get_bundle_for_node(c)->routing;
}

array_t *get_mac_entities(call_t *c) {
    return &get_bundle_for_node(c)->mac;
}

array_t *get_radio_entities(call_t *c) {
    return &get_bundle_for_node(c)->radio;
}

array_t *get_antenna_entities(call_t *c) {
    return &get_bundle_for_node(c)->antenna;
}

entityid_t get_energy_entity(call_t *c) {
    return get_bundle_for_node(c)->energy;
}

entityid_t get_mobility_entity(call_t *c) {
    return get_bundle_for_node(c)->mobility;
}


/* ************************************************** */
/* ************************************************** */
/* Deprecated */
int get_entity_links_up_nbr(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    node_t   *node   = get_node_by_id(c->node);
    bundle_t *bundle = get_bundle_by_id(node->bundle);
    
    return bundle->up[entity->bundles.elts[bundle->id]].size;
}

int *get_entity_links_up(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    node_t   *node   = get_node_by_id(c->node);
    bundle_t *bundle = get_bundle_by_id(node->bundle);
    
    return bundle->up[entity->bundles.elts[bundle->id]].elts;
}

int get_entity_links_down_nbr(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    node_t   *node   = get_node_by_id(c->node);
    bundle_t *bundle = get_bundle_by_id(node->bundle);
    
    return bundle->down[entity->bundles.elts[bundle->id]].size;
}

int *get_entity_links_down(call_t *c) {
    entity_t *entity = get_entity_by_id(c->entity);
    node_t   *node   = get_node_by_id(c->node);
    bundle_t *bundle = get_bundle_by_id(node->bundle);
    
    return bundle->down[entity->bundles.elts[bundle->id]].elts;
}


