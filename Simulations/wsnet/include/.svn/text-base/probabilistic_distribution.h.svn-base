/**
 *  \file   probabilistic_distribution.h
 *  \brief  Probabilistic distributions declarations
 *  \author Quentin Lampin
 *  \date   2009
 **/

#ifndef _PROBABILISTIC_DISTRIBUTION_PUBLIC_H
#define	_PROBABILISTIC_DISTRIBUTION_PUBLIC_H

/* distribution types */
#define GAUSSIAN 0
#define GAUSSIAN_TAIL 1
#define EXPONENTIAL 2
#define LAPLACE 3
#define EXPONENTIAL_POWER 4
#define CAUCHY 5
#define RAYLEIGH 6
#define RAYLEIGH_TAIL 7
#define LANDAU 8
#define LEVY_ALPHA_STABLE 9
#define GAMMA 10
#define UNIFORM 11
#define LOG_NORMAL 12
#define CHI_SQUARED 13
#define BETA 14
#define LOGISTIC 15
#define PARETO 16
#define WEIBULL 17
#define GUMBELL_1 18
#define GUMBELL_2 19
#define POISSON 20
#define BERNOULLI 21
#define BINOMIAL 22
#define GEOMETRIC 23
#define HYPERGEOMETRIC 24
#define LOGARITHMIC 25

typedef struct gaussian_args_s{
    double sigma;
}gaussian_args_t;

typedef struct gaussian_tail_args_s{
    double a;
    double sigma;
} gaussian_tail_args_t;

typedef struct bivariate_gaussian_args_s{
    double sigma_x;
    double sigma_y;
    double rho;
    double *return_x;
    double *return_y;
} bivariate_gaussian_args_t;

typedef struct exponential_args_s{
    double mu;
} exponential_args_t;

typedef struct laplace_args_s{
    double a;
} laplace_args_t;

typedef struct exppow_args_s{
    double a;
    double b;
} exppow_args_t;

typedef struct cauchy_args_s{
    double a;
} cauchy_args_t;

typedef struct rayleigh_s{
    double sigma;
} rayleigh_args_t;

typedef struct rayleigh_tail_s{
    double a;
    double sigma;
} rayleigh_tail_args_t;

typedef struct levy_alpha_stable_s{
    double c;
    double alpha;
} levy_alpha_stable_args_t;

typedef struct gamma_args_s{
    double a;
    double b;
} gamma_args_t;

typedef struct uniform_args_s{
    double a;
    double b;
} uniform_args_t;

typedef struct lognormal_args_s{
    double zeta;
    double sigma;
} lognormal_args_t;

typedef struct chisq_args_s{
    double nu;
} chisq_args_t;

typedef struct beta_args_s{
    double a;
    double b;
} beta_args_t;

typedef struct logistic_args_s{
    double a;
} logistic_args_t;

typedef struct pareto_args_s{
    double a;
    double b;
} pareto_args_t;

typedef struct spherical_vector_2d_args_s{
    double *return_x;
    double *return_y;
} spherical_vector_2d_args_t;

typedef struct spherical_vector_3d_args_s{
    double *return_x;
    double *return_y;
    double *return_z;
}spherical_vector_3d_args_t;

typedef struct weibull_args_s{
    double a;
    double b;
} weibull_args_t;

typedef struct gumbel_t1_args_s{
    double a;
    double b;
} gumbel_t1_args_t;

typedef struct gumbel_t2_args_s{
    double a;
    double b;
} gumbel_t2_args_t;

typedef struct poisson_args_s{
    double mu;
} poisson_args_t;

typedef struct bernoulli_args_s{
    double p;
} bernoulli_args_t;

typedef struct binomial_args_s{
    double p;
    unsigned int n;
} binomial_args_t;

typedef struct geometric_args_s{
    double p;
} geometric_args_t;

typedef struct hyper_geometric_args_s{
    unsigned int n1;
    unsigned int n2;
    unsigned int t;
} hyper_geometric_args_t;

typedef struct logarithmic_args_s{
    double p;
} logarithmic_args_t;

/* distribution function pointer type */
typedef double (* distribution_function_t) (void *rng_id, void *parameters);

/**
 * \brief Return a pointer to the distribution function according to type
 * \parameter type of the distribution
 * \return a function pointer
 **/
distribution_function_t get_distribution_function_by_type(int type);

#endif	/* _PROBABILISTIC_DISTRIBUTION_PUBLIC_H */

