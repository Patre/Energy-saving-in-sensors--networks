/**
 *  \file   measure_config.c
 *  \brief  Measure configuration module
 *  \author Guillaume Chelius
 *  \date   2007
 */
#include "measure_config.h"


/* ************************************************** */
/* ************************************************** */
int parse_measure(void) {
    int i, j, k;

    if (measures.size != 0) {
        int cnt = 0;
        
        if ((measures.elts = (measure_t *) malloc(sizeof(measure_t) * measures.size)) == NULL) {
            fprintf(stderr, "config: malloc error (parse_measure())\n");
            return -1;
        }
        
        for (i = 0 ; i < entities.size ; i++) {
            entity_t *entity = get_entity_by_id(i);
            
            for (j = 0; j < entity->model->measure.count; j++) {
                measure_t *measure;
                
                for (k = 0 ; k < cnt; k++) {
                    if (strcmp(entity->model->measure.exported[j], 
                               get_measure_by_id(k)->name) == 0) {
                        fprintf(stderr, "config: measure %s exported more than once (parse_measure())\n", entity->model->measure.exported[j]);
                        return -1;
                    }
                }
                measure = get_measure_by_id(cnt);
                measure->id = cnt++;
                measure->name = entity->model->measure.exported[j];
                measure->entity = entity;
                print_measure(measure);
            }
        }
    }
    
    return 0;
}

