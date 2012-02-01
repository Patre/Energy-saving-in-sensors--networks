/**
 *  \file   probabilistic_distribution.c
 *  \brief  Random Number Generator
 *  \author Quentin Lampin
 *  \date   2009
 **/

#include <gsl/gsl_randist.h>
#include <include/modelutils.h>
#include "probabilistic_distribution.h"
#include "rng.h"

/**
 * \brief return a float that is given according to a gaussian distribution of parameter sigma
 * \parameter the RNG to use
 * \parameter sigma
 */
double gaussian(void *rng_id, void *parameters){
    gaussian_args_t *args = (gaussian_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_gaussian(r, args->sigma);
}

double gaussian_tail(void *rng_id, void *parameters){
    gaussian_tail_args_t *args = (gaussian_tail_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_gaussian_tail(r, args->a, args->sigma);
}

double laplace(void *rng_id, void *parameters){
    laplace_args_t *args = (laplace_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_laplace(r, args->a);
}

double exponential(void *rng_id, void *parameters){
    exponential_args_t *args = (exponential_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_exponential(r, args->mu);
}

double exponential_power(void *rng_id, void *parameters){
    exppow_args_t *args = (exppow_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_exppow(r, args->a, args->b);
}

double cauchy(void *rng_id, void *parameters){
    cauchy_args_t *args = (cauchy_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_cauchy(r, args->a);
}

double rayleigh(void *rng_id, void *parameters){
    rayleigh_args_t *args = (rayleigh_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_rayleigh(r, args->sigma);
}

double rayleigh_tail(void *rng_id, void *parameters){
    rayleigh_tail_args_t *args = (rayleigh_tail_args_t *)parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_rayleigh_tail(r, args->a, args->sigma);
}

double landau(void *rng_id, void *parameters){
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_landau(r);
}

double levy_alpha_stable(void *rng_id, void *parameters){
    levy_alpha_stable_args_t *args =(levy_alpha_stable_args_t *)parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_levy(r, args->c, args->alpha);
}

double gamma_d(void *rng_id, void *parameters){
    gamma_args_t *args = (gamma_args_t *)parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_gamma(r, args->a, args->b);
}

double log_normal(void *rng_id, void *parameters){
    lognormal_args_t *args = (lognormal_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_lognormal(r,args->zeta, args->sigma);
}

double uniform(void *rng_id, void *parameters){
    uniform_args_t *args = (uniform_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_flat(r, args->a, args->b);
}

double chi_squared(void *rng_id, void *parameters){
    chisq_args_t *args = (chisq_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_chisq(r, args->nu);
}

double beta(void *rng_id, void *parameters){
    beta_args_t *args = (beta_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_beta(r, args->a, args->b);
}

double logistic(void *rng_id, void *parameters){
    logistic_args_t *args = (logistic_args_t *)parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_logistic(r, args->a);
}

double pareto(void *rng_id, void *parameters){
    pareto_args_t *args = (pareto_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_pareto(r, args->a, args->b);
}

double weibull(void *rng_id, void *parameters){
    weibull_args_t *args = (weibull_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_weibull(r, args->a, args->b);
}

double gumbel1(void *rng_id, void *parameters){
    gumbel_t1_args_t *args = (gumbel_t1_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_gumbel1(r, args->a, args->b);
}

double gumbel2(void *rng_id, void *parameters){
    gumbel_t2_args_t *args = (gumbel_t2_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return gsl_ran_gumbel2(r, args->a, args->b);
}

double poisson(void *rng_id, void *parameters){
    poisson_args_t *args = (poisson_args_t *)parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return (double)gsl_ran_poisson(r, args->mu);
}

double bernoulli(void *rng_id, void *parameters){
    bernoulli_args_t *args = (bernoulli_args_t *)parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return (double)gsl_ran_bernoulli(r, args->p);
}

double binomial(void *rng_id, void *parameters){
    binomial_args_t *args =(binomial_args_t *)parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return (double)gsl_ran_binomial(r, args->p, args->n);
}

double geometric(void *rng_id, void *parameters){
    geometric_args_t *args = (geometric_args_t *)parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return (double)gsl_ran_geometric(r, args->p);
}

double hypergeometric(void *rng_id, void *parameters){
    hyper_geometric_args_t *args = (hyper_geometric_args_t *)parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return (double)gsl_ran_hypergeometric(r, args->n1, args->n2, args->t);
}


double logarithmic(void *rng_id, void *parameters){
    logarithmic_args_t *args = (logarithmic_args_t *) parameters;
    gsl_rng *r = get_rng_by_id(rng_id);
    return (double)gsl_ran_logarithmic(r, args->p);
}

distribution_function_t get_distribution_function_by_type(int type) {
    switch (type) {
        case GAUSSIAN:
            return gaussian;
            break;
        case GAUSSIAN_TAIL:
            return gaussian_tail;
            break;
        case EXPONENTIAL:
            return exponential;
            break;
        case LAPLACE:
            return laplace;
            break;
        case EXPONENTIAL_POWER:
            return exponential_power;
            break;
        case CAUCHY:
            return cauchy;
            break;
        case RAYLEIGH:
            return rayleigh;
            break;
        case RAYLEIGH_TAIL:
            return rayleigh_tail;
            break;
        case LANDAU:
            return landau;
            break;
        case LEVY_ALPHA_STABLE:
            return levy_alpha_stable;
            break;
        case GAMMA:
            return gamma_d;
            break;
        case UNIFORM:
            return uniform;
            break;
        case LOG_NORMAL:
            return log_normal;
            break;
        case CHI_SQUARED:
            return chi_squared;
            break;
        case BETA:
            return beta;
            break;
        case LOGISTIC:
            return logistic;
            break;
        case PARETO:
            return pareto;
            break;
        case WEIBULL:
            return weibull;
            break;
        case GUMBELL_1:
            return gumbel1;
            break;
        case GUMBELL_2:
            return gumbel2;
            break;
        case POISSON:
            return poisson;
            break;
        case BERNOULLI:
            return bernoulli;
            break;
        case BINOMIAL:
            return binomial;
            break;
        case GEOMETRIC:
            return geometric;
            break;
        case HYPERGEOMETRIC:
            return hypergeometric;
            break;
        case LOGARITHMIC:
            return logarithmic;
            break;
        default:
            /*if type is not supported yet, use the uniform distribution*/
            return uniform;
            break;
    }
    /* should not happen */
    return NULL;
}





