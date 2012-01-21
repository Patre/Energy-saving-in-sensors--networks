/**
 *  \file   step.c
 *  \brief  Step modulation model
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Step modulation",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_MODULATION, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct entitydata {
	double step;
};


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;
    
    /* default values */
    entitydata->step = 2;

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "step")) {
            if (get_param_double(param->value, &(entitydata->step))) {
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
double modulate(call_t *c, double snr) {
    struct entitydata *entitydata = get_entity_private_data(c);
    if (snr >= entitydata->step) {
        return 0;
    } else {
        return 0.5;
    }
}

int bit_per_symbol(call_t *c){
    return 1;
}

/* ************************************************** */
/* ************************************************** */
modulation_methods_t methods = {modulate, bit_per_symbol};
