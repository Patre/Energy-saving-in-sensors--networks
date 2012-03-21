#include "structure/time_wsnet.h"
/***********************************************************************************************/
/***********************************************************************************************/
//recuperer le temps en nano second
uint64_t get_time_now()
{
        return get_time();
}



/***********************************************************************************************/
/***********************************************************************************************/
//convetir le temps en second
double time_nanos_to_seconds(uint64_t x)
{
    return x * 0.000000001;
}

/***********************************************************************************************/
/***********************************************************************************************/
//convetir le temps en nano seconde
uint64_t time_seconds_to_nanos(double x)
{
    double inter=x * 1000000000;
    return (uint64_t) inter;
}

/***********************************************************************************************/
/***********************************************************************************************/
//recuperer le temps en seconde (Generalement pour l'affichage)
double get_time_now_second()
{
        return time_nanos_to_seconds(get_time());
}


/***********************************************************************************************/
/***********************************************************************************************/
//recuperer le temps de prochiane evenement

uint64_t get_time_next(uint64_t debuut,uint64_t period,uint64_t timeNow)
{
    double peri=time_nanos_to_seconds(period);
    double deb=time_nanos_to_seconds(debuut);
    double ti=time_nanos_to_seconds(timeNow);


    double time_app=ti-deb;
    int nbr=time_app/peri;
    nbr++;

    double debut=peri * nbr + deb;
    uint64_t at=time_seconds_to_nanos(debut) + get_random_time_range(0,period);//*/

    DEBUG;
        //    printf("TA: %lf nbr: %d (+ %lf =)DB: %lf\n",time_app,nbr,time_nanos_to_seconds(entitydata->debut),debut);//*/

    return at;
}

