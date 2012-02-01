/**
 *  \file   config.c
 *  \brief  XML parser & configuration module
 *  \author Guillaume Chelius & Stephane d'Alu
 *  \date   2007
 */
#include "bundle_config.h"
#include "entity_config.h"
#include "measure_config.h"
#include "environment_config.h"
#include "node_config.h"


/* ************************************************** */
/* ************************************************** */
#define DEFAULT_CONFIGFILE "config.xml"
#define DEFAULT_SCHEMAFILE DATADIR"/config.xsd"
#define DEFAULT_MODULESDIR LIBDIR"/"
#define ENV_MODDIR         "WSNET_MODDIR"

static char *schemafile = DEFAULT_SCHEMAFILE;  
static char *configfile = DEFAULT_CONFIGFILE;
static char *user_modulesdir = NULL;
static char *sys_modulesdir = DEFAULT_MODULESDIR;
gchar **user_path_list;
gchar **sys_path_list;

void *dflt_params = NULL;


/* ************************************************** */
/* ************************************************** */
void config_set_schemafile(char *s) {
    schemafile = s;
}
void config_set_configfile(char *c) {
    configfile = c;
}
void config_set_sys_modulesdir(char *c) {
    sys_modulesdir = c;
}
void config_set_usr_modulesdir(void) {
    char *env_moddir = getenv(ENV_MODDIR);
    user_modulesdir = env_moddir ? env_moddir : "";
}


/* ************************************************** */
/* ************************************************** */
void print_simulation(void) {
    fprintf(stderr, "\nSimulation will run using:\n");
    fprintf(stderr, "  nodes       : %d\n", nodes.size);
    fprintf(stderr, "  (x,y,z)     : (%lf, %lf, %lf)\n", get_topology_area()->x, get_topology_area()->y, get_topology_area()->z);
    if (scheduler_get_end()) {
        fprintf(stderr, "  duration    : %" PRId64 "ns\n", scheduler_get_end());
    } else {
        fprintf(stderr, "  duration    : unlimited\n");
    }
    
}


/* ************************************************** */
/* ************************************************** */
static void 
xml_error(void *dummy, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

static void 
xml_warning(void *dummy, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}


/* ************************************************** */
/* ************************************************** */
typedef struct _xmlpathobj {
    xmlXPathObjectPtr *ptr;
    xmlChar *expr;
} xmlpathobj_t; 


/* ************************************************** */
/* ************************************************** */
int parse_simulation(xmlNodeSetPtr nodeset) {
    xmlAttrPtr attr;
    uint64_t duration;
    
    if (nodeset == NULL) {
        fprintf(stderr, "config: schema must require one '" XML_E_SIMULATION "' (parse_simulation())\n");
        return -1;
    }
    
    /* retrieve @nodes / @duration / @x / @y / @z */
    for (attr = nodeset->nodeTab[0]->properties ; attr ; attr = attr->next) {
        if (! strcmp((char *) attr->name, XML_A_NODES)) {
            /* xsd: 1 <= @nodes <= 65535 */
            nodes.size = strtoll((char *) attr->children->content, NULL, 10);
        } else if (! strcmp((char *) attr->name, XML_A_DURATION)) {
            /* xsd: 0 <= @duration */
            get_param_time((char *) attr->children->content, &duration);
            scheduler_set_end(duration);
        } else if (! strcmp((char *) attr->name, XML_A_X)) {
            /* xsd: 0 <= @x */
            get_topology_area()->x = strtod((char *) attr->children->content, NULL);
        } else if (! strcmp((char *) attr->name, XML_A_Y)) {
            /* xsd: 0 <= @y */
            get_topology_area()->y = strtod((char *) attr->children->content, NULL);
        } else if (! strcmp((char *) attr->name, XML_A_Z)) {
            /* xsd: 0 <= @z */
            get_topology_area()->z = strtod((char *) attr->children->content, NULL);
        }
    }

    print_simulation();
    return 0;
}





/* ************************************************** */
/* ************************************************** */
void clean_params() {
    if (dflt_params) {
        dflt_param_t *dflt_param;
        
        while ((dflt_param = (dflt_param_t *) das_pop(dflt_params)) != NULL) {
            param_t *param;
            
            while ((param = (param_t *) das_pop(dflt_param->params)) != NULL) {
                free(param);
            }
            das_destroy(dflt_param->params);
            free(dflt_param);
        }
        
        das_destroy(dflt_params);
    }   
}

void *get_entity_params(nodeid_t node, entityid_t entity, bundleid_t bundle) {
    dflt_param_t *dflt_param;
    dflt_param_t *best = NULL;
    int match = 0;

    das_init_traverse(dflt_params);

    while ((dflt_param = (dflt_param_t *) das_traverse(dflt_params)) != NULL) {
        int c_match = 0;
        
        if ((dflt_param->nodeid != -1) && (dflt_param->nodeid != node))
            continue;
        if (dflt_param->entityid != entity)
            continue;
        if ((dflt_param->bundleid != -1) && (dflt_param->bundleid != bundle))
            continue;
        
        if (dflt_param->nodeid == node) c_match++;
        if (dflt_param->entityid == entity) c_match++;
        if (dflt_param->bundleid == bundle) c_match++;

         if (c_match > match) {
            best = dflt_param;
            match = c_match;
        }
    }   
    
    if (best == NULL)
        return best;
    return best->params;
}


/* ************************************************** */
/* ************************************************** */
int do_configuration(void) {
    xmlSchemaValidCtxtPtr sv_ctxt = NULL;
    xmlSchemaParserCtxtPtr sp_ctxt = NULL;
    xmlSchemaPtr schema = NULL;
    xmlParserCtxtPtr p_ctxt = NULL;
    xmlDocPtr doc = NULL;
    xmlXPathContextPtr xp_ctx = NULL; 
    xmlXPathObjectPtr simul_xobj = NULL;
    xmlXPathObjectPtr entity_xobj = NULL; 
    xmlXPathObjectPtr environment_xobj = NULL; 
    xmlXPathObjectPtr bundle_xobj = NULL; 
    xmlXPathObjectPtr node_xobj = NULL; 
    xmlNodeSetPtr nodeset;
    xmlpathobj_t xpathobj[] = {{&simul_xobj, (xmlChar *) XML_X_SIMULATION}, 
                               {&entity_xobj, (xmlChar *) XML_X_ENTITY},
                               {&environment_xobj, (xmlChar *) XML_X_ENVIRONMENT},
                               {&bundle_xobj, (xmlChar *) XML_X_BUNDLE},
                               {&node_xobj, (xmlChar *) XML_X_NODE}};
    int ok = 0, i;

    /* Check XML version */
    LIBXML_TEST_VERSION;
        
    /* Initialise and parse schema */
    sp_ctxt = xmlSchemaNewParserCtxt(schemafile);
    if (sp_ctxt == NULL) {
        fprintf(stderr, "config: XML schema parser initialisation failure (do_configuration())\n");
        ok = -1;
        goto cleanup;
    }
    xmlSchemaSetParserErrors(sp_ctxt,
                             (xmlSchemaValidityErrorFunc)   xml_error,
                             (xmlSchemaValidityWarningFunc) xml_warning,
                             NULL);
    
    schema = xmlSchemaParse(sp_ctxt);
    if (schema == NULL) {
        fprintf(stderr, "config: error in schema %s (do_configuration())\n", schemafile);
        ok = -1;
        goto cleanup;
    }
    xmlSchemaSetValidErrors(sv_ctxt,
                            (xmlSchemaValidityErrorFunc)   xml_error,
                            (xmlSchemaValidityWarningFunc) xml_warning,
                            NULL);
    
    sv_ctxt = xmlSchemaNewValidCtxt(schema);
    if (sv_ctxt == NULL) {
        fprintf(stderr, "config: XML schema validator initialisation failure (do_configuration())\n");
        ok = -1;
        goto cleanup;
    }
    
    /* Initialise and parse document */
    p_ctxt = xmlNewParserCtxt();
    if (p_ctxt == NULL) {
        fprintf(stderr, "config: XML parser initialisation failure (do_configuration())\n");
        ok = -1;
        goto cleanup;
    }
    
    doc = xmlCtxtReadFile(p_ctxt, configfile, NULL, XML_PARSE_NONET | XML_PARSE_NOBLANKS | XML_PARSE_NSCLEAN);
    if (doc == NULL) {
        fprintf(stderr, "config: failed to parse %s (do_configuration())\n", configfile);
        ok = -1;
        goto cleanup;
    }

    /* Validate document */
    if (xmlSchemaValidateDoc(sv_ctxt, doc)) {
        fprintf(stderr, "config: error in configuration file %s (do_configuration())\n", configfile);
        ok = -1;
        goto cleanup;
    }
    
    /* Create xpath context */
    xp_ctx = xmlXPathNewContext(doc);
    if (xp_ctx == NULL) {
        fprintf(stderr, "config: XPath initialisation failure (do_configuration())\n");
        ok = -1;
        goto cleanup;
    }
    xmlXPathRegisterNs(xp_ctx, (xmlChar *) XML_NS_ID, (xmlChar *) XML_NS_URL); 
    
    
    /* Get xpath obj */
    for (i = 0 ; i < (int) (sizeof(xpathobj) / sizeof(xpathobj[0])); i++) {
        *xpathobj[i].ptr = xmlXPathEvalExpression(xpathobj[i].expr, xp_ctx);
        if (*xpathobj[i].ptr == NULL) {
            fprintf(stderr, "config: unable to evaluate xpath \"%s\" (do_configuration())\n", xpathobj[i].expr);
            ok = -1;
            goto cleanup;
            
        }
    }

    /***************/
    /* Counting... */
    /***************/
    nodeset = entity_xobj->nodesetval;
    if ((entities.size = (nodeset) ? nodeset->nodeNr : 0) == 0) {
        fprintf(stderr, "config: no entity defined (do_configuration())\n");
        ok = -1; 
        goto cleanup;
    }
    fprintf(stderr, "\nFound %d entities...\n", entities.size);
    nodeset = environment_xobj->nodesetval;
    if (((nodeset) ? nodeset->nodeNr : 0) == 0) {
        fprintf(stderr, "config: no environment defined (do_configuration())\n");
        ok = -1; 
        goto cleanup;
    }
    fprintf(stderr, "Found 1 environment...\n");
    nodeset = bundle_xobj->nodesetval;
    if ((bundles.size = (nodeset) ? nodeset->nodeNr : 0) == 0) {
        fprintf(stderr, "config: no bundle defined (do_configuration())\n");
        ok = -1; 
        goto cleanup;
    }
    fprintf(stderr, "Found %d bundles...\n", bundles.size);
    

    if ((dflt_params = das_create()) == NULL) {
        ok = -1; 
        goto cleanup;
    }

    /**************/
    /* Simulation */
    /**************/
    if (parse_simulation(simul_xobj->nodesetval)) {
        ok = -1;
        goto cleanup;
    }
                                                          
    /**********/
    /* Entity */
    /**********/
    /* initialize library paths */
    config_set_usr_modulesdir();
    user_path_list = g_strsplit(user_modulesdir, ":", 0); /* TOCLEAN */
    sys_path_list = g_strsplit(sys_modulesdir, ":", 0); /* TOCLEAN */

    /* parse */
    if (parse_entities(entity_xobj->nodesetval)) {
        ok = -1;
        goto cleanup;
    }

    /**************/
    /* Measure    */
    /**************/
    if (parse_measure()) {
        ok = -1;
        goto cleanup;
    }

    /***************/
    /* Environment */
    /***************/
    if (parse_environment(environment_xobj->nodesetval)) {
        ok = -1;
        goto cleanup;
    }
    
    /***************/
    /* Bundle      */
    /***************/
    if (parse_bundles(bundle_xobj->nodesetval)) {
        ok = -1;
        goto cleanup;
    }

    /***************/
    /* Nodes      */
    /***************/
    if (parse_nodes(node_xobj->nodesetval)) {
        ok = -1;
        goto cleanup;
    }
 
    /* edit by Quentin Lampin <quentin.lampin@orange-ftgroup.com> */
    gchar **path = NULL;
    for (path = user_path_list ; *path ; path++) {
        g_free(*path);
    }
    path = NULL;
    for (path = sys_path_list  ; *path ; path++) {
        g_free(*path);
    }
    /* end of edition */

 cleanup:
    clean_params();
    
    for (i = 0 ; i < (int) (sizeof(xpathobj) / sizeof(xpathobj[0])); i++) {
        xmlXPathFreeObject(*xpathobj[i].ptr);
    }

    if (xp_ctx) {
        xmlXPathFreeContext(xp_ctx);
    }

    if (sp_ctxt) {
        xmlSchemaFreeParserCtxt(sp_ctxt);		
    }

    if (schema) {
        xmlSchemaFree(schema);
    }

    if (sv_ctxt) {
        xmlSchemaFreeValidCtxt(sv_ctxt);
    }

    if (doc) {
        xmlFreeDoc(doc);
    }

    if (p_ctxt) {
        xmlFreeParserCtxt(p_ctxt);
    }

    xmlCleanupParser();
    return ok;
}

