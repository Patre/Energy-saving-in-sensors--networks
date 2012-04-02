/**
 *  \file   entity_config.c
 *  \brief  Entity configuration module
 *  \author Guillaume Chelius
 *  \date   2007
 */
#include "entity_config.h"


/* ************************************************** */
/* ************************************************** */
void parse_entity_init(int id) {
    entity_t *entity = get_entity_by_id(id);

    entity->id = id;
    entity->name = NULL;
    entity->library.name = NULL;
    entity->library.module = NULL;
    entity->library.file = NULL;
    entity->model = NULL;
    entity->methods = NULL;
    entity->init = NULL;
    entity->destroy = NULL;
    entity->bootstrap = NULL;
    entity->setnode = NULL;
    entity->unsetnode = NULL;
    entity->ioctl = NULL;
    entity->private = NULL;
    entity->bundles.size = 0;
    entity->bundles.elts = NULL;
}


/* ************************************************** */
/* ************************************************** */
int parse_entity_library(xmlNodePtr nd1, entity_t *entity) {
    xmlAttrPtr attr;
    for (attr = nd1->properties; attr; attr = attr->next) {
        if (!strcmp((char *) attr->name, XML_A_LIBRARY)) {
            entity->library.name = strdup((char *) attr->children->content);
        } else if (!strcmp((char *) attr->name, XML_A_NAME)) {
            entity->name = strdup((char *) attr->children->content);
        }
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
int parse_entity_module(entity_t *entity) {
    char **path = NULL;
    char *file = NULL;

    for (path = user_path_list ; *path ; path++) {
        entity->library.file = file = g_module_build_path(*path, entity->library.name);
        if (!g_file_test(file, G_FILE_TEST_EXISTS)) {
	    free(file);
            continue;
        }
        if ((entity->library.module = g_module_open(file, G_MODULE_BIND_LOCAL))) {
            break;
        }
        goto load_model_error;
    }
    
    for (path = sys_path_list ; *path ; path++) {
        entity->library.file = file = g_module_build_path(*path, entity->library.name);
        
        if (!g_file_test(file, G_FILE_TEST_EXISTS)) {
            continue;
        }
        if ((entity->library.module = g_module_open(file, G_MODULE_BIND_LOCAL))) {
            break;
        }
        
    load_model_error:
        fprintf(stderr, "config: entity %s, unable to load library '%s' (%s) (parse_entity())\n", entity->name, entity->library.name, g_module_error());
        return -1;
    }
    
    if (entity->library.module == NULL) {
        fprintf(stderr, "config: entity %s: Unable to load model '%s' (%s) (parse_entity())\n", entity->name, entity->library.name, "Not found");
        return -1;
    }
    
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int parse_entity_symbols(entity_t *entity) {
    char *s;

    if (g_module_symbol(entity->library.module, s = "model",  (gpointer *) &(entity->model)) != TRUE) {
        fprintf(stderr, "config: model %s: unable to find symbol '%s' (parse_entity())\n", entity->library.name, s);
        return -1;
    }    
    if (g_module_symbol(entity->library.module, s = "init",  (gpointer *) &(entity->init)) != TRUE) {
        entity->init = NULL;
    }
    if (g_module_symbol(entity->library.module, s = "destroy",  (gpointer *) &(entity->destroy)) != TRUE) {
        entity->destroy = NULL;
    }
    if (g_module_symbol(entity->library.module, s = "bootstrap",  (gpointer *) &(entity->bootstrap)) != TRUE) {
        entity->bootstrap = NULL;
    }
    if (g_module_symbol(entity->library.module, s = "setnode",  (gpointer *) &(entity->setnode)) != TRUE) {
        entity->setnode = NULL;
    }
    if (g_module_symbol(entity->library.module, s = "unsetnode",  (gpointer *) &(entity->unsetnode)) != TRUE) {
        entity->unsetnode = NULL;
    }
    if (g_module_symbol(entity->library.module, s = "ioctl",  (gpointer *) &(entity->ioctl)) != TRUE) {
        entity->ioctl = NULL;
    }
    if (g_module_symbol(entity->library.module, s = "methods",  (gpointer *) &(entity->methods)) != TRUE) {
        entity->methods = NULL;
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
int parse_entity_measures(entity_t *entity) {
    if ((entity->model->measure.count > 0) 
        && (entity->model->type != MODELTYPE_ENVIRONMENT)) {
        fprintf(stderr, "config: model %s: only 'environment' model type can export measures (parse_entity())\n", entity->name);
        return -1;
    }
    measures.size += entity->model->measure.count;
    
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int parse_entity_args(xmlNodePtr nd1, entity_t *entity) {
    xmlNodePtr nd2;
    xmlAttrPtr attr;
    param_t *param;
    
    for (nd2 = nd1->children; nd2; nd2 = nd2->next) {
        if ((nd2->type == XML_ELEMENT_NODE) && 
            (!strcmp((char *) nd2->name, XML_E_DEFAULT))) {
            dflt_param_t *dflt_param;

            if ((dflt_param = (dflt_param_t *) malloc(sizeof(dflt_param_t))) == NULL) {
                fprintf(stderr, "config: malloc error (parse_entity())\n");
                return -1;
            }
            dflt_param->entityid = entity->id;
            dflt_param->bundleid = -1;
            dflt_param->nodeid = -1;
            if ((dflt_param->params = das_create()) == NULL) {
                return -1;
            }
            
            for (attr = nd2->properties; attr; attr = attr->next) {
                if ((param = (param_t *) malloc(sizeof(param_t))) == NULL) {
                    fprintf(stderr, "config: malloc error (parse_entity())\n");
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
int parse_entity_bundle(entity_t *entity) {
    int i;

    entity->bundles.size = bundles.size;
    if ((entity->bundles.elts = (int *) malloc(sizeof(int) * bundles.size)) == NULL) {
        fprintf(stderr, "config: malloc error (parse_entity())\n");
        return -1;
    }

    for (i = 0; i < bundles.size; i++) {
        entity->bundles.elts[i] = -1;
    }
 
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int parse_entity_initialize(xmlNodePtr nd1, entity_t *entity) {
    void *params;
    param_t *param;
    xmlNodePtr nd2;
    xmlAttrPtr attr;
    int ok;
    call_t c = {entity->id, -1, -1};
    
    if ((params = das_create()) == NULL) {
        return -1;
    }

    for (nd2 = nd1->children; nd2; nd2 = nd2->next) {
        if ((nd2->type == XML_ELEMENT_NODE) && 
            (!strcmp((char *) nd2->name, XML_E_INIT))) {
            for (attr = nd2->properties; attr; attr = attr->next) {
                if ((param = (param_t *) malloc(sizeof(param_t))) == NULL) {
                    fprintf(stderr, "config: malloc error (parse_entity())\n");
                    return -1;
                }
                param->key = (char *) attr->name;
                param->value = (char *) attr->children->content;
                das_insert(params, param);
            }
        }
    }
    
    ok = entity->init ? entity->init(&c, params) : 0;
    
    while ((param = (param_t *) das_pop(params)) != NULL) {
        free(param);
    }        
    das_destroy(params);
    
    return ok;
}


/* ************************************************** */
/* ************************************************** */
int parse_entity(xmlNodePtr nd1, entityid_t id) {
    entity_t *entity = get_entity_by_id(id);
    
    if (parse_entity_library(nd1, entity)) {
        return -1;
    }
    if (parse_entity_module(entity)) {
        return -1;
    }
    if (parse_entity_symbols(entity)) {
        return -1;
    }
    if (parse_entity_measures(entity)) {
        return -1;
    }
    if (parse_entity_args(nd1, entity)) {
        return -1;
    }
    if (parse_entity_bundle(entity)) {
        return -1;
    }

    print_entity(entity);
    
    if (parse_entity_initialize(nd1, entity)) {
        return -1;
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
int parse_entities(xmlNodeSetPtr nodeset) {
    int i;

    if ((entities.elts = (entity_t *) malloc(sizeof(entity_t) * entities.size)) == NULL) {
        fprintf(stderr, "config: malloc error (parse_entities())\n");
        return -1;
    }

    for (i = 0 ; i < entities.size ; i++) {
        parse_entity_init(i);
    }
       
    for (i = 0 ; i < entities.size ; i++) {
        if (parse_entity(nodeset->nodeTab[i], i)) {
            return -1;
        }
    }
    
    return 0;
}

