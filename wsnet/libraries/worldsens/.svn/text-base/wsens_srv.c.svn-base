/**
 *  \file   wsens_srv.c
 *  \brief  Worldsens server
 *  \author Guillaume Chelius
 *  \date   2007
 **/
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
#include <fcntl.h>
#include <netinet/in.h>

#include "wsens_srv.h"


/* ************************************************** */
/* ************************************************** */
struct wsens_srv {
	int fd;
	struct sockaddr_in maddr;	
	int synched;
	uint32_t sseq;
};


/* ************************************************** */
/* ************************************************** */
static uint16_t		    lport      = 9998;
static uint16_t		    mport      = 9999;
static char     	    maddr[256] = "224.0.0.1";
static struct wsens_srv	wsens;


/* ************************************************** */
/* ************************************************** */
void wsens_srv_init(void) {
    wsens.fd = -1;
    return;
} 	

void wsens_srv_finalize(void) {
	if (wsens.fd >= 0) {
		close(wsens.fd);
    }
    wsens.fd = -1;
}


/* ************************************************** */
/* ************************************************** */
int wsens_srv_start(void) {
	struct sockaddr_in addr;
	
	/* unicast socket */
	if ((wsens.fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		fprintf(stderr, "worldsens: unable to open socket (wsens_srv_start())");
		goto error;
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_port        = htons(lport);
	addr.sin_addr.s_addr = INADDR_ANY;
	
        /* bind */
	if (bind(wsens.fd, (struct sockaddr *) (&addr), sizeof(addr)) != 0) {
	  fprintf(stderr, "worldsens: unable to bind socket on port %d \n",lport);
	  goto error;
	}
 	
        /* set non blocking socket */
#if defined(NONBLOCKING_RECV)
	if (fcntl(wsens.fd, F_SETFL, O_NONBLOCK)) {
		fprintf(stderr, "worldsens: unable to set non-blocking socket (wsens_srv_start())");
        goto error;
	}
#endif

	/* initialize multicast */
	memset(&(wsens.maddr), 0, sizeof(wsens.maddr));
	if (inet_aton(maddr, &(wsens.maddr.sin_addr)) == 0) {
		fprintf(stderr, "worldsens: unable to translate IP (wsens_srv_start())");
        goto error;
	}
	wsens.maddr.sin_port   = htons(mport);
	wsens.maddr.sin_family = AF_INET;
	
	wsens.synched = 0;
	wsens.sseq    = 0;
    
	return 0;
    
 error:
	fprintf(stderr, "worldsens: ============================================================\n");
	fprintf(stderr, "worldsens: ============================================================\n");
	fprintf(stderr, "worldsens: == Unable to initialize IP network settings \n");
	fprintf(stderr, "worldsens: == \n");
	fprintf(stderr, "worldsens: == Please change your local port or check that  \n");
	fprintf(stderr, "worldsens: == WSNet is not already running on your machine \n");
	fprintf(stderr, "worldsens: == \n");
	fprintf(stderr, "worldsens: ============================================================\n");
	fprintf(stderr, "worldsens: ============================================================\n");
	sleep(30);
	wsens_srv_finalize();
	return -1;
}	


/* ************************************************** */
/* ************************************************** */
int wsens_srv_send(struct sockaddr_in *addr, char *msg, int len) {
	if (sendto(wsens.fd, msg, len, 0, (struct sockaddr *) addr, sizeof(struct sockaddr_in)) < len) {
		fprintf(stderr, "worldsens: unable to send : %s (wsens_srv_send())", strerror(errno));
        goto error;
	}

    return 0;

 error:
    wsens_srv_finalize();
    return -1;
}


int wsens_srv_msend(char *msg, int len) {
  if (sendto(wsens.fd, msg, len, 0, (struct sockaddr *) &(wsens.maddr), sizeof(struct sockaddr_in)) < len) {
		fprintf(stderr, "worldsens: unable to send : %s (wsens_srv_send())", strerror(errno));
        goto error;
	}
    
    return 0;

 error:
    wsens_srv_finalize();
    return -1;
}

int wsens_srv_rcv(char *msg, int *len, struct sockaddr_in *addr) {
    socklen_t addrlen;
	
    /* receive */
    addrlen = sizeof(*addr);
    memset(addr, 0, sizeof(*addr));

    *len = recvfrom(wsens.fd, msg, WORLDSENS_MAX_PKTLENGTH, 0, (struct sockaddr *) addr, &addrlen);
    if (*len > 0) {
        return 0;
    } else if (errno == EAGAIN) {
        *len = 0;
        return 0;
    } else {        
        fprintf(stderr, "worldsens: error in recvfrom: %s (wsens_srv_rcv())\n", strerror(errno));
        return -1;
    }
    
    return 0;
}
