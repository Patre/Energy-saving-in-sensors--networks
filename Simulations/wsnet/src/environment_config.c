/**
 *  \file   environment_config.c
 *  \brief  Environment configuration module
 *  \author Guillaume Chelius
 *  \date   2007
 */
#include "environment_config.h"


/* ************************************************** */
/* ************************************************** */
int parse_environment(xmlNodeSetPtr nodeset) {
    xmlNodePtr nd2;
    xmlNodePtr nd1;
    xmlAttrPtr attr;
    
    nd1 = nodeset->nodeTab[0];
    
    /* parse elements */
    with_entities = das_create();
    modulation_entities = das_create();
    for (nd2 = nd1->children; nd2; nd2 = nd2->next ) {
        if (!strcmp((gchar *) nd2->name, XML_E_WITH)) {
            entity_t *entity;
            attr = nd2->properties;
            
            entity = get_entity_by_name((char *) attr->children->content);
            das_insert(with_entities, entity);
        } else if (!strcmp((gchar *) nd2->name, XML_E_MODULATION)) {
            entity_t *entity;
            attr = nd2->properties;
            
            entity = get_entity_by_name((char *) attr->children->content);
            das_insert(modulation_entities, entity);
        } else if (!strcmp((gchar *) nd2->name, XML_E_MONITORING)) {
            entity_t *entity;
            attr = nd2->properties;
            
            entity = get_entity_by_name((char *) attr->children->content);
            monitor_entity = entity;
        } else if (!strcmp((gchar *) nd2->name, XML_E_PROPAGATION)) {
            for (attr = nd2->properties; attr; attr = attr->next) {
                if (! strcmp((gchar *) attr->name, XML_A_ENTITY)) {
                    entity_t *entity = get_entity_by_name((char *) attr->children->content);
                    
                    propagation_entity = entity;
                } else if (! strcmp((gchar *) attr->name, XML_A_RANGE)) {
                    propagation_range = strtod((char *) attr->children->content, NULL);
                }
            }
        } else if (!strcmp((gchar *) nd2->name, XML_E_INTERFERENCES)) {
            entity_t *entity;
            attr = nd2->properties;
            
            entity = get_entity_by_name((char *) attr->children->content);
            interference_entity = entity;
        } else if (!strcmp((gchar *) nd2->name, XML_E_NOISE)) {
            entity_t *entity;
            attr = nd2->properties;
            
            entity = get_entity_by_name((char *) attr->children->content);
            noise_entity = entity;
        }            
    }
    
    print_environment();
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void print_environment(void) {
    entity_t *entity;

    das_init_traverse(with_entities);
    das_init_traverse(modulation_entities);

    fprintf(stderr, "\nEnvironment\n");
    fprintf(stderr, "    using propagation   : %s\n", propagation_entity->name);
    fprintf(stderr, "    propagation range   : %lf\n", propagation_range);
    fprintf(stderr, "    using interferences : %s\n", interference_entity->name);
    if (noise_entity) {
        fprintf(stderr, "    using noise         : %s\n", noise_entity->name);
    }
    if (monitor_entity) {
        fprintf(stderr, "    using monitoring    : %s\n", monitor_entity->name);
    }
    while ((entity = (entity_t *) das_traverse(with_entities)) != NULL) {
        fprintf(stderr, "    with                : %s\n", entity->name);
    }
    while ((entity = (entity_t *) das_traverse(modulation_entities)) != NULL) {
        fprintf(stderr, "    modulation          : %s\n", entity->name);
    }
}

