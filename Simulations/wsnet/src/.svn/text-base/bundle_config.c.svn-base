/**
 *  \file   bundle_config.c
 *  \brief  Bundle configuration module
 *  \author Guillaume Chelius
 *  \date   2007
 */
#include "bundle_config.h"


/* ************************************************** */
/* ************************************************** */
bundle_t *dflt_bundle = NULL;


/* ************************************************** */
/* ************************************************** */
void parse_bundle_init(bundle_t *bundle, int id) {
    bundle->id = id;
    bundle->name = NULL;
    bundle->worldsens = NODE_LOCAL;
    bundle->birth = DEFAULT_STRING;

    bundle->energy = -1;
    bundle->mobility = -1;
    bundle->entity.size = 0;
    bundle->antenna.size = 0;
    bundle->radio.size = 0;
    bundle->mac.size = 0;
    bundle->routing.size = 0;
    bundle->application.size = 0;

    bundle->up = NULL;
    bundle->down = NULL;
}


/* ************************************************** */
/* ************************************************** */
int parse_bundle_name(xmlNodePtr nd1, bundle_t *bundle) {
    xmlAttrPtr attr;

    for (attr = nd1->properties; attr; attr = attr->next) {
        if (!strcmp((char *) attr->name, XML_A_NAME)) {
            bundle->name = strdup((char *) attr->children->content);
            return 0;
        }
    }

    return -1;
}


/* ************************************************** */
/* ************************************************** */
int parse_bundle_birth(xmlNodePtr nd1, bundle_t *bundle) {
    xmlAttrPtr attr;

    for (attr = nd1->properties; attr; attr = attr->next) {
        if (!strcmp((gchar *) attr->name, XML_A_BIRTH)) {
            bundle->birth = (char *) attr->children->content;
        }
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
int parse_bundle_default(xmlNodePtr nd1, bundle_t *bundle) {
    xmlAttrPtr attr;

    for (attr = nd1->properties; attr; attr = attr->next) {
        if (!strcmp((char *) attr->name, XML_A_DEFAULT)) {
            gchar *v = g_strstrip((gchar *) attr->children->content);
            if  (!strcmp(v, "true") || !strcmp(v, "1")) {
                dflt_bundle = bundle;
            }
        }
    }
    
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int parse_bundle_worldsens(xmlNodePtr nd1, bundle_t *bundle) {
    xmlAttrPtr attr;

    for (attr = nd1->properties; attr; attr = attr->next) {
        if (!strcmp((char *) attr->name, XML_A_WORLDSENS)) {
            gchar *v = g_strstrip((gchar *) attr->children->content);
            if  (!strcmp(v, "true") || !strcmp(v, "1")) {
                bundle->worldsens = NODE_DISCONNECTED;
            }
        }
    }
    
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int parse_bundle_allocate(xmlNodePtr nd1, bundle_t *bundle) {
    xmlNodePtr nd2;
    int i;

    /* count antenna & entities */
    for (nd2 = nd1->children; nd2; nd2 = nd2->next) {
        bundle->entity.size++;
    }

    /* allocate antenna & entitites */
    if ((bundle->entity.elts = (int *) malloc(sizeof(int) * bundle->entity.size)) == NULL) {
        fprintf(stderr, "config: malloc error (parse_bundle())\n");
        return -1;
    }
    if ((bundle->up = (array_t *) malloc(sizeof(array_t) * bundle->entity.size)) == NULL) {
        fprintf(stderr, "config: malloc error (parse_bundle())\n");
        return -1;
    }
    if ((bundle->down = (array_t *) malloc(sizeof(array_t) * bundle->entity.size)) == NULL) {
        fprintf(stderr, "config: malloc error (parse_bundle())\n");
        return -1;
    }

    /* initialize */
    for (i = 0; i < bundle->entity.size; i++) {
        bundle->up[i].size = 0;
        bundle->down[i].size = 0;
    }
    
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int parse_bundle_types(xmlNodePtr nd1, bundle_t *bundle) {
    int i;
    int antenna_c = 0;
    int radio_c = 0;
    int mac_c = 0;
    int routing_c = 0;
    int application_c = 0;
	
     
    /* get number of entities */
    for (i = 0; i < bundle->entity.size; i++) {
        entity_t *entity = get_entity_by_id(bundle->entity.elts[i]);
        switch (entity->model->type) {
        case MODELTYPE_ANTENNA:
            bundle->antenna.size++;
            break;
        case MODELTYPE_RADIO:
            bundle->radio.size++;
            break;
        case MODELTYPE_MAC:
            bundle->mac.size++;
            break;
        case MODELTYPE_ROUTING:
            bundle->routing.size++;
            break;
        case MODELTYPE_APPLICATION:
            bundle->application.size++;
            break;
        }
    }

    /* allocate */
    if ((bundle->antenna.elts = (int *) malloc(sizeof(int) * bundle->antenna.size)) == NULL) {
        fprintf(stderr, "config: malloc error (parse_bundle())\n");
        return -1;
    }
    if ((bundle->radio.elts = (int *) malloc(sizeof(int) * bundle->radio.size)) == NULL) {
        fprintf(stderr, "config: malloc error (parse_bundle())\n");
        return -1;
    }
    if ((bundle->mac.elts = (int *) malloc(sizeof(int) * bundle->mac.size)) == NULL) {
        fprintf(stderr, "config: malloc error (parse_bundle())\n");
        return -1;
    }
    if ((bundle->routing.elts = (int *) malloc(sizeof(int) * bundle->routing.size)) == NULL) {
        fprintf(stderr, "config: malloc error (parse_bundle())\n");
        return -1;
    }
    if ((bundle->application.elts = (int *) malloc(sizeof(int) * bundle->application.size)) == NULL) {
        fprintf(stderr, "config: malloc error (parse_bundle())\n");
        return -1;
    }
    
    
    /* fill */
    for (i = 0; i < bundle->entity.size; i++) {
       entity_t *entity = get_entity_by_id(bundle->entity.elts[i]);
        switch (entity->model->type) {
        case MODELTYPE_ANTENNA:
            bundle->antenna.elts[antenna_c++] = entity->id;
            break;
        case MODELTYPE_RADIO:
            bundle->radio.elts[radio_c++] = entity->id;
            break;
        case MODELTYPE_MAC:
            bundle->mac.elts[mac_c++] = entity->id;
            break;
        case MODELTYPE_ROUTING:
            bundle->routing.elts[routing_c++] = entity->id;
            break;
        case MODELTYPE_APPLICATION:
            bundle->application.elts[application_c++] = entity->id;
            break;
        case MODELTYPE_MOBILITY:
            bundle->mobility = entity->id;
            break;
        case MODELTYPE_ENERGY:
            bundle->energy = entity->id;
            break;
        }
    }
    
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int parse_bundle_updown(xmlNodePtr nd2, bundle_t *bundle, int entity_c) {
    xmlNodePtr nd3;
    xmlAttrPtr attr;
    int up_c = 0;
    int down_c = 0;
    
    /* count attributes */
    for (nd3 = nd2->children; nd3; nd3 = nd3->next) {
        if (!strcmp((char *) nd3->name, XML_E_UP)) {
            bundle->up[entity_c].size++;                
        } else if (!strcmp((char *) nd3->name, XML_E_DOWN)) {
            bundle->down[entity_c].size++;                
        }
    }
    
    /* allocate */
    if ((bundle->up[entity_c].size) && 
        (bundle->up[entity_c].elts = 
         (int *) malloc(sizeof(int) * bundle->up[entity_c].size)) == NULL) {
        fprintf(stderr, "config: malloc error (parse_bundle())\n");
        return -1;
    }
    if ((bundle->down[entity_c].size) && 
        (bundle->down[entity_c].elts = 
         (int *) malloc(sizeof(int) * bundle->down[entity_c].size)) == NULL) {
        fprintf(stderr, "config: malloc error (parse_bundle())\n");
        return -1;
    }
    
    /* fill */
    for (nd3 = nd2->children; nd3; nd3 = nd3->next ) {
        if (!strcmp((char *) nd3->name, XML_E_UP)) {
            for (attr = nd3->properties; attr; attr = attr->next) {
                entity_t *entity;
                if ((entity = get_entity_by_name((char *) attr->children->content)) == NULL) {
                    fprintf(stderr, "config: wrong entity '%s' has been used (parse_bundle())", attr->children->content);
                    return -1;
                }
                bundle->up[entity_c].elts[up_c++] = entity->id;
            }
        } else if (!strcmp((char *) nd3->name, XML_E_DOWN)) {
            for (attr = nd3->properties; attr; attr = attr->next) {
                entity_t *entity;
                if ((entity = get_entity_by_name((char *) attr->children->content)) == NULL) {
                    fprintf(stderr, "config: wrong entity '%s' has been used (parse_bundle())", attr->children->content);
                    return -1;
                }
                bundle->down[entity_c].elts[down_c++] = entity->id;
            }
        }
    }

    return 0;
}        


/* ************************************************** */
/* ************************************************** */
int parse_bundle_args(xmlNodePtr nd2, bundle_t *bundle, entity_t *entity, int entity_c) {
    xmlNodePtr nd3;
    xmlAttrPtr attr;
    param_t *param;

    for (nd3 = nd2->children ; nd3 ; nd3 = nd3->next ) {
        if (!strcmp((char *) nd3->name, XML_E_DEFAULT)) {
            dflt_param_t *dflt_param;
                
            if ((dflt_param = (dflt_param_t *) malloc(sizeof(dflt_param_t))) == NULL) {
                fprintf(stderr, "config: malloc error (parse_bundle())\n");
                return -1;
            }
            dflt_param->entityid = entity->id;
            dflt_param->bundleid = bundle->id;
            dflt_param->nodeid = -1;
            if ((dflt_param->params = das_create()) == NULL) {
                return -1;
            }
                
            for (attr = nd3->properties; attr; attr = attr->next) {
                if ((param = (param_t *) malloc(sizeof(param_t))) == NULL) {
                    fprintf(stderr, "config: malloc error (parse_bundle())\n");
                    return -1;
                }
                param->key = (char *) attr->name;
                param->value = (char *) attr->children->content;
                das_insert(dflt_param->params, param);
            }   
            das_insert(dflt_params, dflt_param);
        }
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
int parse_bundle(xmlNodePtr nd1, int id) {
    bundle_t *bundle = get_bundle_by_id(id);
    xmlNodePtr nd2;
    xmlAttrPtr attr;
    int entity_c = 0;

    /* initialize */
    parse_bundle_init(bundle, id);

    if (parse_bundle_name(nd1, bundle)) {
        return -1;
    }
    if (parse_bundle_worldsens(nd1, bundle)) {
        return -1;
    }
    if (parse_bundle_birth(nd1, bundle)) {
        return -1;
    }
    if (parse_bundle_default(nd1, bundle)) {
        return -1;
    }
    if (parse_bundle_allocate(nd1, bundle)) {
        return -1;
    }
    
    /* retrieve entitites */
    for (nd2 = nd1->children; nd2; nd2 = nd2->next) {  
        entity_t *entity = NULL;
        
        for (attr = nd2->properties; attr; attr = attr->next) {
            if (!strcmp((gchar *) attr->name, XML_A_ENTITY)) {
                if ((entity = get_entity_by_name((char *) attr->children->content)) == NULL) {
                    fprintf(stderr, "config: wrong entity '%s' has been used (parse_bundle())", attr->children->content);
                    return -1;
                }
            }
        }
            
        /* include entity in bundle and vice-versa */
        bundle->entity.elts[entity_c] = entity->id;
        entity->bundles.elts[bundle->id] = entity_c;
        
        
        /* retrieve elements */
        if (parse_bundle_updown(nd2, bundle, entity_c)) {
            return -1;
        }
        if (parse_bundle_args(nd2, bundle, entity, entity_c)) {
            return -1;
        }
                
        entity_c++;
    }
    
    

    if (parse_bundle_types(nd1, bundle)) {
        return -1;
    }

    print_bundle(bundle);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int parse_bundles(xmlNodeSetPtr nodeset) {
    int i;

    /* allocate memory for entity list */
    if ((bundles.elts = (bundle_t *) malloc(sizeof(bundle_t) * bundles.size)) == NULL) {
        fprintf(stderr, "config: malloc error (parse_bundles())\n");
        return -1;
    }
 
    /* parse bundles */
    for (i = 0 ; i < bundles.size; i++) {
        if (parse_bundle(nodeset->nodeTab[i], i)) {
            return -1;
        }
    }
    
    return 0;
}
