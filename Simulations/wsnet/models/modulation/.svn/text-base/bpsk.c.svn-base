/**
 *  \file   bpsk.c
 *  \brief  BPSK
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include <math.h>

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "bpsk",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_MODULATION, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    return 0;
}

int destroy(call_t *c) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
double modulate(call_t *c, double snr) {
    if (snr == MAX_SNR) {
      return 0;
    } else {
      return 0.5 * erfc(sqrt(snr));
    }
}

int bit_per_symbol(call_t *c){
    return 1;
}

/* ************************************************** */
/* ************************************************** */
modulation_methods_t methods = {modulate, bit_per_symbol};
