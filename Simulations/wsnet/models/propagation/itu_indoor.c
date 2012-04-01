/**
 *  \file   indoor.c
 *  \brief  ITU Indoor Propagation Model
 *  \author Doreid Ammar and Elyès Ben Hamida
 *  \date   2009
 **/
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "ITU Indoor Propagation Model",
    "Doreid Ammar and Elyès Ben Hamida",
    "0.1",
    MODELTYPE_PROPAGATION, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct entitydata {
    double frequency;		/* The carrier frequency in MHz           */
    double distpower_lc;	/* The distance power loss coefficient    */
    int n_floors;		/* The number of floors penetrated        */
    double Lf;			/* The floor penetration loss factor (dB) */
    double cst;
};


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;
    
    /* default values */
    entitydata->frequency    = 868;
    entitydata->distpower_lc = 33;
    entitydata->n_floors     = 2;
    entitydata->Lf           = -1;
    entitydata->cst          = 0;

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "frequency_MHz")) {
            if (get_param_double(param->value, &(entitydata->frequency))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "distpower_lc")) {
            if (get_param_double(param->value, &(entitydata->distpower_lc))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "n_floors")) {
            if (get_param_integer(param->value, &(entitydata->n_floors))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "Lf")) {
            if (get_param_double(param->value, &(entitydata->Lf))) {
                goto error;
            }
        }
    }

    if (entitydata->Lf == -1) {
	entitydata->Lf = 15 + 4 * (entitydata->n_floors - 1);
    } 

    /* precomputed value for optimizing the simulation speedup */
    entitydata->cst = 20 * log10(entitydata->frequency) + entitydata->Lf - 28;

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

double propagation(call_t *c, packet_t *packet, nodeid_t src, nodeid_t dst, double rxdBm) {
    struct entitydata *entitydata = get_entity_private_data(c);
    double dist = distance(get_node_position(src), get_node_position(dst));
    double L_dB = 0;

    /*
     * Pr_dBm = Pt + Gt + Gr - L
     *
     * L_dB = 20 * log10(f) + N * log10(dist) + Lf(n) - 28
     * 
     * For the frequency 1.8-2.0 GHz
     * Lf(n) is the floor penetration loss factor (dB)
     * Lf(n) = 15 + 4(n-1); (n >= 1)
     *
     * Note: rxdBm = [Pt + Gt + Gr]_dBm
     *
     * ref1: recommendation ITU-R P.1238-1, propagation data and prediction methods for the planning of indoor radiocommunication systems, 1999.
     * ref2: http://en.wikipedia.org/wiki/ITU_Model_for_Indoor_Attenuation, 2009
     *
     */

     L_dB = entitydata->cst + entitydata->distpower_lc * log10(dist);

     return (rxdBm - L_dB);
}

/* ************************************************** */
/* ************************************************** */
propagation_methods_t methods = {propagation};

