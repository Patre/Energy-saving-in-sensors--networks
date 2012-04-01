#ifndef _TIME_RBH
#define _TIME_RBH

#include <include/modelutils.h>

uint64_t get_time_now();
double time_nanos_to_seconds(uint64_t time_nanos);
uint64_t time_seconds_to_nanos(double time_seconds);
double get_time_now_second();
uint64_t get_time_next(uint64_t debut,uint64_t period,uint64_t timeNow);

#endif //_TIME_RBH
