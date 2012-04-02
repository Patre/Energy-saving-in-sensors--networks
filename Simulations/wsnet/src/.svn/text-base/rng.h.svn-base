/**
 *  \file   rng.h
 *  \brief  RNG based on GSL 
 *  \author Quentin Lampin
 *  \date   2009
 **/
#ifndef __rng__
#define __rng__

#include <include/modelutils.h>
#include <gsl/gsl_rng.h>

/* RNG structure and type */
typedef struct rng_s{
    gsl_rng *r;
    unsigned long int seed;
} rng_t;

/**
 * variables to be set in main
 * used to instanciate the default RNG
 **/
extern int default_rng_type;
extern unsigned long int default_rng_seed;

/**
 * when the output of the generator has to be constrained, eg. when getting a 
 * random x position, specifies how many attempts are made before exiting with
 * an error. If the value is -1 (default), then the number of attempts is infinite.
 */
extern int rng_retry_attempts;

/* set the type of the default RNG */
void default_rng_type_select(char *rng);

/**
 * create a new RNG with specified type and seed
 * return the ID of the RNG
 */
void *create_rng(int rng_type, unsigned long int seed);

/* init the default RNG */
int rng_init(void);

/* not used yet */
int rng_bootstrap(void);

/* clean all rngs, delete the hadas structure */
void rng_clean(void);


#endif
