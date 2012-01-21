/**
 *  \file   scheduler.c
 *  \brief  Event scheduler
 *  \author Guillaume Chelius & Elyes Ben Hamida & Loic Lemaitre
 *  \date   2007
 **/
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "../../internals.h"

#include <libraries/worldsens/wsens_srv.h>
#include <include/worldsens_debug.h>


/* ************************************************** */
/* ************************************************** */
#ifndef RUSAGE_SELF
#define RUSAGE_SELF 0
#endif /* RUSAGE_SELF */

/* ************************************************** */
/* ************************************************** */
#define PRIORITY_BIRTH      0x00000000
#define PRIORITY_MOBILITY   0x00000001
#define PRIORITY_TX_END     0x00000002
#define PRIORITY_RX_END     0x00000003
#define PRIORITY_RX_BEGIN   0x00000004
#define PRIORITY_CALLBACK   0x00000005
#define PRIORITY_MILESTONE  0x00000006
#define PRIORITY_QUIT       0x00000007

#define SCHEDULER_MILESTONE_PERIOD 2000000000

//#define WORLDSENS_SYNC_PERIOD   1000000000
ws_time WORLDSENS_SYNC_PERIOD =    500000000; /* 0.5s */


/* ************************************************** */
/* ************************************************** */
typedef enum {
    NO_EXCEPTION,
    EXCEPTION_QUIT,
    EXCEPTION_FATAL,
    EXCEPTION_BACKTRACK
} EXCEPTION;


/* ************************************************** */
/* ************************************************** */
static void *scheduler = NULL;      /* events */
EXCEPTION exception = NO_EXCEPTION; /* current exception */

static int event_id = 0;            /* unique event id     */
static void *mem_event = NULL;      /* memory slice for events */

static uint64_t g_clock = 0;        /* global simulation time */
static uint64_t g_end = 0;          /* global simulation end time */

static struct _debug {
    int c_events;
    int d_events; 
    uint64_t o_unanotime; 
    uint64_t o_clock; 
} dbg = {0, 0, 0, 0};


/* ************************************************** */
/* ************************************************** */
static ws_time        ws_nsync       =  0; /* next synchronization point */
static ws_time        ws_csync       =  0; /* current synchronization point */
static ws_id_rp       ws_csyncseq    =  0; /* current synchronization sequence number */
static ws_id_rp       ws_nsyncseq    =  1; /* next synchronization sequence number */
static int            ws_rp_updated  =  0; /* rp point has already been updated? */
static int            ws_count       =  0; /* number of worldsens nodes */
static int            ws_connected   =  0; /* number of connected nodes */
static int            ws_synched     =  0; /* number of synchronized nodes */
static ws_id_seq      ws_seq         =  1; /* server packet sequence number */
static ws_id_resource ws_default_mod = -1; /* modulation being uses if wsim mod not implemented in wsnet */
static int            ws_backtrack   =  0; /* use to avoid to print an error on a deprecated rp cause by a bracktrack */

typedef struct _ws_node_features {
    ws_id_node        wsim_node_id;    /* correspondence between wsim and wsnet nodes ids */
    position_t        position_backup; /* storage of last node position for backtrack */
    struct _ws_node_features  *next;
} ws_node_features_t;

static ws_node_features_t *ws_nodes_feat = NULL; /* worldsens nodes informations */

typedef struct _ws_rdv {
    ws_time           clock;
    int               priority;
    /* eventually other options */
} ws_rdv_t;

static void *mem_ws_rdv = NULL;    /* memory slice for rdvs */

static void *ws_rdvs = NULL;  /* list of next scheduled rdv (rx end, callbacks, milestones) */


/* ************************************************** */
/* ************************************************** */
event_t *scheduler_next(void);
event_t *scheduler_see_next(void);
void scheduler_add_event(event_t *event);
void scheduler_add_birth(uint64_t clock, nodeid_t id);
void scheduler_add_quit(uint64_t clock);
void scheduler_add_milestone(uint64_t clock);

void do_observe(void);
void do_run(void);
void do_event(event_t *event);
void do_clockadvance(uint64_t clock);

void scheduler_stats(void);
void dump_stats(void);


/* ************************************************** */
/* ************************************************** */
#define WORLDSENS_CONNECTED    0
#define WORLDSENS_DISCONNECTED 1

static void worldsens_add_rdv(ws_time clock, int priority);
static ws_rdv_t *worldsens_rdv_see_next(void);
static ws_rdv_t *worldsens_rdv_next(void);
static void worldsens_rdv_update(event_t *event);
static void worldsens_scheduler_release(void);
static void worldsens_scheduler_backtrack(void);
static void worldsens_rm_duplicate_pk(event_t *event);

static int  worldsens_init(void);
static int  worldsens_bootstrap(int mode);
static void worldsens_clean(void);
static void worldsens_quit(void);

static ws_id_node worldsens_get_wsnet_node_id  (ws_id_node);
static ws_id_node worldsens_get_wsim_node_id   (ws_id_node);
static ws_id_node worldsens_register_node_infos(ws_id_node);

static void worldsens_synchronize(void);
static void worldsens_syncrelease(uint64_t clock);
static void worldsens_syncreminder(void);
static void worldsens_send_reply(void);
static void worldsens_backtrack_rq(uint64_t);
static int  worldsens_callback_tx_measure(call_t *, void *);
static void worldsens_update(void);

static void worldsens_event_dump(event_t *event);
static void worldsens_packet_display(union _worldsens_pkt *);

static int  worldsens_save_nodes_pos(void);
static int  worldsens_restore_nodes_pos(void);


/* ************************************************** */
/* ************************************************** */
int scheduler_compare(void *key0, void *key1) {
    event_t *evt0 = (event_t *) key0;
    event_t *evt1 = (event_t *) key1;

    if (key0 == NULL)
        return 1;

    if (evt0->clock < evt1->clock)
        return 1;
    if (evt0->clock > evt1->clock)
        return -1;
    
    if (evt0->priority < evt1->priority)
        return 1;
    if (evt0->priority > evt1->priority)
        return -1;

    if (evt0->id < evt1->id)
        return 1;
    if (evt0->id > evt1->id)
        return -1;

    return 0;
}


/* ************************************************** */
/* ************************************************** */
int scheduler_init(void) {
    if (sodas_init() == -1) {
        return -1;
    } 

    if ((mem_event = mem_fs_slice_declare(sizeof(event_t))) == NULL) {
        return -1;
    }

    if ((scheduler = sodas_create(scheduler_compare)) == NULL) {
        return -1;
    }

    worldsens_init();

    return 0;
}

void scheduler_clean(void) {
    event_t *event;
    
    if (scheduler == NULL) {
        return;
    }

    while ((event = (event_t *) sodas_pop(scheduler)) != NULL) {
        switch (event->priority) {
        case PRIORITY_RX_END:
            packet_dealloc(event->u.rx.packet);
            break;
        case PRIORITY_RX_BEGIN:
            packet_dealloc(event->u.rx.packet);
            break;
        case PRIORITY_TX_END:
            packet_dealloc(event->u.rx.packet);
            break;
        default:
            break;
        }

        mem_fs_dealloc(mem_event, event);
    }

    sodas_destroy(scheduler);

    worldsens_clean();
}

int scheduler_bootstrap(void) {
    event_t *event;

    /* set first milestone */
    if (ws_count) {  /* WORLDSENS MODE? */
        scheduler_add_milestone(WORLDSENS_SYNC_PERIOD);
    }
    else {
	scheduler_add_milestone(SCHEDULER_MILESTONE_PERIOD);
    }

    /* set end event and worldsens mode */
    if (g_end) {
        scheduler_add_quit(g_end);
    } 

    /* set the first synchronization point */
    event = (event_t *) sodas_see_first(scheduler);
    ws_nsync = event->clock;
    ws_rp_updated = 1;

    if (worldsens_bootstrap(WORLDSENS_CONNECTED)) {
        return -1;
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
void do_end(void) {
    int i = get_node_count();
    
    /* unsetnode */
    while (i--) {
        int j;
        node_t *node = get_node_by_id(i);
        if ((node->state == NODE_DEAD) || (node->state == NODE_UNDEF)) {
            continue;
        }
        for (j = 0; j < bundles.elts[node->bundle].entity.size; j++) {
            entity_t *entity = get_entity_by_id(bundles.elts[node->bundle].entity.elts[j]);
            call_t c = {entity->id, node->id, -1};
            if ((entity)->unsetnode) {
                (entity)->unsetnode(&c);
            }
        }
    }

    /* unsetnode */
    i = entities.size;
    while (i--) {
        entity_t *entity = get_entity_by_id(i);
        call_t c = {entity->id, -1, -1};
        if ((entity)->destroy) {
            (entity)->destroy(&c);
        }
    }
}

void do_observe(void) {
    while (1) {
        do_run();

        switch (exception) {
        case NO_EXCEPTION:
            break;
        case EXCEPTION_QUIT:
            dump_stats();
            return;
        case EXCEPTION_FATAL:
            fprintf(stderr, "Fatal error...\n");
            return;
        case EXCEPTION_BACKTRACK:
            worldsens_scheduler_backtrack();
            break;
        }
    }
}

void do_run(void) {
    event_t *event = NULL;

    while (1) {

        /* synchronize nodes */
        worldsens_synchronize();

        /* catch exceptions */
        if (exception != NO_EXCEPTION) {
            return;
        }

         /* get next event */
        event = scheduler_next();

       /* eventually jump in time */
        if (event->clock > g_clock) {
            do_clockadvance(event->clock);
        }

        /* do event */
        do_event(event);        

        /* catch exceptions */
        if (exception != NO_EXCEPTION) {
            return;
        }

        /* release nodes */
        worldsens_scheduler_release();
    }
}

void do_event(event_t *event) {

    if (ws_count)    /* WORLDSENS MODE? */
        worldsens_rdv_update(event);

    switch (event->priority) {
    case PRIORITY_BIRTH:
        node_birth(event->u.nodeid);
        break;
    case PRIORITY_MOBILITY:
        mobility_event(g_clock);
        break;
    case PRIORITY_RX_BEGIN:
        medium_cs(event->u.rx.packet, &(event->u.rx.call));
	worldsens_rm_duplicate_pk(event);
        break;
    case PRIORITY_RX_END:
        medium_rx(event->u.rx.packet, &(event->u.rx.call));
        break;
    case PRIORITY_TX_END:
        medium_tx_end(event->u.rx.packet, &(event->u.rx.call));
        break;
    case PRIORITY_CALLBACK:
        if ((event->u.cb.call.node != -1) && (!is_node_alive(event->u.cb.call.node))) {
            break;
        }
        event->u.cb.callback(&(event->u.cb.call), event->u.cb.arg);
        break;
    case PRIORITY_MILESTONE:
        scheduler_stats();
	if (ws_count) {  /* WORLDSENS MODE? */
	    scheduler_add_milestone(g_clock + WORLDSENS_SYNC_PERIOD);
	}
	else {
	    scheduler_add_milestone(g_clock + SCHEDULER_MILESTONE_PERIOD);
	}
       break;
    case PRIORITY_QUIT:
        worldsens_quit();
        exception = EXCEPTION_QUIT;
        break;
    default:
        break;
    }

    mem_fs_dealloc(mem_event, event);
    dbg.c_events--;
    dbg.d_events++;
}

void do_clockadvance(uint64_t clock) {
    if (ws_count) {  /* WORLDSENS MODE? */
        WSNET_S_DBG_DBG ("WSNET2:: === ==================================================\n");
	WSNET_S_DBG_OUT ("WSNET2:: === TIME ADVANCED FROM %"PRId64" TO %"PRId64" (rp seq: %"PRId64")\n", get_time(), clock, ws_csyncseq);
	WSNET_S_DBG_DBG ("WSNET2:: === ==================================================\n");
    }
    mobility_update(clock);
    g_clock = clock;
}


/* ************************************************** */
/* ************************************************** */
void end_simulation(void) {
    exception = EXCEPTION_QUIT;
    return;
}


/* ************************************************** */
/* ************************************************** */
void scheduler_add_birth(uint64_t clock, nodeid_t id) {
    event_t *event;

    if ((event = (event_t *) mem_fs_alloc(mem_event)) == NULL) {
        return;
    }
    event->clock = clock;
    event->priority = PRIORITY_BIRTH;
    event->u.nodeid = id;
    scheduler_add_event(event);
}

void scheduler_add_quit(uint64_t clock) {
    event_t *event;

    if ((event = (event_t *) mem_fs_alloc(mem_event)) == NULL) {
        return;
    }
    event->clock = clock;
    event->priority = PRIORITY_QUIT;
    scheduler_add_event(event);

    /* if worldsens mode save this event as rdv */
    if (ws_count)
        worldsens_add_rdv(clock, PRIORITY_QUIT);
}

void scheduler_add_mobility(uint64_t clock) {
    event_t *event;

    if ((event = (event_t *) mem_fs_alloc(mem_event)) == NULL) {
        return;
    }
    event->clock = clock;
    event->priority = PRIORITY_MOBILITY;
    scheduler_add_event(event);
}

void scheduler_add_milestone(uint64_t clock) {
    event_t *event;

    if ((event = (event_t *) mem_fs_alloc(mem_event)) == NULL) {
        return;
    }
    event->clock = clock;
    event->priority = PRIORITY_MILESTONE;
    scheduler_add_event(event);

    /* if worldsens mode save this event as rdv */
    if (ws_count)
        worldsens_add_rdv(clock, PRIORITY_MILESTONE);
}

event_t *scheduler_add_callback(uint64_t clock, call_t *c, callback_t callback, void *arg) {
    event_t *event;

    if ((scheduler_get_end()) && (clock > scheduler_get_end())) {
        return NULL;
    }

    if ((event = (event_t *) mem_fs_alloc(mem_event)) == NULL) {
        fprintf(stderr, "scheduler.c: add callback node %d : ERROR when allocating event\n", c->node);
        return NULL;
    }
    event->clock = clock;
    event->u.cb.call.entity = c->entity;
    event->u.cb.call.node = c->node;
    event->u.cb.call.from = c->from;
    event->u.cb.callback = callback;
    event->u.cb.arg = arg;
    event->priority = PRIORITY_CALLBACK;
    scheduler_add_event(event);

    /* if worldsens mode save this event as rdv */
    if (ws_count)
        worldsens_add_rdv(clock, PRIORITY_CALLBACK);

    return event;
}

void scheduler_add_rx_begin(uint64_t clock, call_t *c, packet_t *packet) {
    event_t *event;

    if ((event = (event_t *) mem_fs_alloc(mem_event)) == NULL) {
        return;
    }
    event->clock = clock;
    event->u.rx.packet = packet;
    event->u.rx.call.entity = c->entity;
    event->u.rx.call.node = c->node;
    event->u.rx.call.from = c->from;
    event->priority = PRIORITY_RX_BEGIN;
    scheduler_add_event(event);

    /* if worldsens mode save this event as rdv */
    if (ws_count)
        worldsens_add_rdv(clock, PRIORITY_RX_BEGIN);

    return;
}

void scheduler_add_rx_end(uint64_t clock, call_t *c, packet_t *packet) {
    event_t *event;

    if ((event = (event_t *) mem_fs_alloc(mem_event)) == NULL) {
        return;
    }
    event->clock = clock;
    event->u.rx.packet = packet;
    event->u.rx.call.entity = c->entity;
    event->u.rx.call.node = c->node;
    event->u.rx.call.from = c->from;
    event->priority = PRIORITY_RX_END;
    scheduler_add_event(event);

    /* if worldsens mode save this event as rdv */
    if (ws_count)
        worldsens_add_rdv(clock, PRIORITY_RX_END);
    
    return;
}

void scheduler_add_tx_end(uint64_t clock, call_t *c, packet_t *packet) {
    event_t *event;

    if ((event = (event_t *) mem_fs_alloc(mem_event)) == NULL) {
        return;
    }
    event->clock = clock;
    event->u.rx.packet = packet;
    event->u.rx.call.node = c->node;
    event->u.rx.call.entity = c->entity;
    event->u.rx.call.from = c->from;
    event->priority = PRIORITY_TX_END;
    scheduler_add_event(event);
    return;
}

/* ************************************************** */
/* ************************************************** */

/* edit by Ibrahim Amadou <ibrahim.amadou@insa-lyon.fr> */
void scheduler_delete_callback(call_t *c, event_t *event) {
  event_t *event1 = NULL;

  if((event->clock > get_time()) && (event->clock < scheduler_get_end())) {
      event1 = (event_t *) sodas_delete(scheduler, event);
      if(event1 != NULL) {
         dbg.c_events--;
	 mem_fs_dealloc(mem_event, event1);
      }
      return;
  }
 
  return;
}
/* end of edition */

/* ************************************************** */
/* ************************************************** */
void scheduler_add_event(event_t *event) {
    event->id = event_id++;
    sodas_insert(scheduler, event, event);
    dbg.c_events++;
}

event_t *scheduler_next(void) {
    return (event_t *) sodas_pop(scheduler);
}

event_t *scheduler_see_next(void) {
    return (event_t *) sodas_see_first(scheduler);
}

/* ************************************************** */
/* ************************************************** */
void scheduler_set_end(uint64_t end) {
    g_end = end;
}

uint64_t scheduler_get_end(void) {
    return g_end;
}


/* ************************************************** */
/* ************************************************** */
void scheduler_stats(void) {
    struct rusage ru;
    uint64_t unanotime;
    double speedup;
    double i_speedup;

    getrusage(RUSAGE_SELF,&ru);
#define NANO  (1000*1000*1000)
#define MICRO (1000)
    unanotime = ((uint64_t) ru.ru_utime.tv_sec) * NANO + ((uint64_t) ru.ru_utime.tv_usec) * MICRO;
    speedup = ((double) g_clock) / ((double) unanotime);
    i_speedup = ((double) (g_clock - dbg.o_clock)) / ((double) (unanotime - dbg.o_unanotime));
    dbg.o_clock = g_clock;
    dbg.o_unanotime = unanotime;

    /*    fprintf(stderr, "\nSimulation stats:\n  simulated time: %"PRId64"\n  simulation time: %"PRId64"\n  speedup: %lf\n  instantaneous speedup: %lf\n  events in queue: %d\n  events executed: %d\n", 
            g_clock,
            unanotime,
            speedup,
              i_speedup,
            dbg.c_events, 
            dbg.d_events); */
   
}

void dump_stats(void) {
    struct rusage ru;
    uint64_t unanotime;
    double speedup;

    getrusage(RUSAGE_SELF,&ru);
#define NANO  (1000*1000*1000)
#define MICRO (1000)
    unanotime = ((uint64_t) ru.ru_utime.tv_sec) * NANO + ((uint64_t) ru.ru_utime.tv_usec) * MICRO;
    speedup = ((double) g_clock) / ((double) unanotime);
    dbg.o_clock = g_clock;
    dbg.o_unanotime = unanotime;
    
    fprintf(stderr, "\nSimulation stats:\n  simulated time: %"PRId64"\n  simulation time: %"PRId64"\n  speedup: %lf\n  events in queue: %d\n  events executed: %d\n", 
            g_clock,
            unanotime,
            speedup,
            dbg.c_events, 
            dbg.d_events);
}

/* ************************************************** */
/* ************************************************** */
uint64_t get_time(void) {
    return g_clock;
}


/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

void worldsens_set_syncperiod(uint64_t period)
{
  WORLDSENS_SYNC_PERIOD = period;
}

/* ************************************************** */
/* ************************************************** */

void worldsens_set_count(int count) {
    ws_count = count;
}


/* ************************************************** */
/* ************************************************** */
static int worldsens_compare(void *key0, void *key1) {
    ws_rdv_t *rdv0 = (ws_rdv_t *) key0;
    ws_rdv_t *rdv1 = (ws_rdv_t *) key1;

    if (key0 == NULL)
        return 1;

    if (rdv0->clock < rdv1->clock)
        return 1;
    if (rdv0->clock > rdv1->clock)
        return -1;

    return 0;
}

static void worldsens_add_rdv(ws_time clock, int priority) {
    ws_rdv_t *rdv;
    if ((rdv = (ws_rdv_t *) mem_fs_alloc(mem_ws_rdv)) == NULL) {
        return;
    }
    rdv->clock    = clock;
    rdv->priority = priority;
    sodas_insert(ws_rdvs, rdv, rdv);
}

static ws_rdv_t *worldsens_rdv_see_next(void) {
    return (ws_rdv_t *) sodas_see_first(ws_rdvs);
}

static ws_rdv_t *worldsens_rdv_next(void) {
    return (ws_rdv_t *) sodas_pop(ws_rdvs);
}

static void worldsens_rdv_update(event_t *event) {
    if ( (event->priority == PRIORITY_QUIT)     || 
	 (event->priority == PRIORITY_RX_BEGIN) || 
	 (event->priority == PRIORITY_RX_END)   ||
	 (event->priority == PRIORITY_CALLBACK) ||
	 (event->priority == PRIORITY_MILESTONE) ) {
        ws_rdv_t *rdv;
        rdv = worldsens_rdv_next();
	if (rdv->clock != event->clock) {
	    WSNET_S_DBG_DBG ("WSNET2:: === ==================================================\n");
	    WSNET_S_DBG_DBG ("WSNET2:: === ERROR: CORE DESYNCHRONIZATION, RDV TIME %"PRId64" != EVENT TIME %"PRId64" (rp seq: %"PRId64")\n", rdv->clock, event->clock, ws_csyncseq);
	    WSNET_S_DBG_DBG ("WSNET2:: === ==================================================\n");
	}
        worldsens_event_dump(event);
	mem_fs_dealloc(mem_ws_rdv, rdv);
    }
}

static void worldsens_send_reply(void) {
    ws_rdv_t *next_rdv = worldsens_rdv_see_next();
    
    /* need to backtrack? */
    if (next_rdv->clock < ws_nsync) {
        worldsens_backtrack_rq(next_rdv->clock - ws_csync);
    }
    else {
        /* a node just have sent a TX and wait for a reply, so send a rdv reminder */
        worldsens_syncreminder();
    }
}

/* ************************************************** */
/* ************************************************** */
static void worldsens_rm_duplicate_pk(event_t *event) {

    if (ws_count) {
        event_t *next_event = scheduler_see_next();

	/* A packet sent by a node has the same id for each receiving nodes.
	   So if we find a packet with same source node, same clock and differents ids,
	   it is a duplicated packet (because of a backtrack) and we have to skip it. */

	while(next_event->priority            == PRIORITY_RX_BEGIN          && 
	      next_event->u.rx.packet->node   == event->u.rx.packet->node   && 
	      next_event->u.rx.packet->clock0 == event->u.rx.packet->clock0 && 
	      next_event->u.rx.packet->id     != event->u.rx.packet->id)  {
 	    /* get out of the fifo the duplicated packet */
	    event_t *trash_event = scheduler_next();
	    worldsens_rdv_update(trash_event);
	    WSNET_S_DBG_DBG("WSNET2:: --> RX BEGIN: same packet found in fifo, skip it (src ip:%d, data:0x%02x, freq:%ghz, wsim modul:%d)\n", worldsens_get_wsim_node_id(next_event->u.rx.packet->node), *(next_event->u.rx.packet->data), next_event->u.rx.packet->worldsens_freq, next_event->u.rx.packet->worldsens_mod);
	    mem_fs_dealloc(mem_event, trash_event);
	    dbg.c_events--;
	    next_event = scheduler_see_next();
	}
    }    
}

/* ************************************************** */
/* ************************************************** */
static int worldsens_init(void) {
    wsens_srv_init();

    if ((mem_ws_rdv = mem_fs_slice_declare(sizeof(ws_rdv_t))) == NULL) {
        return -1;
    }

    if ((ws_rdvs = sodas_create(worldsens_compare)) == NULL) {
        return -1;
    }

    return 0;
}

static int worldsens_bootstrap(int mode) {
    if (!ws_count) {
        return 0;
    }
    
    if (wsens_srv_start()) {
        return -1;
    }

    if (mode == WORLDSENS_CONNECTED) {
        WSNET_S_DBG_DBG ("\nWaiting for %d worldsens client connections...\n", ws_count);
        while (ws_connected != 1) {
            worldsens_update();
            if (exception != NO_EXCEPTION) {
                return -1;
            }
        }
    }

    return 0;
}

static void worldsens_clean(void) {
    if (ws_count) {
        wsens_srv_finalize();

	/* free node informations space */
	ws_node_features_t *p_temp = ws_nodes_feat;
	ws_node_features_t *p_free = NULL;
	while(p_temp != NULL) {
	    p_free = p_temp;
	    p_temp = p_temp->next;
	    free(p_free);
	}
	
	/* free rdvs */
	ws_rdv_t *rdv;
	while ((rdv = (ws_rdv_t *) sodas_pop(ws_rdvs)) != NULL) {
	    mem_fs_dealloc(mem_ws_rdv, rdv);
	}
    }
}

static void worldsens_quit(void) {
    if (ws_connected == 0) {
        return;
    } else {
        union _worldsens_pkt pkt;

        pkt.killsim.type = WORLDSENS_S_KILLSIM;
        pkt.killsim.seq = ws_seq++;
        worldsens_packet_hton(&pkt);
        worldsens_packet_display(&pkt);

        wsens_srv_msend((char *) &pkt, sizeof(struct _worldsens_s_killsim));
        ws_connected = 0;
    }
}


/* ************************************************** */
/* ************************************************** */
void worldsens_scheduler_release(void) {
    ws_rdv_t *next_rdv;
    uint64_t sync;

    if (ws_count) {

        if (ws_connected == 0 || ws_rp_updated == 1) {
	    return;
	}

	/* update the current and next synchronization points */
	next_rdv = worldsens_rdv_see_next();
	
	if( (next_rdv->clock > get_time()) && (ws_synched >= ws_connected) ) {
	    sync = next_rdv->clock;
    
	    /* program next rendez-vous-point */
	    worldsens_syncrelease(sync);
	    
	    /* save nodes positions */
	    worldsens_save_nodes_pos(); /* not usefull if sync at each event*/
	}
    }
}

static void worldsens_scheduler_backtrack(void) {

    /* restore time */
    WSNET_S_DBG_DBG ("WSNET2:: === ==================================================\n");
    WSNET_S_DBG_DBG ("WSNET2:: === TIME BACKTRACKED FROM %"PRId64" TO %"PRId64" (rp seq: %"PRId64")\n",
		     get_time(), ws_csync, ws_csyncseq);
    WSNET_S_DBG_DBG ("WSNET2:: === ==================================================\n");
    g_clock = ws_csync;

    /* restore nodes position */
    worldsens_restore_nodes_pos();  /* not usefull if sync at each event*/
    
    /* reset exception */
    exception = NO_EXCEPTION;
}


/* ************************************************** */
/* ************************************************** */
static uint32_t worldsens_get_wsnet_node_id(ws_id_node wsim_id) {
    ws_node_features_t *p_temp = ws_nodes_feat;
    ws_id_node wsnet_id = 0;

    if(p_temp == NULL) {
        return -1;
    }

    while(p_temp->wsim_node_id != wsim_id) {
        p_temp = p_temp->next;
        if(p_temp == NULL) {
	    return -1;
	}
	wsnet_id++;
    }

    return wsnet_id;

}

static uint32_t worldsens_get_wsim_node_id (ws_id_node wsnet_id) {
    ws_node_features_t *p_temp = ws_nodes_feat;
    ws_id_node i;

    if(p_temp == NULL) {
        return -1;
    }

    for(i = 0; i < wsnet_id; i++) {
        p_temp = p_temp->next;
        if(p_temp == NULL) {
	    return -1;
	}
    }

    return p_temp->wsim_node_id;
}

static uint32_t worldsens_register_node_infos (ws_id_node wsim_id) {
    ws_id_node wsnet_id = 0;
    ws_node_features_t *ws_node_feat_next = malloc(sizeof(ws_node_features_t));

    /* puts new structure at the end of the pointer chain */
    if(ws_nodes_feat == NULL) {
        ws_nodes_feat = ws_node_feat_next;
    }
    else {
        ws_node_features_t *p_temp = ws_nodes_feat;
	wsnet_id = 1;
	while(p_temp->next != NULL) {
   	    p_temp = p_temp->next;
	    wsnet_id++;
	}
	p_temp->next = ws_node_feat_next;
    }    

    /* fills node infos */
    node_t *node = get_node_by_id(wsnet_id);
    ws_node_feat_next->wsim_node_id = wsim_id;
    ws_node_feat_next->position_backup.x = node->position.x;
    ws_node_feat_next->position_backup.y = node->position.y;
    ws_node_feat_next->position_backup.z = node->position.z;
    ws_node_feat_next->next = NULL;

    return wsnet_id;
}

static int worldsens_save_nodes_pos(void) {
    ws_node_features_t *p_node_feat = ws_nodes_feat;
    int i;
    node_t *node;

    for(i=0; i<ws_connected; i++) {
        if(p_node_feat == NULL) {
	    return -1;
	}
	node = get_node_by_id(i);
        p_node_feat->position_backup.x = node->position.x;
	p_node_feat->position_backup.y = node->position.y;
	p_node_feat->position_backup.z = node->position.z;
	p_node_feat = p_node_feat->next;
    }

    return 0;
}

static int worldsens_restore_nodes_pos(void) {
    ws_node_features_t *p_node_feat = ws_nodes_feat;
    int i;
    node_t *node;

    for(i=0; i<ws_connected; i++) {
        if(p_node_feat == NULL) {
	    return -1;
	}
	node = get_node_by_id(i);
        node->position.x = p_node_feat->position_backup.x;
        node->position.y = p_node_feat->position_backup.y;
        node->position.z = p_node_feat->position_backup.z;
	p_node_feat = p_node_feat->next;
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
static void worldsens_synchronize(void) {
    event_t  *next_event;
    ws_rdv_t *next_rdv;
	
    if (ws_count) {
        next_event = scheduler_see_next();
	next_rdv   = worldsens_rdv_see_next();

        if (ws_connected == 0 || ws_rp_updated == 0 || next_rdv->clock != next_event->clock) {
	    return;
	}

	if(next_rdv->clock >= ws_nsync) {
  	    while (ws_synched != ws_connected) {
	        worldsens_update();
		if (exception != NO_EXCEPTION) {
		    return;
		}
	    }  
	    /* ws_csync update */
	    ws_rp_updated   = 0;
	    ws_csync     = ws_nsync;
	    ws_csyncseq  = ws_nsyncseq;
	    ws_backtrack = 0;
	}
    }

    return;
}

static void worldsens_syncrelease(uint64_t clock) {
    if (ws_connected == 0) {
        return;
    } else {
        union _worldsens_pkt pkt;
        
        /* sync updates */
        ws_nsync = clock;
        ws_nsyncseq++;
	ws_rp_updated = 1;
	ws_synched = 0;

        /* forge and send packet */
        pkt.sync_release.type        = WORLDSENS_S_SYNC_RELEASE;
        pkt.sync_release.seq         = ws_seq++;
        pkt.sync_release.rp_next     = ws_nsyncseq;
        pkt.sync_release.rp_duration = ws_nsync - ws_csync;

        worldsens_packet_display(&pkt);
        worldsens_packet_hton(&pkt);

        wsens_srv_msend((char *) &pkt, sizeof(struct _worldsens_s_sync_release));

	WSNET_S_DBG_DBG("WSNET2:: --> RELEASE (seq: %"PRId64")\n", ws_seq - 1);
	WSNET_S_DBG_DBG("WSNET2:: --> RP (seq: %"PRId64") (rp seq: %"PRId64", period: %"PRId64", rp:%"PRId64")\n", ws_seq - 1, ws_nsyncseq, ws_nsync - ws_csync, ws_nsync);
    }
}

static void worldsens_syncreminder(void) {
    union _worldsens_pkt pkt;

    /* forge and send packet */
    pkt.sync_reminder.type    = WORLDSENS_S_SYNC_REMINDER;
    pkt.sync_reminder.seq     = ws_seq++;
    pkt.sync_reminder.rp_next = ws_nsyncseq;

    worldsens_packet_display(&pkt);
    worldsens_packet_hton(&pkt);

    wsens_srv_msend((char *) &pkt, sizeof(struct _worldsens_s_sync_release));

    WSNET_S_DBG_DBG("WSNET2:: --> RP REMINDER (seq: %"PRId64") (rp seq: %"PRId64")\n", ws_seq - 1, ws_nsyncseq);
}

static void worldsens_rx_sync_ack(struct _worldsens_c_sync_ack *pkt) {
    if (pkt->rp_id == ws_nsyncseq) {
        ws_synched++;
	WSNET_S_DBG_DBG("WSNET2:: <-- SYN(%d/%d)  (rp seq: %"PRId64")\n", ws_synched, ws_connected, pkt->rp_id);
    }
    /* a late sync may be received after backtrack has been requested : print nothing in that case */
    else if (!ws_backtrack) { 
        WSNET_S_DBG_DBG("WSNET2:: ERROR: Deprecated rdv acknowledge, received rdv seq %"PRId64" while rdv seq %"PRId64" expected\n", pkt->rp_id, ws_nsyncseq);
    }
}

static void worldsens_rx_disconnect(struct _worldsens_c_disconnect *pkt) {
    node_t *node = get_node_by_id(worldsens_get_wsnet_node_id(pkt->node_id));

    if (node->worldsens != NODE_CONNECTED) {
        return;
    }
    
    node->worldsens = NODE_DISCONNECTED;
    node_kill(node->id);

    if (ws_connected == 0 && ws_count == get_node_count())
      {
	end_simulation();
      }
}

void worldsens_tx_kill(nodeid_t id) {
    union _worldsens_pkt pkt;

    pkt.kill.type    = WORLDSENS_S_KILL;
    pkt.kill.seq     = ws_seq++;
    pkt.kill.node_id = id;
    worldsens_packet_display(&pkt);
    worldsens_packet_hton(&pkt);
    
    wsens_srv_msend((char *) &pkt, sizeof(struct _worldsens_s_kill));
    ws_connected--;
}


/* ************************************************** */
/* ************************************************** */
/**
 * Forge response to a node connect request
 */
static void worldsens_rx_connect_req(struct _worldsens_c_connect_req *pkt, struct sockaddr_in *addr) {

    node_t   *node   = get_node_by_id(worldsens_register_node_infos(pkt->node_id));
    bundle_t *bundle = get_bundle_by_id(node->bundle);
    entity_t *entity;
    union _worldsens_pkt pkt0;
    int offset = 0;
    int i;
    int nb_mod = 0;

    if (node->worldsens != NODE_DISCONNECTED) {
        //PRINT_WORLDSENS("wsnet:worldsens_rx_connect_req (%"PRId64"): node %d not disconnected!\n", get_time(), node->id);
        return;
    }
    
    /* forge response*/
    pkt0.cnx_rsp_ok.type = WORLDSENS_S_CONNECT_RSP_OK;
    pkt0.cnx_rsp_ok.seq = ws_seq;
    pkt0.cnx_rsp_ok.rp_next = ws_nsyncseq;
    pkt0.cnx_rsp_ok.rp_duration = ws_nsync - ws_csync;
    pkt0.cnx_rsp_ok.n_antenna_id = bundle->antenna.size;
    pkt0.cnx_rsp_ok.n_measure_id = measures.size;

   /* pkt->cnx_rsp_ok.names_and_ids format: */

/*|***************antennas***************|**************modulations*************|***************measures***************|*/
/*|ant id1|ant name1|ant id2|ant name2|..|mod id1|mod name1|mod id2|mod name2|..|mea id1|mea name1|mea id2|mea name2|..|*/
/*  *********************************************************************************************************************/

    /* forge list of available antennas */
    for (i = 0; i < bundle->antenna.size; i++) {
        entity = get_entity_by_id(bundle->antenna.elts[i]);
	/* PRINT_WORLDSENS("wsnet: worldsens_rx_connect_req (%"PRId64"): antenna '%s' 
	   (id %d) available\n", get_time(), entity->name, entity->id); */
        *((uint32_t *) (pkt0.cnx_rsp_ok.names_and_ids + offset)) = entity->id;
        offset += sizeof(uint32_t);
        strcpy(pkt0.cnx_rsp_ok.names_and_ids + offset, entity->name);
        offset += strlen(entity->name) + 1;
    }

    /* forge list of available modulations */
    das_init_traverse(modulation_entities);
    while ((entity = (entity_t *) das_traverse(modulation_entities)) != NULL) {
        /*PRINT_WORLDSENS("wsnet: worldsens_rx_connect_req (%"PRId64"): modulation '%s' 
	  (id %d) available\n", get_time(), entity->library.name, entity->id); */
        *((uint32_t *) (pkt0.cnx_rsp_ok.names_and_ids + offset)) = entity->id;
        offset += sizeof(uint32_t);
        strcpy(pkt0.cnx_rsp_ok.names_and_ids + offset, entity->library.name);
        offset += strlen(entity->library.name) + 1;
	nb_mod++;
	if ((nb_mod == 1) || (strcmp(entity->library.name, "modulation_none") == 0)) {
	    ws_default_mod = entity->id;
	}
    }
    pkt0.cnx_rsp_ok.n_modulation_id = nb_mod;

    /* forge list of available measure */
    for (i = 0; i < measures.size; i++) {
        measure_t *measure = get_measure_by_id(i);
	/* PRINT_WORLDSENS("wsnet: worldsens_rx_connect_req (%"PRId64"): measure '%s' 
	   (id %d) available\n", get_time(), measure->name, measure->id); */
        *((uint32_t *) (pkt0.cnx_rsp_ok.names_and_ids + offset)) = measure->id;
        offset += sizeof(uint32_t);
        strcpy(pkt0.cnx_rsp_ok.names_and_ids + offset, measure->name);
        offset += strlen(measure->name) + 1;
    }
    
    /* give birth to node */
    ws_connected++;
    node->worldsens = NODE_CONNECTED;
    node_birth(node->id);

    WSNET_S_DBG_DBG("WSNET2:: <-- CONNECT(%d/%d) (ip: %d)\n", ws_connected, ws_count, pkt->node_id);
    WSNET_S_DBG_DBG("WSNET2:: --> RP (seq: %"PRId64") (rp seq: %"PRId64", period: %"PRId64", rp:%"PRId64")\n", ws_seq, ws_nsyncseq, ws_nsync - ws_csync, ws_nsync);

    /* send response */
    worldsens_packet_display(&pkt0);
    worldsens_packet_hton(&pkt0);
    wsens_srv_send(addr, (char *) &pkt0, sizeof(struct _worldsens_s_connect_rsp_ok));    
}


/**
 * Deal with data sent by a node
 */
static void worldsens_tx_byte(struct _worldsens_c_byte_tx *pkt){
    uint64_t tx_start = ws_csync + pkt->period;
    uint64_t tx_end   = tx_start + pkt->duration;

    /* create packet */
    packet_t *packet;
    node_t   *node   = get_node_by_id(worldsens_get_wsnet_node_id(pkt->node_id));
    entity_t *entity = get_entity_by_id(pkt->antenna_id);
    call_t c = {entity->id, node->id, -1};

    /* put uint64_t into doubles to retrieve double value after swap */
    double *power_dbm = (double *) &(pkt->power_dbm);
    double *freq      = (double *) &(pkt->freq);  
    
    packet = packet_alloc(&c, 1);

    packet->clock0         =  tx_start;                /* tx start */
    packet->clock1         =  tx_end;                  /* tx end */
    packet->duration       =  pkt->duration;
    packet->node           =  node->id;                /* source */
    packet->antenna        =  pkt->antenna_id;
    packet->worldsens_mod  =  pkt->wsim_mod_id;        /* radio modulation in wsim. Better when it matches with wsnet modulation... */
    packet->channel        =  0;                       /* TODO: handle channel from wsim radio */
    packet->worldsens_freq = *freq;
    packet->txdBm          = *power_dbm;
    packet->Tb             =  pkt->duration / packet->real_size;
    *(packet->data)        =  pkt->data;

    if (pkt->wsnet_mod_id == -1)
        packet->modulation = ws_default_mod;
    else 
        packet->modulation = pkt->wsnet_mod_id;       /* modulation we are using in wsnet for calculation */

    /* log informations*/
    int iii;
    WSNET_S_DBG_DBG ("WSNET2:: <-- TX (%"PRId64") (src ip:%d,size:%d,data",
		     packet->clock0, pkt->node_id, packet->size);
    for(iii=0; iii<packet->size; iii++) {
        WSNET_S_DBG_DBG(":%02x", packet->data[ iii ] & 0xff);
    }
    WSNET_S_DBG_DBG (",duration:%"PRId64",freq:%ghz,wsim modul:%d,tx:%lgdbm)\n",
		     packet->duration, packet->worldsens_freq, packet->worldsens_mod, packet->txdBm);

    /* proceed TX */
    MEDIA_TX(&c, packet);

    /* reply by sending a backtrack or a rp reminder */
    worldsens_send_reply();

    return;
}


/* ************************************************** */
/* ************************************************** */
/**
 * Retransmit data to nodes
 **/
int worldsens_nodes_rx(call_t *c, packet_t *packet){

    ws_rdv_t *next_rdv   = worldsens_rdv_see_next();

    if(next_rdv->clock  == get_time()){ /* just send data (no rdv)*/
        union _worldsens_pkt pkt_rx;
	
	/* compute sinr */
	double sinr = packet->rxdBm - mW2dBm(*(packet->noise_mW));

	/* put doubles into uint64_t variables for swap */
	uint64_t *prxdBm          = (uint64_t *) &(packet->rxdBm);
	uint64_t *pworldsens_freq = (uint64_t *) &(packet->worldsens_freq);
	uint64_t *psinr           = (uint64_t *) &(sinr);

	/* forge msg */
	pkt_rx.byte_rx.type        =  WORLDSENS_S_BYTE_RX;
	pkt_rx.byte_rx.seq         =  ws_seq++;
	pkt_rx.byte_rx.antenna_id  =  packet->antenna;
	pkt_rx.byte_rx.wsim_mod_id =  packet->worldsens_mod;
	pkt_rx.byte_rx.freq        = *pworldsens_freq;
	pkt_rx.byte_rx.data        = *(packet->data);
	pkt_rx.byte_rx.node_id     =  worldsens_get_wsim_node_id(c->node);
	pkt_rx.byte_rx.power_dbm   = *prxdBm;
	pkt_rx.byte_rx.sinr        = *psinr;

	/* send data */
	worldsens_packet_display(&pkt_rx);
	worldsens_packet_hton   (&pkt_rx);
	if(wsens_srv_msend((char *) &pkt_rx, sizeof(struct _worldsens_s_byte_rx))){
	    return -1;  
	}

	WSNET_S_DBG_DBG("WSNET2:: --> RX (dest ip:%d, data:0x%02x, freq:%ghz, wsim modul:%d, power:%gdbm)\n", worldsens_get_wsim_node_id(c->node), (*(packet->data)) & 0xff, packet->worldsens_freq, packet->worldsens_mod, packet->rxdBm);

	packet_dealloc(packet);

    }

    else { /* program new rdv and send data */
        union _worldsens_pkt pkt_sr_rx;

	/* compute sinr */
	double sinr = packet->rxdBm - mW2dBm(*(packet->noise_mW));

	/* put doubles into uint64_t variables for swap */
	uint64_t *prxdBm          = (uint64_t *) &(packet->rxdBm);
	uint64_t *pworldsens_freq = (uint64_t *) &(packet->worldsens_freq);
	uint64_t *psinr           = (uint64_t *) &(sinr);

	/* update next rdv */    
	ws_nsync = next_rdv->clock;
	ws_nsyncseq++;
	ws_synched = 0;
	ws_rp_updated = 1;

	/* forge msg */
	pkt_sr_rx.byte_sr_rx.type        =  WORLDSENS_S_BYTE_SR_RX;
	pkt_sr_rx.byte_sr_rx.seq         =  ws_seq++;
	pkt_sr_rx.byte_sr_rx.rp_next     =  ws_nsyncseq;
	pkt_sr_rx.byte_sr_rx.rp_duration =  ws_nsync - ws_csync;
	pkt_sr_rx.byte_sr_rx.antenna_id  =  packet->antenna;
	pkt_sr_rx.byte_sr_rx.wsim_mod_id =  packet->worldsens_mod;
	pkt_sr_rx.byte_sr_rx.freq        = *pworldsens_freq;
	pkt_sr_rx.byte_sr_rx.data        = *(packet->data);
	pkt_sr_rx.byte_sr_rx.node_id     =  worldsens_get_wsim_node_id(c->node);
	pkt_sr_rx.byte_sr_rx.power_dbm   = *prxdBm;
	pkt_sr_rx.byte_sr_rx.sinr        = *psinr;

	/* send data */
	worldsens_packet_display(&pkt_sr_rx);
	worldsens_packet_hton   (&pkt_sr_rx);
	if(wsens_srv_msend((char *) &pkt_sr_rx, sizeof(struct _worldsens_s_byte_sr_rx))){
	    return -1;  
	}

	WSNET_S_DBG_DBG("WSNET2:: --> RX (dest ip:%d, data:0x%02x, freq:%ghz, wsim modul:%d, power:%gdbm)\n", worldsens_get_wsim_node_id(c->node), (*(packet->data)) & 0xff, packet->worldsens_freq, packet->worldsens_mod, packet->rxdBm);
	WSNET_S_DBG_DBG("WSNET2:: --> RP (seq: %"PRId64") (rp seq: %"PRId64", period: %"PRId64", rp:%"PRId64")\n", ws_seq - 1, ws_nsyncseq, ws_nsync - ws_csync, ws_nsync);

	packet_dealloc(packet);

    }
    
    return 0;
}


/* ************************************************** */
/* ************************************************** */
/**
 * Program new rp and backtrack wsnet scheduler
 **/
static void worldsens_backtrack_rq(uint64_t period){
    union _worldsens_pkt pkt;

    /* sync update */
    ws_nsync = ws_csync + period;
    ws_nsyncseq++;
    ws_rp_updated = 1;
    ws_synched = 0;
    ws_backtrack = 1;

    WSNET_S_DBG_EXC("WSNET2:: --> BACKTRACK (seq: %"PRId64")(rp seq: %"PRId64", period: %"PRId64", rp:%"PRId64")\n", ws_seq - 1, ws_nsyncseq, period, ws_nsync);

    /* forge and send packet */
    pkt.bktrk.type = WORLDSENS_S_BACKTRACK;
    pkt.bktrk.seq = ws_seq++;
    pkt.bktrk.rp_next = ws_nsyncseq;
    pkt.bktrk.rp_duration = ws_nsync - ws_csync;
  
    worldsens_packet_display(&pkt);
    worldsens_packet_hton(&pkt);

    wsens_srv_msend((char *) &pkt, sizeof(struct _worldsens_s_backtrack));

    return;
}


/* ************************************************** */
/* ************************************************** */
/**
 * Program new callback event to send measure and send new rp
 **/
static int worldsens_rx_measure_req(struct _worldsens_c_measure_req *msg) {

    call_t c = {-1, worldsens_get_wsnet_node_id(msg->node_id), -1};

    /* create a copy of the received packet */
    struct _worldsens_c_measure_req *msg_cpy = malloc(sizeof(struct _worldsens_c_measure_req));
    msg_cpy->type       = msg->type;
    msg_cpy->node_id    = msg->node_id;
    msg_cpy->measure_id = msg->measure_id;
    msg_cpy->period     = msg->period;

    /* add immediate measure event to scheduler */
    scheduler_add_callback(get_time() + msg->period, &c, worldsens_callback_tx_measure, (void *) msg_cpy);

    /* reply by sending a backtrack or a rp reminder */
    worldsens_send_reply();

    return 0;
}

/**
 * Function to be callback for measure sending
 **/
static int worldsens_callback_tx_measure(call_t *c, void *arg) {

    struct _worldsens_c_measure_req *msg = (struct _worldsens_c_measure_req *) arg;
    ws_rdv_t *next_rdv   = worldsens_rdv_see_next();
    event_t  *next_event = scheduler_see_next();
    double value;

    if(next_event->priority == PRIORITY_CALLBACK && (uint8_t)(next_event->u.nodeid) == worldsens_get_wsnet_node_id(msg->node_id)) {
        WSNET_S_DBG_DBG("WSNET2:: --> TX_MEASURE: same measure found in fifo, skip sending (ip:%d)\n", c->node);
	return 0;
    }

    if(next_rdv->clock  == get_time()) { /* send measure only, no rdv */

        /* read measure */
        READ_MEASURE(c, msg->measure_id, &value);

        /* forge msg */
        union _worldsens_pkt pkt_rsp;
	pkt_rsp.measure_rsp.type        = WORLDSENS_S_MEASURE_RSP;
	pkt_rsp.measure_rsp.seq         = ws_seq++;
	pkt_rsp.measure_rsp.node_id     = msg->node_id;
	pkt_rsp.measure_rsp.measure_id  = msg->measure_id;
	pkt_rsp.measure_rsp.measure_val = value;
 	
	/* send data */
	worldsens_packet_display(&pkt_rsp);
	worldsens_packet_hton(&pkt_rsp);
	if(wsens_srv_msend((char *) &pkt_rsp, sizeof(struct _worldsens_s_measure_rsp))){
       	    return -1;  
	}
    }

    else { /* send measure and rdv */

        /* read measure */
        READ_MEASURE(c, msg->measure_id, &value);

	/* update next rp */    
	ws_nsync = next_event->clock;
	ws_nsyncseq++;
	ws_synched = 0;
	ws_rp_updated = 1;

        /* forge msg */
        union _worldsens_pkt pkt_sr_rsp;
	pkt_sr_rsp.measure_sr_rsp.type        = WORLDSENS_S_MEASURE_SR_RSP;
	pkt_sr_rsp.measure_sr_rsp.seq         = ws_seq++;
	pkt_sr_rsp.measure_sr_rsp.node_id     = msg->node_id;
	pkt_sr_rsp.measure_sr_rsp.measure_id  = msg->measure_id;
	pkt_sr_rsp.measure_sr_rsp.measure_val = value;
	pkt_sr_rsp.measure_sr_rsp.rp_next     = ws_nsyncseq;
	pkt_sr_rsp.measure_sr_rsp.rp_duration = ws_nsync - ws_csync;
	
	/* send data */
	worldsens_packet_display(&pkt_sr_rsp);
	worldsens_packet_hton(&pkt_sr_rsp);
	if(wsens_srv_msend((char *) &pkt_sr_rsp, sizeof(struct _worldsens_s_measure_sr_rsp))){
       	    return -1;  
	}
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
static void worldsens_update(void) {
    char msg[WORLDSENS_MAX_PKTLENGTH];
    int len, error;
    struct sockaddr_in address;

    union _worldsens_pkt *pkt = (union _worldsens_pkt *) msg;
    error = wsens_srv_rcv(msg, &len, &address);

    if ((error == 0) && (len == 0)) {
        return;
    }
    else if (error == -1) {
        exception = EXCEPTION_FATAL;
    }

    worldsens_packet_ntoh(pkt);
    worldsens_packet_display(pkt);

    switch (pkt->c_header.type) {
    case WORLDSENS_C_CONNECT_REQ:
        worldsens_rx_connect_req((struct _worldsens_c_connect_req *) pkt, &address);
	break;
    case WORLDSENS_C_SYNC_ACK:
        worldsens_rx_sync_ack((struct _worldsens_c_sync_ack *) pkt);
	break;
    case WORLDSENS_C_BYTE_TX:
        worldsens_tx_byte((struct _worldsens_c_byte_tx *) pkt);
	break;
    case WORLDSENS_C_MEASURE_REQ:
        worldsens_rx_measure_req((struct _worldsens_c_measure_req *) pkt);
        break;
    case WORLDSENS_C_DISCONNECT:
        worldsens_rx_disconnect((struct _worldsens_c_disconnect *) pkt);
	break;
    }   
    
    /* if a new event has been added before current time, set exception to backtrack */
    event_t *next_event = scheduler_see_next();
    if (next_event->clock < get_time()) {
        exception = EXCEPTION_BACKTRACK;
    }

    return;
}


/* ************************************************** */
/* ************************************************** */
/**
 * Print packet informations
 **/
static void worldsens_packet_display(union _worldsens_pkt *msg) {
    /* comment/uncomment next line to disable/enable packet informations display */
    //worldsens_packet_dump(msg);
}


/**
 * Print event informations
 **/
static void worldsens_event_dump(event_t *event) {
    if (ws_count) {
        switch (event->priority) {
	case PRIORITY_BIRTH:
	    WSNET_S_DBG_DBG ("WSNET2:: === EVENT BIRTH PERFORMED AT TIME %"PRId64"\n", event->clock);
	    break;
	case PRIORITY_MOBILITY:
  	    WSNET_S_DBG_DBG ("WSNET2:: === EVENT MOBILITY PERFORMED AT TIME %"PRId64"\n", event->clock);
	    break;
	case PRIORITY_RX_BEGIN:
	    WSNET_S_DBG_DBG ("WSNET2:: === EVENT RX BEGIN PERFORMED AT TIME %"PRId64"\n", event->clock);
	    break;
	case PRIORITY_RX_END:
	    WSNET_S_DBG_DBG ("WSNET2:: === EVENT RX END PERFORMED AT TIME %"PRId64"\n", event->clock);
	    break;
	case PRIORITY_TX_END:
	    WSNET_S_DBG_DBG ("WSNET2:: === EVENT TX END PERFORMED AT TIME %"PRId64"\n", event->clock);
	    break;
	case PRIORITY_CALLBACK:
            WSNET_S_DBG_DBG ("WSNET2:: === EVENT CALLBACK PERFORMED AT TIME %"PRId64"\n", event->clock);
	    break;
	case PRIORITY_MILESTONE:
 	    WSNET_S_DBG_DBG ("WSNET2:: === EVENT MILESTONE PERFORMED AT TIME %"PRId64"\n", event->clock); 
	    break;
	case PRIORITY_QUIT:
  	    WSNET_S_DBG_DBG ("WSNET2:: === EVENT QUIT PERFORMED AT TIME %"PRId64"\n", event->clock);
	    break;
	default:
	    WSNET_S_DBG_DBG ("WSNET2:: === UNKNOWN EVENT PERFORMED AT TIME %"PRId64"\n", event->clock);
	    break;
	}
    }
}
