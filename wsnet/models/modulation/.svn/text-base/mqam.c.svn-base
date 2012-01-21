/**
 *  \file   mqam.c
 *  \brief  modulation model
 *  \author ruifeng Zhang and Elyes Ben Hamida
 *  \date   2008
 **/
#include <math.h>

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "mqam modulation",
    "Ruifeng Zhang",
    "0.1",
    MODELTYPE_MODULATION, 
    {NULL, 0}
};

struct entitydata {
    int m_qam; /* order of modulation */
};

/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
//    return 0;
  struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;


    /* default values */
    entitydata->m_qam = 4;

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
	   if (!strcmp(param->key, "m_QAM")) {
		  if (!strcmp(param->value, "4")) {
		      entitydata->m_qam = 4;
		  }
		  else if (!strcmp(param->value, "8")) {
		      entitydata->m_qam = 8;
		  }
		  else if (!strcmp(param->value, "16")) {
		      entitydata->m_qam = 16;
		  }
		  else if (!strcmp(param->value, "32")) {
		      entitydata->m_qam = 32;
		  }
		  else if (!strcmp(param->value, "64")) {
		      entitydata->m_qam = 64;
		  }
		  else if (!strcmp(param->value, "128")) {
		      entitydata->m_qam = 128;
		  }
		  else if (!strcmp(param->value, "256")) {
		      entitydata->m_qam = 256;
		  }
		  else {
		      fprintf(stderr,"\n[MQAM_modulation] Unknown modulation type (%s) !\n", param->value);
		      fprintf(stderr,"[MQAM_modulation] Possible modulation types are: 4, 8, 16, 32, 64, 128 and 256\n\n");
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
    return 0;
}


/* ************************************************** */
/* ************************************************** */
double modulate(call_t *c, double snr) {
    struct entitydata *entitydata = get_entity_private_data(c);

    if (snr == MAX_SNR) {
      return 0;
    }
    else {
      return 2*(sqrt(entitydata->m_qam)-1)/sqrt(entitydata->m_qam)/(log(entitydata->m_qam)/log(2)) * erfc(sqrt(3/2*(log(entitydata->m_qam)/log(2))/(entitydata->m_qam-1)*snr));
    }
}

int bit_per_symbol(call_t *c){
    struct entitydata *entitydata = get_entity_private_data(c);
    return log10(entitydata->m_qam)/log10(2);
}

/* ************************************************** */
/* ************************************************** */
modulation_methods_t methods = {modulate, bit_per_symbol};



