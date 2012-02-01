/**
 *  \file   collection_ctrl.c
 *  \brief  Control the data collection schemes of the nodes
 *  \author Romain KUNTZ
 *  \date   2009
 **/

#include <stdio.h>
#include <include/modelutils.h>
#include "../application/dynamic.h"

/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "collection control module",
    "Romain Kuntz",
    "0.1",
    MODELTYPE_ENVIRONMENT, 
    {NULL, 0}
};

/* ************************************************** */
/* ************************************************** */
struct _env_data {
    /* List of events */
    void *events;
};

/* An event is defined by the time at which it happens, 
 * the node id where it happens, the type of the event, 
 * and the value of the event */
struct _collec_event {
    uint64_t time;
    nodeid_t node_id;
    int event_type;
    int event_value;
};

/* ************************************************** */
/* ************************************************** */
int event_callback(call_t *c, void *data);

/* ************************************************** */
/* ************************************************** */
// Debug macro
#ifdef LOG_ENVIRONMENT
#define DBG(arg...)  \
do { \
    fprintf(stderr, "%s: ", __FUNCTION__); \
    fprintf(stderr, arg); \
} while (0)
#else
#define DBG(...)
#endif /* LOG_ENVIRONMENT */

/* ************************************************** */
/* ************************************************** */
int event_compare(void *key0, void *key1) {
    uint64_t *time0 = (uint64_t *) key0;
    uint64_t *time1 = (uint64_t *) key1;

    if (key0 == NULL)
        return 1;

    if (*time0 < *time1)
        return 1;

    if (*time0 > *time1)
        return -1;

    return 0;
}

/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct _env_data *entitydata = malloc(sizeof(struct _env_data));
    char *filepath = "collection_ctrl.data";
    int line;
    struct _collec_event *event;
    param_t *param;
    char str[128];
    char event_str[30];
    FILE *file;

    /* We use a sorted list for the events (sorted by time) */
    entitydata->events = sodas_create(event_compare);

    /* Get parameters from the configuration file */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "file")) {
            filepath = param->value;            
        }
    }

    /* Open the data collection scheme file */
    if ((file = fopen(filepath, "r")) == NULL) {
        DBG("ERROR: cannot open file %s in init()\n", filepath);
        goto error;
    }

    /* Save the events in a data structure */
    /* Structure of the file is:
     * <time> <node id> <event type> <value>
     */
    fseek(file, 0L, SEEK_SET);
    line = 1;
    while (fgets(str, 128, file) != NULL) {
        event = malloc(sizeof(struct _collec_event));
        memset(event_str, 0, sizeof(event_str));

        if (event == NULL) {
            DBG("ERROR: malloc failed\n");
            goto error;
        }

        if (str[0] == '#' || str[0] == '\n') {
            /* Line is a comment or an empty line */
            line++;
            continue;
        }

        if (sscanf(str, "%"PRId64" %d %s %d\n", 
                   &event->time, &event->node_id,
                   event_str, &event->event_value) != 4) {
            DBG("ERROR: cannot read event in file %s, line %d\n", 
                    filepath, line);
            free(event);
            goto error;
        }

        if (!strncmp(event_str, "APP_TIME_DRV", sizeof(event_str))) {
            event->event_type = APP_TIME_DRV;
        } else if (!strncmp(event_str, "APP_EVENT_DRV", sizeof(event_str))) {
            event->event_type = APP_EVENT_DRV;
        } else if (!strncmp(event_str, "APP_QUERY_DRV", sizeof(event_str))) {
            event->event_type = APP_QUERY_DRV;
        } else if (!strncmp(event_str, "APP_PAYLOAD_SIZE", sizeof(event_str))) {
            event->event_type = APP_PAYLOAD_SIZE;
        } else if (!strncmp(event_str, "ENV_EVENT", sizeof(event_str))) {
            event->event_type = ENV_EVENT;
        } else if (!strncmp(event_str, "QUERY_MSG", sizeof(event_str))) {
            event->event_type = QUERY_MSG;
        } else if (!strncmp(event_str, "CHANGE_APP_DST", sizeof(event_str))) {
            event->event_type = CHANGE_APP_DST;
        } else {
            DBG("ERROR: event type %s unknown in file %s, line %d\n", 
                    event_str, filepath, line);
            free(event);
            goto error;
        }
            
        //DBG("EVENT: %"PRId64" %d %d %d\n", 
        //    event->time, event->node_id, 
        //    event->event_type, event->event_value);

        if (event->node_id >= get_node_count()) {
            DBG("ERROR: node id %d (line %d) does not exist, "
                "skipping this event \n", event->node_id, line);
            free(event);
        } else {
            /* Insert the event in the sorted data structure */
            sodas_insert(entitydata->events, &event->time, event);
        }

        line++;
    }

    /* Close the opened file */
    fclose(file);

    set_entity_private_data(c, entitydata);
    return 0;

error:
    free(entitydata);
    return -1;
}

int destroy(call_t *c) {
    struct _env_data *entitydata = get_entity_private_data(c);
    struct _collec_event *event;

    /* Free the pending events */
    while ((event = (struct _collec_event *)
                sodas_pop(entitydata->events)) != NULL) {
        free(event);
    }

    /* Free the event list */
    sodas_destroy(entitydata->events);

    free(entitydata);
    return 0;
}

/* ************************************************** */
/* ************************************************** */
int bootstrap(call_t *c) {
    struct _env_data *entitydata = get_entity_private_data(c);
    struct _collec_event *event = NULL;

    /* Get the first event */
    event = (struct _collec_event *) sodas_pop(entitydata->events);

    if (event == NULL) {
        DBG("ERROR: no events in queue\n");
        return 0;
    }

    /* Schedule the first event */
    scheduler_add_callback(event->time, c, event_callback, event);

    return 0;
}

/* ************************************************** */
/* ************************************************** */
int event_callback(call_t *c, void *data) {
    struct _env_data *entitydata = get_entity_private_data(c);    
    struct _collec_event *event = (struct _collec_event *) data;
    call_t c0;
    array_t *app_layer = NULL;
    int type, value;

    /* Execute the event using an IOCTL */
    type = event->event_type;
    value = event->event_value;
    c0.node = event->node_id;
    app_layer = get_application_entities(&c0);
    c0.entity = app_layer->elts[0];
    IOCTL(&c0, type, &value, 0);
    DBG("Time %"PRId64", IOCTL for node %d, type %d, value %d\n", 
         event->time, event->node_id, type, value);

    /* Delete the event */
    free(event);

    /* Schedule the next event */
    event = (struct _collec_event *) sodas_pop(entitydata->events);
    if (event == NULL) {
        DBG("No more events in queue\n");
        return 0;
    }

    scheduler_add_callback(event->time, c, event_callback, event);
    return 0;
}

/* ************************************************** */
/* ************************************************** */
int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void read_measure(call_t *c, measureid_t measure, double *value) {
    /* The read_measure() function is not used in this model */
    return;
}
/* ************************************************** */
environment_methods_t methods = {read_measure};
