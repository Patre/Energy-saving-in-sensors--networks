/**
 *  \file   white.c
 *  \brief  White noise model
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "White noise",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_NOISE, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct entitydata {
	double white_mW;
};


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;
    double dBm = MIN_DBM;

    /* default values */
    entitydata->white_mW = 0.0;

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "white-noise-dBm")) {
            if (get_param_double(param->value, &(dBm))) {
                goto error;
            }
        }
    }
    
    if (dBm != MIN_DBM) {
      entitydata->white_mW = dBm2mW(dBm);
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
double noise(call_t *c, nodeid_t node, int channel) {
    struct entitydata *entitydata = get_entity_private_data(c);
    return entitydata->white_mW;
}


/* ************************************************** */
/* ************************************************** */
noise_methods_t methods = {noise};
