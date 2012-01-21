/**
 *  \file   scheduler.c
 *  \brief  Event scheduler
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "../../internals.h"


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

#define SCHEDULER_MILESTONE_PERIOD 20000


/* ************************************************** */
/* ************************************************** */
typedef enum {
    NO_EXCEPTION,
    EXCEPTION_QUIT
} EXCEPTION;


/* ************************************************** */
/* ************************************************** */
static void *scheduler = NULL;             /* events                     */
static EXCEPTION exception = NO_EXCEPTION; /* current exception          */
static int event_id = 0;                   /* unique event id            */
static uint64_t g_clock = 0;               /* global simulation time     */
static uint64_t g_end = 0;                 /* global simulation end time */
static void *mem_event = NULL;             /* memory slice for events    */
static struct _debug {
    int c_events;
    int d_events; 
    uint64_t o_unanotime; 
    uint64_t o_clock; 
} dbg = {0, 0, 0, 0};

/* ************************************************** */
/* ************************************************** */
event_t *scheduler_next(void);
void scheduler_add_event(event_t *event);
void scheduler_add_birth(uint64_t clock, nodeid_t id);
void scheduler_add_quit(uint64_t clock);
void scheduler_add_milestone(uint64_t clock);
void scheduler_backtrack(void);
void do_observe(void);
void do_run(void);
void do_event(event_t *event);
void do_clockadvance(uint64_t clock);
void scheduler_stats(void);
void dump_stats(void);


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

    return 0;
}

void scheduler_clean(void) {
    event_t *event;
    
    while ((event = (event_t *) sodas_pop(scheduler)) != NULL) {
        switch (event->priority) {
        case PRIORITY_RX_END:
            packet_dealloc(event->u.rx.packet);
            break;
        default:
            break;
        }

        mem_fs_dealloc(mem_event, event);
    }

    sodas_destroy(scheduler);
}

int scheduler_bootstrap(void) {

    /* set first milestone */
    scheduler_add_milestone(SCHEDULER_MILESTONE_PERIOD);
    
    /* set end event */
    if (g_end) {
        scheduler_add_quit(g_end);
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
        node_t *node = nodes + i;
        if ((node->state == NODE_DEAD) || (node->state == NODE_UNDEF)) {
            continue;
        }
        for (j = 0; j < bundles[node->bundle].entity_cnt; j++) {
            entity_t *entity = get_entity_by_id(bundles[node->bundle].entity[j]);
            call_t c = {entity->id, node->id, -1};
            if ((entity)->unsetnode) {
                (entity)->unsetnode(&c);
            }
        }
    }

    /* unsetnode */
    i = entity_cnt;
    while (i--) {
        entity_t *entity = entities + i;
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
        }
    }
}

void do_run(void) {
    event_t *event = NULL;

    while (1) {
        
        /* get next event */
        if ((event = scheduler_next()) == NULL) {
            fprintf(stderr, "scheduler: null event (do_event())\n");
            exception = EXCEPTION_QUIT;
            break;
        }
        
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
    }
}

void do_event(event_t *event) {
    switch (event->priority) {
    case PRIORITY_BIRTH:
        node_birth(event->u.nodeid);
        break;
    case PRIORITY_MOBILITY:
        mobility_event(g_clock);
        break;
    case PRIORITY_RX_BEGIN:
        medium_cs(event->u.rx.packet, &(event->u.rx.call));
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
        scheduler_add_milestone(g_clock + SCHEDULER_MILESTONE_PERIOD);
       break;
    case PRIORITY_QUIT:
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
}

event_t *scheduler_add_callback(uint64_t clock, call_t *c, callback_t callback, void *arg) {
    event_t *event;

    if ((scheduler_get_end()) && (clock > scheduler_get_end())) {
      return NULL;
    }
    
    if ((event = (event_t *) mem_fs_alloc(mem_event)) == NULL) {
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
    return;
}

void scheduler_add_tx_end(uint64_t clock, call_t *c, packet_t *packet) {
    event_t *event;

    if ((event = (event_t *) mem_fs_alloc(mem_event)) == NULL) {
        return;
    }
    event->clock = clock;
	event->u.rx.packet = packet;
	event->u.rx.call.entity = c->entity;
	event->u.rx.call.node = c->node;
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


    /*fprintf(stderr, "\nSimulation stats:\n  simulated time: %"PRId64"\n  simulation time: %"PRId64"\n  speedup: %lf\n  instantaneous speedup: %lf\n  events in queue: %d\n  events executed: %d\n", 
            g_clock,
            unanotime,
            speedup,
              i_speedup,
            dbg.c_events, 
            dbg.d_events);*/
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
void worldsens_set_count(int count) {
}
