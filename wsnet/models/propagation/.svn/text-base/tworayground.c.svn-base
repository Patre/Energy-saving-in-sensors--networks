/**
 *  \file   tworayground.c
 *  \brief  Two-Ray ground propagation model
 *  \author Guillaume Chelius and Elyes Ben Hamida
 *  \date   2007
 **/
#include <include/modelutils.h>



/**
 *  TODO : include ht & hr (heights of transmit and receive antennas) in the antenna module
 *         
 **/

/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Two-Ray ground propagation model",
    "Elyes Ben Hamida",
    "0.1",
    MODELTYPE_PROPAGATION, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct entitydata {
	double factor;
    double Pr0;
};


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;
    double frequency = 868;

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "frequency_MHz")) {
            if (get_param_double(param->value, &(frequency))) {
                goto error;
            }
        }
    }

    /* update factor */
    entitydata->factor = 300 / (4 * M_PI * frequency);
    entitydata->Pr0 = mW2dBm(entitydata->factor * entitydata->factor);

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
    double h_src = get_node_position(src)->z, h_dst = get_node_position(dst)->z;
    double crossover = h_src * h_dst / entitydata->factor;

    /*
     * Cross over distance:
     *
     *        4 * pi * ht * hr
     *  dc = ------------------
     *             lambda
     *                               Pt * Gt * Gr * (ht * hr)^2
     *  Case 1: d >= dc  => Pr(d) = -----------------------------
     *                                        d^4 * L
     *
     *                               Pt * Gt * Gr * lambda^2
     *  Case 2: d < dc   => Pr(d) = -------------------------
     *                                 (4 * pi * d)^2 * L
     *
     *  Note: rxmW = Pt * Gt * Gr, and L = 1
     *  
     *  cf p89 ref "Wireless Communications: Principles and Practice", Theodore Rappaport, 1996.
     *
     */

    if ((crossover == 0) || (dist <= crossover)) {
        if (dist <= 1) {
            return  entitydata->Pr0 + rxdBm;
        } else {
            return entitydata->Pr0 + rxdBm - 10.0 * 2 * log10(dist);
        }    
    } else {
        return rxdBm + mW2dBm(h_src * h_dst * h_src * h_dst / pow(dist, 4.0));
    }
}


/* ************************************************** */
/* ************************************************** */
propagation_methods_t methods = {propagation};
