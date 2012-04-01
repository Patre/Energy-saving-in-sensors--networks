/**
 *  \file   node_config.c
 *  \brief  Node configuration module
 *  \author Guillaume Chelius
 *  \date   2007
 */
#include "bundle_config.h"
#include "node_config.h"


/* ************************************************** */
/* ************************************************** */
int parse_nodes(xmlNodeSetPtr nodeset) {
    int i, j;
    int worldsens = 0;
    int count = (nodeset) ? nodeset->nodeNr : 0;
    xmlAttrPtr attr;
    xmlNodePtr nd1;
    xmlNodePtr nd2;
    param_t *param;
    int birth_spcfd[nodes.size];
    
    /* create nodes */
    nodes_create();

    /* check for a default bundle*/
    if (dflt_bundle == NULL) {
        fprintf(stderr, "config: no default bundle defined (parse_nodes())\n");
        return -1;
    }

    for (i = 0; i < nodes.size; i++) {
        node_t *node = get_node_by_id(i);
        node->bundle = dflt_bundle->id;
        node->birth = 0;
        birth_spcfd[i] = 0;
    }

    /* for all node definitions, eventually overide bundle */
    for (i = 0; i < count; i++) {
        char *as = NULL;
        nodeid_t nodeid = -1;

        nd1 = nodeset->nodeTab[i];
        for (attr = nd1->properties; attr; attr = attr->next) {
            if (!strcmp((char *) attr->name, XML_A_ID)) {
                nodeid = strtoll((char *) attr->children->content, NULL, 10);
            } else if (!strcmp((char *) attr->name, XML_A_AS)) {
                as = (char *) attr->children->content;
            } else if (!strcmp((char *) attr->name, XML_A_BIRTH)) {
                node_t *node = get_node_by_id(nodeid);
                birth_spcfd[nodeid] = 1;
                get_param_time((char *) attr->children->content, &(node->birth));
            }
            
            if ((as != NULL) && (nodeid != -1)) {
                node_t *node = get_node_by_id(nodeid);
                bundle_t *bundle = get_bundle_by_name(as);
                node->bundle = bundle->id;
            }
        }

        /* get entity parameters */
        for (nd2 = nd1->children; nd2; nd2 = nd2->next) {
            node_t *node = get_node_by_id(nodeid);

            if ((nd2->type == XML_ELEMENT_NODE) &&
                (!strcmp((char *)nd2->name, XML_E_FOR))) {
                dflt_param_t *dflt_param;
                
                if ((dflt_param = (dflt_param_t *) malloc(sizeof(dflt_param_t))) == NULL) {
                    fprintf(stderr, "config: malloc error (parse_nodes())\n");
                    return -1;
                }
                dflt_param->entityid = -1;
                dflt_param->bundleid = node->bundle;
                dflt_param->nodeid = node->id;
                if ((dflt_param->params = das_create()) == NULL) {
                    return -1;
                }
                
                /* retrieve entity */
                for (attr = nd2->properties ; attr ; attr = attr->next) {
                    if (!strcmp((gchar *) attr->name, XML_A_ENTITY)) {
                        entity_t *entity = get_entity_by_name((char *) attr->children->content);
                        dflt_param->entityid = entity->id;
                    } else {
                        if ((param = (param_t *) malloc(sizeof(param_t))) == NULL) {
                            fprintf(stderr, "config: malloc error (parse_nodes())\n");
                            return -1;
                        }
                        param->key = (char *) attr->name;
                        param->value = (char *) attr->children->content;
                        das_insert(dflt_param->params, param);
                    }
                }
                das_insert(dflt_params, dflt_param);
            }
        }
    }

    /* for all nodes */
    for (i = 0; i < nodes.size; i++) {
        node_t *node = get_node_by_id(i);
        bundle_t *bundle = get_bundle_by_id(node->bundle);


        /* allocate pointer array for private memory */
        if ((node->private = malloc(sizeof(void *) * bundle->entity.size)) == NULL) {
            fprintf(stderr, "config: malloc error (parse_nodes())\n");
            return -1;
        }

        /* for all entities, call setnode */
        for (j  = 0; j < bundle->entity.size; j++) {
            entity_t *entity = get_entity_by_id(bundle->entity.elts[j]);
            void *params = get_entity_params(node->id, entity->id, node->bundle);
            int create = 0;
            int ok = 0;

            if (params == NULL) {
                create = 1;
                if ((params = das_create()) == NULL) {
                    return -1;
                }
            }
            /* call setnode */
            if (entity->setnode) {
                call_t c = {entity->id, node->id, -1};
                ok = entity->setnode(&c, params);
            }

            if (create) {
                das_destroy(params);
            }
            
            /* deal with return value */
            if (ok) {
                return -1;
            }
        }

        /* program birth */
        if (birth_spcfd[i] == 0) {
            get_param_time(bundle->birth, &(node->birth));
        }

        /* heck wether it is a worldsens node and override birth in this case */
        if (bundle->worldsens == NODE_DISCONNECTED) {
            worldsens++;
            node->birth = 0;
            node->worldsens = NODE_DISCONNECTED;
        }
    }
   
    worldsens_set_count(worldsens);
    return 0;
}
