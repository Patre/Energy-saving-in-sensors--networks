/**
 *  \file   rng.h
 *  \brief  Random number generator declarations
 *  \author Quentin Lampin 
 *  \date   2009
 **/
#ifndef __rng_public__
#define __rng_public__

#include <include/types.h>
#include <gsl/gsl_rng.h>


/* ID of the default RNG */
#define DEFAULT_RNG 1

/* RNGs types */
#define MT19937   1
#define RANLXS0   2
#define RANLXS1   3
#define RANLXS2   4
#define RANLXD1   5
#define RANLXD2   6
#define RANLUX    7
#define RANLUX389 8

/* Default number of attempts to get a constrained value from an RNG */
#define RNG_DEFAULT_RETRY_ATTEMPTS -1

/**
 * \brief retrieve RNG by ID.
 * \param rng_id ID of the rng to retrieve.
 * \return pointer to the RNG.
 **/
gsl_rng *get_rng_by_id(void *rng_id);

/**
 * \brief create RNG
 * \param rng_type the type of the RNG to instanciate
 * \param seed the seed of the RNG
 * \return id of the newly created RNG
 **/
void *create_rng(int rng_type, unsigned long int seed);
/* destroy */

/**
 * \brief Return a random distance.
 * \param rng_id ID of the RNG to use.
 * \param distribution_type type of the probability distribution
 * \param parameters the probability distribution parameters 
 * \return A random distance.
 **/
double get_random_distance_gsl(void *rng_id, int distribution_type, void *parameters);

/**
 * \brief Return a random position on the x dimension.
 * \param rng_id ID of the RNG to use.
 * \param distribution_type type of the probability distribution
 * \param parameters the probability distribution parameters 
 * \return A random position on the x dimension.
 **/
double get_random_x_position_gsl(void *rng_id, int distribution_type, void *parameters);

/**
 * \brief Return a random position on the y dimension.
 * \param rng_id ID of the RNG to use.
 * \param distribution_type type of the probability distribution
 * \param parameters the probability distribution parameters 
 * \return A random position on the y dimension.
 **/
double get_random_y_position_gsl(void *rng_id, int distribution_type, void *parameters);

/**
 * \brief Return a random position on the z dimension.
 * \param rng_id ID of the RNG to use.
 * \param distribution_type type of the probability distribution
 * \param parameters the probability distribution parameters 
 * \return A random position on the z dimension.
 **/
double get_random_z_position_gsl(void *rng_id, int distribution_type, void *parameters);

/**
 * \brief Return a random double value in [0,1[.
 * \param rng_id ID of the RNG to use.
 * \param distribution_type type of the probability distribution
 * \param parameters the probability distribution parameters 
 * \return A random double value in [0,1[.
 **/
double get_random_double_gsl(void *rng_id, int distribution_type, void *parameters);

/**
 * \brief Return a random double value in [min,max[.
 * \param rng_id ID of the RNG to use.
 * \param distribution_type type of the probability distribution
 * \param parameters the probability distribution parameters 
 * \param min the min value that can be drawn.
 * \param max the max value that can be drawn.
 * \return A random double value in [min,max[.
 **/
double get_random_double_range_gsl(void *rng_id, int distribution_type, void *parameters, double min, double max);


/**
 * \brief Return a random integer value.
 * \param rng_id ID of the RNG to use.
 * \param distribution_type type of the probability distribution
 * \param parameters the probability distribution parameters 
 * \return A random integer value.
 **/
int get_random_integer_gsl(void *rng_id, int distribution_type, void *parameters);


/**
 * \brief Return a random integer value in [min,max].
 * \param rng_id ID of the RNG to use.
 * \param distribution_type type of the probability distribution
 * \param parameters the probability distribution parameters 
 * \param min the min value that can be drawn.
 * \param max the max value that can be drawn.
 * \return A random integer value.
 **/
int get_random_integer_range_gsl(void *rng_id, int distribution_type, void *parameters, int min, int max);


/**
 * \brief Return a random time.
 * \param rng_id ID of the RNG to use.
 * \param distribution_type type of the probability distribution
 * \param parameters the probability distribution parameters 
 * \return A random time in ns.
 **/
uint64_t get_random_time_gsl(void *rng_id, int distribution_type, void *parameters);


/**
 * \brief Return a random time in [min,max].
 * \param rng_id ID of the RNG to use.
 * \param distribution_type type of the probability distribution
 * \param parameters the probability distribution parameters 
 * \param min the min value that can be drawn.
 * \param max the max value that can be drawn.
 * \return A random time in [min,max].
 **/
uint64_t get_random_time_range_gsl(void *rng_id, int distribution_type, void *parameters, uint64_t min, uint64_t max);


/**
 * \brief Return a random node id different from a specified one.
 * \param rng_id ID of the RNG to use.
 * \param distribution_type type of the probability distribution
 * \param parameters the probability distribution parameters 
 * \param exclusion the node identifier we do not want to be returned.
 * \return A random node identifier.
 **/
nodeid_t get_random_node_gsl(void *rng_id, int distribution_type, void *parameters, nodeid_t exclusion);


/**
 * ALL THE FOLLOWING FUNCTION ARE USED BY THE WSNET KERNEL WITH THE DEFAULT RNG. 
 **/


/** 
 * \brief Return a random distance.
 * \return A random distance.
 **/
double get_random_distance(void);


/** 
 * \brief Return a random position on the x dimension.
 * \return A random position on the x dimension.
 **/
double get_random_x_position(void);


/** 
 * \brief Return a random position on the y dimension.
 * \return A random position on the y dimension.
 **/
double get_random_y_position(void);


/** 
 * \brief Return a random position on the z dimension.
 * \return A random position on the z dimension.
 **/
double get_random_z_position(void);


/** 
 * \brief Return a random double value in [0,1[.
 * \return A random double value in [0,1[.
 **/
double get_random_double(void);


/** 
 * \brief Return a random double value in [min,max[.
 * \param min the min value that can be drawn.
 * \param max the max value that can be drawn.
 * \return A random double value in [min,max[.
 **/
double get_random_double_range(double min, double max);


/** 
 * \brief Return a random integer value.
 * \return A random integer value.
 **/
int get_random_integer(void);


/** 
 * \brief Return a random integer value in [min,max].
 * \param min the min value that can be drawn.
 * \param max the max value that can be drawn.
 * \return A random integer value.
 **/
int get_random_integer_range(int min, int max);


/** 
 * \brief Return a random time in [0,simulation_end].
 * \return A random time [0,simulation_end].
 **/
uint64_t get_random_time(void);


/** 
 * \brief Return a random time in [min,max].
 * \param min the min value that can be drawn.
 * \param max the max value that can be drawn.
 * \return A random time in [min,max].
 **/
uint64_t get_random_time_range(uint64_t min, uint64_t max);


/** 
 * \brief Return a random node id different from a specified one.
 * \param exclusion the node identifier we do not want to be returned.
 * \return A random node identifier.
 **/
nodeid_t get_random_node(nodeid_t exclusion);


#endif //__rng_public__
