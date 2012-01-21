/**
 *  \file   rayleigh_fading.c
 *  \brief  Rayleigh and Pathloss propagation model
 *  \author Guillaume Chelius and Elyes Ben Hamida and Ruifeng Zhang
 *  \date   2007
 **/
#include <include/modelutils.h>

#define VARIANCE 0.6366197723676

#define FREESPACE     1
#define TWORAYGROUND  2
#define LOGDISTANCE   3
#define LOGNORMAL     4

/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Rayleigh Fading and Pathloss propagation model",
    "Elyes Ben Hamida",
    "0.1",
    MODELTYPE_PROPAGATION, 
    {NULL, 0}
};

/* ************************************************** */
/* ************************************************** */
struct entitydata {
    int    propagation; /* propagation type */
    double pathloss;    /* pathloss exponent */
    double deviation;   /* Shadowing deviation (dB) */
    double dist0;       /* reference distance (m) */
    double ht;
    double hr;
    /* optimize computation */
    double factor;
    double crossover_distance;
    double last_rxdBm;
    double Pr0;
};


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;
    double frequency = 868;

    /* default values */
    entitydata->ht          = 1;
    entitydata->hr          = 1;
    entitydata->pathloss    = 2.0;
    entitydata->deviation   = 4.0;
    entitydata->dist0       = 1.0;
    entitydata->last_rxdBm  = 9999;
    entitydata->Pr0         = 0.0;
    entitydata->propagation = FREESPACE;

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "propagation")) {
	  if (!strcmp(param->value, "freespace")) {
	      entitydata->propagation = FREESPACE;
	  }
	  else if (!strcmp(param->value, "tworayground")) {
	      entitydata->propagation = TWORAYGROUND;
	  }
	  else if (!strcmp(param->value, "logdistance")) {
	      entitydata->propagation = LOGDISTANCE;
	  }
	  else if (!strcmp(param->value, "shadowing")) {
	      entitydata->propagation = LOGNORMAL;
	  }
	  else {
	      fprintf(stderr,"\n[rayleigh_fading] Unknown propagation type (%s) !\n", param->value);
	      fprintf(stderr,"[rayleigh_fading] Possible propagation models are: freespace, tworayground, logdistance and shadowing\n\n");
	      goto error;
	  }
        }
        if (!strcmp(param->key, "pathloss")) {
            if (get_param_distance(param->value, &(entitydata->pathloss))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "frequency_MHz")) {
            if (get_param_double(param->value, &(frequency))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "deviation")) {
            if (get_param_double(param->value, &(entitydata->deviation))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "dist0")) {
            if (get_param_double(param->value, &(entitydata->dist0))) {
                goto error;
            }
        }
    }
    
    /* update factor */
    entitydata->factor = 300 / (4 * M_PI * frequency);
    entitydata->crossover_distance = entitydata->ht * entitydata->hr / entitydata->factor;

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
double normal (double avg, double deviation) {
    return (avg + deviation * cos(2*M_PI*get_random_double()) * sqrt(-2.0 * log(get_random_double())));
}


/* ************************************************** */
/* ************************************************** */
double freespace(struct entitydata *entitydata, double dist, double rxmW) {
    /*
     *           Pt * Gt * Gr * lambda^2
     *  Pr(d) = -------------------------
     *             (4 * pi * d)^2 * L
     *
     *  Note: rxmW = Pt * Gt * Gr, and L = 1
     *
     *  cf p71 ref "Wireless Communications: Principles and Practice", Theodore Rappaport, 1996.
     *  
     */
    return (rxmW * entitydata->factor*entitydata->factor / pow(dist, 2));
}

/* ************************************************** */
/* ************************************************** */
double compute_freespace(struct entitydata *entitydata, nodeid_t src, nodeid_t dst, double rxdBm) {
    /*
     *           Pt * Gt * Gr * lambda^2
     *  Pr(d) = -------------------------
     *             (4 * pi * d)^2 * L
     *
     *  Note: rxdBm = [Pt + Gt + Gr]_dBm, and L = 1
     *
     *  cf p71 ref "Wireless Communications: Principles and Practice", Theodore Rappaport, 1996.
     *
     */
    double dist = distance(get_node_position(src), get_node_position(dst));

    if (dist == 0) {
      return (rxdBm);
    } else {
      return (rxdBm + 2 * mW2dBm(entitydata->factor / dist));
    }
}

/* ************************************************** */
/* ************************************************** */
double compute_logdistance_pathloss(struct entitydata *entitydata, nodeid_t src, nodeid_t dst, double rxdBm) {
    /*
     *  Pr_dBm(d) = Pr_dBm(d0) - 10 * beta * log10(d/d0)
     *
     *  Note: rxdBm = [Pt + Gt + Gr]_dBm, and L = 1
     *
     *  cf p102-104 ref "Wireless Communications: Principles and Practice", Theodore Rappaport, 1996.
     *
     */
    double dist;

    if (rxdBm != entitydata->last_rxdBm) {
        entitydata->Pr0  = freespace(entitydata, entitydata->dist0, dBm2mW(rxdBm));
        entitydata->last_rxdBm = rxdBm;
    }

    dist = distance(get_node_position(src), get_node_position(dst));

    return mW2dBm(entitydata->Pr0) - 10.0 * entitydata->pathloss * log10(dist/entitydata->dist0);
}

/* ************************************************** */
/* ************************************************** */
double compute_tworayground(struct entitydata *entitydata, nodeid_t src, nodeid_t dst, double rxdBm) {
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
    double dist;

    dist = distance(get_node_position(src), get_node_position(dst));

    if (dist == 0) {
      return rxdBm;
    } 
    else if (dist < entitydata->crossover_distance) {
        /* uses friis formula for distance < crossover distance or if (ht == hr == 0) */
        return (rxdBm + 2 * mW2dBm(entitydata->factor / dist));
    } 
    else {
        /* else two-ray ground model is more accurate for distance > crossover distance */
        return (rxdBm + mW2dBm(entitydata->ht * entitydata->ht * 
                               entitydata->hr * entitydata->hr / pow(dist, 4.0)));
    }
}

/* ************************************************** */
/* ************************************************** */
double compute_lognormal_shadowing(struct entitydata *entitydata, nodeid_t src, nodeid_t dst, double rxdBm) {
    /*
     *  Pr_dBm(d) = Pr_dBm(d0) - 10 * beta * log10(d/d0) + X
     *
     *  Note: rxdBm = [Pt + Gt + Gr]_dBm, L = 1, and X a normal distributed RV (in dBm)
     *
     *  cf p104-105 ref "Wireless Communications: Principles and Practice", Theodore Rappaport, 1996.
     *
     */
    double dist, powerloss_dbm;

    if (rxdBm != entitydata->last_rxdBm) {
        entitydata->Pr0  = freespace(entitydata, entitydata->dist0, dBm2mW(rxdBm));
        entitydata->last_rxdBm = rxdBm;
    }

    dist = distance(get_node_position(src), get_node_position(dst));

    powerloss_dbm = -10.0 * entitydata->pathloss * log10(dist/entitydata->dist0) + normal(0.0, entitydata->deviation);
    
    return mW2dBm(entitydata->Pr0) + powerloss_dbm;
}


/* ************************************************** */
/* ************************************************** */
double compute_fading() {
  //    return 5.0 * log10(-2.0 * VARIANCE * log(get_random_double()));
  return 10*log10(-1.55 * VARIANCE * log(get_random_double()));
}

  
/* ************************************************** */
/* ************************************************** */
double propagation(call_t *c, packet_t *packet, nodeid_t src, nodeid_t dst, double rxdBm) {
    struct entitydata *entitydata = get_entity_private_data(c);
    double rx_dBm = rxdBm;
    
    switch(entitydata->propagation){
      case FREESPACE    : rx_dBm = compute_freespace(entitydata, src, dst, rxdBm); break;
      case TWORAYGROUND : rx_dBm = compute_tworayground(entitydata, src, dst, rxdBm); break;
      case LOGDISTANCE  : rx_dBm = compute_logdistance_pathloss(entitydata, src, dst, rxdBm); break;
      case LOGNORMAL    : rx_dBm = compute_lognormal_shadowing(entitydata, src, dst, rxdBm); break;
      default : rx_dBm = rxdBm; /* should not happen */
    }

    return rx_dBm + compute_fading();
}


/* ************************************************** */
/* ************************************************** */
propagation_methods_t methods = {propagation};
