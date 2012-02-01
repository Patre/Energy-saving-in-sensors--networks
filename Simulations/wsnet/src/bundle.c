/**
 *  \file   bundle.c
 *  \brief  Bundle module
 *  \author Guillaume Chelius
 *  \date   2007
 */
#include "internals.h"


/* ************************************************** */
/* ************************************************** */
bundle_array_t bundles = {0, NULL};


/* ************************************************** */
/* ************************************************** */
bundleid_t get_bundleid_by_name(char *name) {
    int i = bundles.size;
    while (i--)
        if (!strcmp(name, (bundles.elts + i)->name))
            break;
    return i;
}

bundle_t *get_bundle_by_name(char *name) {
    bundleid_t id = get_bundleid_by_name(name);
    return (id < 0) ? NULL : get_bundle_by_id(id);
}
 
bundle_t *get_bundle_by_id(bundleid_t id) {
    return bundles.elts + id;
}

 
/* ************************************************** */
/* ************************************************** */
int bundle_init(void) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bundle_bootstrap(void) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void bundle_clean(void) {
    int i, j;

    if (bundles.elts == NULL) {
        return;
    }
    
    for (i = 0; i < bundles.size; i++) {
        bundle_t *bundle = get_bundle_by_id(i);
        
        free(bundle->name);
        
        if (bundle->antenna.size) {
            free(bundle->antenna.elts);
        }
        if (bundle->radio.size) {
            free(bundle->radio.elts);
        }
        if (bundle->mac.size) {
            free(bundle->mac.elts);
        }
        if (bundle->routing.size) {
            free(bundle->routing.elts);
        }
        if (bundle->application.size) {
            free(bundle->application.elts);
        }
        
        if (!bundle->entity.size) {
            continue;
        }

        for (j = 0; j < bundle->entity.size; j++) {
            if (bundle->up[j].size) {
                free(bundle->up[j].elts);
            }
            if (bundle->down[j].size) {
                free(bundle->down[j].elts);
            }
        }
        
        free(bundle->up);
        free(bundle->down);
        free(bundle->entity.elts);
    }
    
    free(bundles.elts);
}


/* ************************************************** */
/* ************************************************** */
void print_bundle(bundle_t *bundle) {
    int i, j;
    fprintf(stderr, "\nBundle '%s' (%d)\n",          bundle->name, bundle->id);
    fprintf(stderr, "    worldsens             : %d\n", bundle->worldsens);
    fprintf(stderr, "    mobility              : %s\n", get_entity_by_id(bundle->mobility)->name);
    fprintf(stderr, "    number of entities    : %d\n", bundle->entity.size);
    for (i = 0; i < bundle->entity.size; i++) {
        fprintf(stderr, "                            %s\n", get_entity_by_id(bundle->entity.elts[i])->name);
        if (bundle->up[i].size) {
            fprintf(stderr, "                               up  :");
            for (j = 0; j < bundle->up[i].size; j++) {
                fprintf(stderr, " %s", get_entity_by_id(bundle->up[i].elts[j])->name);        
            }
            fprintf(stderr, "\n");
        }        
        if (bundle->down[i].size) {
            fprintf(stderr, "                               down:");
            for (j = 0; j < bundle->down[i].size; j++) {
                fprintf(stderr, " %s", get_entity_by_id(bundle->down[i].elts[j])->name);        
            }
            fprintf(stderr, "\n");
        }
    }
}
