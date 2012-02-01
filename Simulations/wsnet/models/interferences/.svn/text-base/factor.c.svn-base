/**
 *  \file   factor.c
 *  \brief  Correlation factor
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Correlation factor",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_INTERFERENCES, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct entitydata {
	double factor;
};


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;
    
    /* default value */
    entitydata->factor = 0.5;

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "factor")) {
            if (get_param_double(param->value, &(entitydata->factor))) {
                goto error;
            }
        }
    }
    
    set_entity_private_data(c, entitydata);
    return 0;

 error:
    free(entitydata);
    return -1;
}

int destroy(call_t *c) {
    free(get_entity_private_data(c));
    return 0;
}


/* ************************************************** */
/* ************************************************** */
double interfere(call_t *c, int channel0, int channel1) {
    struct entitydata *entitydata = get_entity_private_data(c);
    if (channel0 == channel1) {
        return 1;
    } else {
        return entitydata->factor;
    }
}

/* ************************************************** */
/* ************************************************** */
interferences_methods_t methods = {interfere};
