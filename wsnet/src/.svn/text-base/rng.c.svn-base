/**
 *  \file   rng.c
 *  \brief  Random Number Generator
 *  \author Quentin Lampin and Elyes Ben Hamida
 *  \date   2009
 **/

#include <string.h>
#include <include/modelutils.h>

#include "internals.h"
#include "rng.h"
#include "probabilistic_distribution.h"

/**
 * RNGS global variables and functions
 **/
static void *hash_index = (void *)1;
static void *rngs = NULL;
static void *mem_rng = NULL;

unsigned long rng_hash(void *key) {
    return (unsigned long) key;
}

int rng_equal(void *key0, void *key1) {
    return (int) (key0 == key1);
}

/**
 *default RNG parameters, set in main
 **/
int default_rng_type = DEFAULT_RNG;
unsigned long int default_rng_seed = 0;

int rng_retry_attempts = -1;

/**
 * get RNG from ID
 **/
gsl_rng *get_rng_by_id(void *rng_id){
    rng_t *rng = (rng_t *)hadas_get(rngs, rng_id);
    gsl_rng *r = (gsl_rng *) rng->r;
    return r;
}

/* retrieve RNG type from string name */
int rng_type(char *rng) {
    /**
     * extract from GSL documentation:
     * The MT19937 generator of Makoto Matsumoto
     * and Takuji Nishimura is a variant of the twisted
     * generalized feedback shift-register algorithm,
     * and is known as the “Mersenne Twister” generator.
     * It has a Mersenne prime period of 2^19937 - 1 (about 10^6000)
     * and is equi-distributed in 623 dimensions. It has passed the diehard
     * statistical tests. It uses 624 words of state per generator and is
     * comparable in speed to the other generators. For more information see,
     * Makoto Matsumoto and Takuji Nishimura,
     * “Mersenne Twister: A 623-dimensionally equidistributed
     * uniform pseudorandom number generator”.
     * ACM Transactions on Modeling and Computer Simulation,
     * Vol. 8, No. 1 (Jan. 1998), Pages 3–30
     **/
    if (strcmp(rng, "mt19937") == 0) {
        /**
         * extract from GSL documentation:
         * The generator ranlxs0 is a second-generation version
         * of the ranlux algorithm of Lüscher, which produces
         * “luxury random numbers”. This generator provides
         * single precision output (24 bits) at three
         * luxury levels ranlxs0, ranlxs1 and ranlxs2,
         * in increasing order of strength. It uses
         * double-precision floating point arithmetic
         * internally and can be significantly faster
         * than the integer version of ranlux,
         * particularly on 64-bit architectures.
         * The period of the generator is about 10^171.
         * The algorithm has mathematically proven properties
         * and can provide truly decorrelated numbers at a
         * known level of randomness. The higher luxury levels
         * provide increased decorrelation between samples
         * as an additional safety margin.
         **/
        return MT19937;
    } else if (strcmp(rng, "ranlxs0")) {
        return RANLXS0;
    } else if (strcmp(rng, "ranlxs1")) {
        return RANLXS1;
    } else if (strcmp(rng, "ranlxs2")) {
        return RANLXS2;
        /**
         * extract from GSL documentation:
         * These generators produce double precision output (48 bits)
         * from the ranlxs generator. The library provides two luxury
         * levels ranlxd1 and ranlxd2, in increasing order of strength.
         **/
    } else if (strcmp(rng, "ranlxd1")) {
        return RANLXD1;
    } else if (strcmp(rng, "ranlxd2")) {
        return RANLXD2;
        /**
         * extract from GSL documentation:
         *The ranlux generator is an implementation of the original algorithm
         * developed by Lüscher. It uses a lagged-fibonacci-with-skipping algorithm
         * to produce “luxury random numbers”. It is a 24-bit generator, originally
         * designed for single-precision IEEE floating point numbers.
         * This implementation is based on integer arithmetic,
         * while the second-generation versions ranlxs and ranlxd described above
         * provide floating-point implementations which will be faster on many
         * platforms. The period of the generator is about 10^171.
         * The algorithm has mathematically proven properties and it
         * can provide truly decorrelated numbers at a known level of randomness.
         * The default level of decorrelation recommended by Lüscher is provided by
         * rng_ranlux, while rng_ranlux389 gives the highest level of randomness,
         * with all 24 bits decorrelated.
         * Both types of generator use 24 words of state per generator.
         **/
    } else if (strcmp(rng, "rng_ranlux")) {
        return RANLUX;
    } else if (strcmp(rng, "rng_ranlux389")) {
        return RANLUX389;
    } else {
        /**
         * algorithm not matched
         * default rng is used
         */
        return MT19937;
    }
}

void default_rng_type_select(char *rng) {
    default_rng_type = rng_type(rng);
}

int rng_init(void) {
    if ((mem_rng = mem_fs_slice_declare(sizeof(uniform_args_t))) == NULL) {
        return -1;
    }
    rngs = hadas_create(rng_hash, rng_equal);
    create_rng(default_rng_type, default_rng_seed);
    return 0;
}

int rng_bootstrap(void) {
    return 0;
}

void rng_clean(void) {
    void *i;
    for (i = (void *)1; i < hash_index; i++) {
      rng_t *rng = (rng_t *) hadas_get(rngs, i);
        gsl_rng_free(rng->r);
        free(rng);
    }
    hadas_destroy(rngs);
    return;
}

void *create_rng(int rng_type, unsigned long int seed) {
    gsl_rng *r = NULL;
    rng_t *rng = malloc(sizeof (rng_t));
    switch (rng_type) {
        case MT19937:
            r = gsl_rng_alloc(gsl_rng_mt19937);
            break;
        case RANLXS0:
            r = gsl_rng_alloc(gsl_rng_ranlxs0);
            break;
        case RANLXS1:
            r = gsl_rng_alloc(gsl_rng_ranlxs1);
            break;
        case RANLXS2:
            r = gsl_rng_alloc(gsl_rng_ranlxs2);
            break;
        case RANLXD1:
            r = gsl_rng_alloc(gsl_rng_ranlxd1);
            break;
        case RANLXD2:
            r = gsl_rng_alloc(gsl_rng_ranlxd2);
            break;
        case RANLUX:
            r = gsl_rng_alloc(gsl_rng_ranlux);
            break;
        case RANLUX389:
            r = gsl_rng_alloc(gsl_rng_ranlux389);
            break;
        default:
            /* should not happen */
            break;
    }
    rng->r = r;
    if (seed) {
        rng->seed = seed;
        gsl_rng_set(r, seed);
    } else {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        gsl_rng_set(r, tv.tv_usec + tv.tv_sec);
        rng->seed = tv.tv_usec + tv.tv_sec;
    }
    hadas_insert(rngs, hash_index, (void *) rng);
    hash_index++;
    return hash_index - 1;
}

void destroy_rng(void *rng_id) {
    rng_t *rng = (rng_t *) hadas_get(rngs, rng_id);
    gsl_rng_free(rng->r);
    hadas_delete(rngs, rng_id);
}
/**
 * get a random distance following the specified distribution and parameters
 **/
double get_random_distance_gsl(void *rng_id, int distribution_type, void *parameters) {
    int attempts = 0;
    distribution_function_t distribution = get_distribution_function_by_type(distribution_type);
    double result = distribution(rng_id, parameters);
    double max = sqrt(get_topology_area()->x * get_topology_area()->x +
                      get_topology_area()->y * get_topology_area()->y +
                      get_topology_area()->z * get_topology_area()->z);
    while(result > max && (attempts < rng_retry_attempts || rng_retry_attempts <= -1)){
        result = distribution(rng_id, parameters);
        if(rng_retry_attempts > -1){
            attempts++;
        }
    }
    if(attempts == rng_retry_attempts){
        printf("Error: Too many attempts to get a valid value from the rng %p."
	       "Check the distribution parameters. Exiting!\n", rng_id);
        exit(-1);
    }
    return result;
}

double get_random_x_position_gsl(void *rng_id, int distribution_type, void *parameters) {
    int attempts = 0;
    double max = get_topology_area()->x;
    distribution_function_t distribution = get_distribution_function_by_type(distribution_type);
    double result = distribution(rng_id, parameters);
    while(result > max && (attempts < rng_retry_attempts || rng_retry_attempts <= -1)){
        result = distribution(rng_id, parameters);
        if(rng_retry_attempts > -1){
            attempts++;
        }
    }
    if(attempts == rng_retry_attempts){
        printf("Error: Too many attempts to get a valid value from the rng %p."
	       "Check the distribution parameters. Exiting!\n", rng_id);
        exit(-1);
    }
    return result;
}

double get_random_y_position_gsl(void *rng_id, int distribution_type, void *parameters) {
    int attempts = 0;
    double max = get_topology_area()->y;
    distribution_function_t distribution = get_distribution_function_by_type(distribution_type);
    double result = distribution(rng_id, parameters);
    while(result > max && (attempts < rng_retry_attempts || rng_retry_attempts <= -1)){
        result = distribution(rng_id, parameters);
        if(rng_retry_attempts > -1){
            attempts++;
        }
    }
    if(attempts == rng_retry_attempts){
        printf("Error: Too many attempts to get a valid value from the rng %p."
	       "Check the distribution parameters. Exiting!\n", rng_id);
        exit(-1);
    }
    return result;
}

double get_random_z_position_gsl(void *rng_id, int distribution_type, void *parameters) {
    int attempts = 0;
    double max = get_topology_area()->z;
    distribution_function_t distribution = get_distribution_function_by_type(distribution_type);
    double result = distribution(rng_id, parameters);
    while(result > max && (attempts < rng_retry_attempts || rng_retry_attempts <= -1)){
        result = distribution(rng_id, parameters);
        if(rng_retry_attempts > -1){
            attempts++;
        }
    }
    if(attempts == rng_retry_attempts){
        printf("Error: Too many attempts to get a valid value from the rng %p."
	       "Check the distribution parameters. Exiting!\n", rng_id);
        exit(-1);
    }
    return result;
}

double get_random_double_gsl(void *rng_id, int distribution_type, void *parameters) {
    distribution_function_t distribution = get_distribution_function_by_type(distribution_type);
    double result = distribution(rng_id, parameters);
    return result;
}

double get_random_double_range_gsl(void *rng_id, int distribution_type, void *parameters, double min, double max) {
    int attempts = 0;
    distribution_function_t distribution = get_distribution_function_by_type(distribution_type);
    double result = distribution(rng_id, parameters);
    while((result < min || result > max) && (attempts < rng_retry_attempts || rng_retry_attempts <= -1)){
        result = distribution(rng_id, parameters);
        if(rng_retry_attempts > -1){
            attempts++;
        }
    }
    if(attempts == rng_retry_attempts){
        printf("Error: Too many attempts to get a valid value from the rng %p."
	       "Check the distribution parameters. Exiting!\n", rng_id);
        exit(-1);
    }
    return result;
}

int get_random_integer_gsl(void *rng_id, int distribution_type, void *parameters) {
    distribution_function_t distribution = get_distribution_function_by_type(distribution_type);
    double result = distribution(rng_id, parameters);
    return (int)result;
}

int get_random_integer_range_gsl(void *rng_id, int distribution_type, void *parameters, int min, int max) {
    int attempts = 0;
    distribution_function_t distribution = get_distribution_function_by_type(distribution_type);
    double result = distribution(rng_id, parameters);
    while((result < min || result > max) && (attempts < rng_retry_attempts || rng_retry_attempts <= -1)){
        result = distribution(rng_id, parameters);
        if(rng_retry_attempts > -1){
            attempts++;
        }
    }
    if(attempts == rng_retry_attempts){
        printf("Error: Too many attempts to get a valid value from the rng %p."
	       "Check the distribution parameters. Exiting!\n", rng_id);
        exit(-1);
    }
    return result;
}

uint64_t get_random_time_gsl(void *rng_id, int distribution_type, void *parameters) {
    int attempts = 0;
    distribution_function_t distribution = get_distribution_function_by_type(distribution_type);
    double result = distribution(rng_id, parameters);
    while(result < 0 && (attempts < rng_retry_attempts || rng_retry_attempts <= -1)){
        result = distribution(rng_id, parameters);
        if(rng_retry_attempts > -1){
            attempts++;
        }
    }
    if(attempts == rng_retry_attempts){
        printf("Error: Too many attempts to get a valid value from the rng %p."
	       "Check the distribution parameters. Exiting!\n", rng_id);
        exit(-1);
    }
    return (uint64_t)result;
}

uint64_t get_random_time_range_gsl(void *rng_id, int distribution_type, void *parameters, uint64_t min, uint64_t max) {
    int attempts = 0;
    distribution_function_t distribution = get_distribution_function_by_type(distribution_type);
    double result = distribution(rng_id, parameters);
    while(((uint64_t)result*1000000000 < min || (uint64_t)result*1000000000 > max)
            && (attempts < rng_retry_attempts || rng_retry_attempts <= -1)){
        result = distribution(rng_id, parameters);
        if(rng_retry_attempts > -1){
            attempts++;
        }
    }
    if(attempts == rng_retry_attempts){
        printf("Error: Too many attempts to get a valid value from the rng %p."
	       "Check the distribution parameters. Exiting!\n", rng_id);
        exit(-1);
    }
    return (uint64_t)result;
}

nodeid_t get_random_node_gsl(void *rng_id, int distribution_type, void *parameters, nodeid_t exclusion) {
    nodeid_t node = get_random_integer_range_gsl(rng_id, distribution_type,  parameters, 0, nodes.size);
    while (node == exclusion){
        node = get_random_integer_range_gsl(rng_id, distribution_type, parameters, 0, nodes.size);
    }
    return node;
}

/******************************************************************************/
/******************************************************************************/

uint64_t get_random_time(void) {
    uniform_args_t *args;
    if ((args = (uniform_args_t *) mem_fs_alloc(mem_rng)) == NULL) {
      printf("Error: Error while allocating memory for rng. Exiting!\n");
      exit(-1);
    }
    args->a = 0;
    /* GSL bounds are [min,max[, so we use cheduler_get_end()+1 
     * to provide an integer in the [min,max] range */
    args->b = scheduler_get_end() + 1;
    uint64_t result = get_random_time_gsl((void *)DEFAULT_RNG, UNIFORM, args);
    mem_fs_dealloc(mem_rng, args);
    return result;
}

double get_random_distance(void) {
    uniform_args_t *args;
    if ((args = (uniform_args_t *) mem_fs_alloc(mem_rng)) == NULL) {
      printf("Error: Error while allocating memory for rng. Exiting!\n");
      exit(-1);
    }
    args->a = 0;
    args->b = sqrt(get_topology_area()->x * get_topology_area()->x +
                   get_topology_area()->y * get_topology_area()->y +
                   get_topology_area()->z * get_topology_area()->z);
    double result = get_random_double_gsl((void *)DEFAULT_RNG, UNIFORM, args);
    mem_fs_dealloc(mem_rng, args);
    return result;
}

double get_random_x_position(void) {
    uniform_args_t *args;
    if ((args = (uniform_args_t *) mem_fs_alloc(mem_rng)) == NULL) {
      printf("Error: Error while allocating memory for rng. Exiting!\n");
      exit(-1);
    }
    args->a = 0;
    args->b = get_topology_area()->x;
    double result = get_random_double_gsl((void *)DEFAULT_RNG, UNIFORM, args);
    mem_fs_dealloc(mem_rng, args);
    return result;
}

double get_random_y_position(void) {
    uniform_args_t *args;
    if ((args = (uniform_args_t *) mem_fs_alloc(mem_rng)) == NULL) {
      printf("Error: Error while allocating memory for rng. Exiting!\n");
      exit(-1);
    }
    args->a = 0;
    args->b = get_topology_area()->y;
    double result = get_random_double_gsl((void *)DEFAULT_RNG, UNIFORM, args);
    mem_fs_dealloc(mem_rng, args);
    return result;
}

double get_random_z_position(void) {
    uniform_args_t *args;
    if ((args = (uniform_args_t *) mem_fs_alloc(mem_rng)) == NULL) {
      printf("Error: Error while allocating memory for rng. Exiting!\n");
      exit(-1);
    }
    args->a = 0;
    args->b = get_topology_area()->z;
    double result = get_random_double_gsl((void *)DEFAULT_RNG, UNIFORM, args);
    mem_fs_dealloc(mem_rng, args);
    return result;
}

double get_random_double(void) {
    uniform_args_t *args;
    if ((args = (uniform_args_t *) mem_fs_alloc(mem_rng)) == NULL) {
      printf("Error: Error while allocating memory for rng. Exiting!\n");
      exit(-1);
    }
    args->a = 0;
    args->b = 1;
    double result = get_random_double_gsl((void *)DEFAULT_RNG, UNIFORM, args);
    mem_fs_dealloc(mem_rng, args);
    return result;
}

double get_random_double_range(double min, double max) {
    uniform_args_t *args;
    if ((args = (uniform_args_t *) mem_fs_alloc(mem_rng)) == NULL) {
      printf("Error: Error while allocating memory for rng. Exiting!\n");
      exit(-1);
    }
    args->a = min;
    args->b = max;
    double result = get_random_double_gsl((void *)DEFAULT_RNG, UNIFORM, args);
    mem_fs_dealloc(mem_rng, args);
    return result;
}

uint64_t get_random_time_range(uint64_t min, uint64_t max){
    uniform_args_t *args;
    if ((args = (uniform_args_t *) mem_fs_alloc(mem_rng)) == NULL) {
      printf("Error: Error while allocating memory for rng. Exiting!\n");
      exit(-1);
    }
    args->a = min;
    /* GSL bounds are [min,max[, so we use max+1 to provide an 
     * integer in the [min,max] range */
    args->b = max + 1;
    uint64_t result = get_random_time_gsl((void *)DEFAULT_RNG, UNIFORM, args);
    mem_fs_dealloc(mem_rng, args);
    return result;

}

int get_random_integer(void) {
    uniform_args_t *args;
    if ((args = (uniform_args_t *) mem_fs_alloc(mem_rng)) == NULL) {
      printf("Error: Error while allocating memory for rng. Exiting!\n");
      exit(-1);
    }
    args->a = INT_MIN;
    args->b = INT_MAX;
    int result = get_random_integer_gsl((void *)DEFAULT_RNG, UNIFORM, args);
    mem_fs_dealloc(mem_rng, args);
    return result;
}

int get_random_integer_range(int min, int max) {
    uniform_args_t *args;
    if ((args = (uniform_args_t *) mem_fs_alloc(mem_rng)) == NULL) {
      printf("Error: Error while allocating memory for rng. Exiting!\n");
      exit(-1);
    }
    args->a = min;
    /* GSL bounds are [min,max[, so we use max+1 to provide an 
     * integer in the [min,max] range */
    args->b = max + 1;
    int result = get_random_integer_gsl((void *)DEFAULT_RNG, UNIFORM, args);
    mem_fs_dealloc(mem_rng, args);
    return result;
}

nodeid_t get_random_node(nodeid_t exclusion) {
    uniform_args_t *args;
    if ((args = (uniform_args_t *) mem_fs_alloc(mem_rng)) == NULL) {
      printf("Error: Error while allocating memory for rng. Exiting!\n");
      exit(-1);
    }
    args->a = 0;
    /* GSL bounds are [min,max[, so we use nodes.size instead of nodes.size-1 */
    args->b = nodes.size;
    int result = get_random_node_gsl((void *)DEFAULT_RNG, UNIFORM, args, exclusion);
    mem_fs_dealloc(mem_rng, args);
    return result;
}
