/**
 *  \file   main.c
 *  \brief  Simulator main
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include "version.h"
#include "internals.h"
#include "xmlparser.h"


/* ************************************************** */
/* ************************************************** */
int do_parse(int argc, char *argv[]);
int do_init(void);
int do_bootstrap(void);
void do_end(void);
void do_clean(void);


/* ************************************************** */
/* ************************************************** */
int main(int argc, char *argv[]) {
    if (do_parse(argc, argv) ||  /* parse arguments */
	do_init()            ||  /* initialization  */
	do_configuration()   ||  /* configuration   */
	do_bootstrap()) {        /* bootstrap       */
        goto end;
    }
    do_observe();                /* observe         */
    do_end();                    /* end             */
    goto end;

 end:
    do_clean();                  /* clean           */
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void usage(void) {    
    fprintf(stderr, "\nWSNet: an event driven simulator for wireless networks - version %s.%s\n", WSNET_VERSION_YEAR, WSNET_VERSION_MONTH);
    fprintf(stderr, "Usage: wsnet [-c configfile] [-S rng-seed] [-R rng-type] [-h] [-V]\n");
    return;
}

void version(void) {
    fprintf(stderr, "WSNet: an event driven simulator for wireless networks - version %s.%s\n", WSNET_VERSION_YEAR, WSNET_VERSION_MONTH);
    return;
}


int do_parse(int argc, char *argv[]) {
    char c;

    while((c = getopt(argc, argv, "c:D:s:R:m:S:h:V")) != -1) {

        switch (c) {
        case 'S':
             default_rng_seed = atoi(optarg);
            break;
        case 'R':
	     default_rng_type_select(optarg);
	    break;
        case 'c':
             config_set_configfile(optarg);
            break;
        case 's':
             config_set_schemafile(optarg);
            break;
	case 'D':
	    worldsens_set_syncperiod(atoi(optarg));
	    break;
        case 'm':
             config_set_sys_modulesdir(optarg);
            break;
        case 'h': 
            usage();
            return -1;
        case 'V': 
            version();
            return -1;
        default: 
            usage();
            return -1;
        }
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
int do_init(void) {
    if (das_init()             ||  /* das        */
	sodas_init()           ||  /* sodas      */
	spadas_init()          ||  /* spadas     */
	hadas_init()           ||  /* hadas      */
	rng_init()             ||  /* rng        */
	timer_init()           ||  /* timer      */
	bundle_init()          ||  /* bundle     */
	entity_init()          ||  /* entity     */
	monitor_init()         ||  /* monitor    */
	measure_init()         ||  /* mesure     */
	medium_init()          ||  /* medium     */
	mobility_init()        ||  /* mobility   */
	modulation_init()      ||  /* modulation */
	noise_init()           ||  /* noise      */
	node_init()            ||  /* node       */
	packet_init()          ||  /* packet     */
	scheduler_init()) {        /* scheduler  */
        return -1;
    }
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int do_bootstrap(void) {
    if (rng_bootstrap()        ||  /* rng        */
	bundle_bootstrap()     ||  /* entity     */
	entity_bootstrap()     ||  /* bundle     */
	monitor_bootstrap()    ||  /* monitor    */
	measure_bootstrap()    ||  /* measure    */
	medium_bootstrap()     ||  /* medium     */
	mobility_bootstrap()   ||  /* mobility   */
	modulation_bootstrap() ||  /* modulation */
	noise_bootstrap()      ||  /* noise      */
	node_bootstrap()       ||  /* node       */
	packet_bootstrap()     ||  /* packet     */
	scheduler_bootstrap()) {   /* scheduler  */
        return -1;
    }
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void do_clean(void) {
    rng_clean();                   /* rng        */
    scheduler_clean();             /* scheduler  */
    packet_clean();                /* packet     */
    noise_clean();                 /* noise      */
    node_clean();                  /* node       */
    modulation_clean();            /* modulation */
    mobility_clean();              /* mobility   */
    medium_clean();                /* medium     */
    measure_clean();               /* measure    */
    monitor_clean();               /* monitor    */
    entity_clean();                /* entity     */
    bundle_clean();                /* bundler    */
    timer_clean();                 /* timer      */
    mem_fs_clean();                /* mem_fs     */
}
