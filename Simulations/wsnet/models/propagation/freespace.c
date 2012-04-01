/**
 *  \file   logdistance_pathloss.c
 *  \brief  Log-Distance Pathloss propagation model
 *  \author Guillaume Chelius and Elyes Ben Hamida
 *  \date   2007
 **/
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Log-Distance Pathloss propagation model",
    "Elyes Ben Hamida",
    "0.1",
    MODELTYPE_PROPAGATION, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct entitydata {
    double pathloss;
    double Pr0;
};


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;
    double frequency = 868;

    /* default values */
    entitydata->pathloss   = 2.0;

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "frequency_MHz")) {
            if (get_param_double(param->value, &(frequency))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "pathloss")) {
            if (get_param_double(param->value, &(entitydata->pathloss))) {
                goto error;
            }
        }
    }
    
    /*
     * Compute attenuation at reference distance d=1 with reference power Pt=1 :
     *
     *           Pt * Gt * Gr * lambda^2
     *  Pr(d) = -------------------------
     *             (4 * pi * d)^2 * L
     *
     *  Note: rxmW = Pt * Gt * Gr, and L = 1
     *
     *  cf p71 ref "Wireless Communications: Principles and Practice", Theodore Rappaport, 1996.
     *
     */
    entitydata->Pr0 = mW2dBm((300 / (4 * M_PI * frequency)) * (300 / (4 * M_PI * frequency)));
    
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

    /*
     *  Pr_dBm(d) = Pr_dBm(d0) - 10 * beta * log10(d/d0)
     *
     *  Note: rxdBm = [Pt + Gt + Gr]_dBm, and L = 1
     *
     *  cf p102-104 ref "Wireless Communications: Principles and Practice", Theodore Rappaport, 1996.
     *
     */
    if (dist <= 1) {
        return entitydata->Pr0 + rxdBm;
    } else {
        return entitydata->Pr0 + rxdBm - 10.0 * entitydata->pathloss * log10(dist);
    }
}


/* ************************************************** */
/* ************************************************** */
propagation_methods_t methods = {propagation};
