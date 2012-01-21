/**
 *  \file   wsens_srv.h
 *  \brief  Worldsens server declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __wsens_srv__
#define __wsens_srv__

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <sched.h>
#include <sys/signal.h>
#include <time.h>

#include <netinet/in.h>

#include <include/worldsens_pkt.h>


/* ************************************************** */
/* ************************************************** */
void wsens_srv_init(void);
int wsens_srv_start(void);
void wsens_srv_finalize(void);
int wsens_srv_send(struct sockaddr_in *addr, char *msg, int len);
int wsens_srv_msend(char *msg, int len);
int wsens_srv_rcv(char *msg, int *len, struct sockaddr_in *addr);


#endif //__wsens_srv__
