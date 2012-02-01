/**
 *  \file   filestatic.c
 *  \brief  Link success probability from file
 *  \author Guillaume Chelius
 *  \date   2007
 **/
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Link success probability extracted from file",
    "Guillaume Chelius",
    "0.1",
    MODELTYPE_PROPAGATION, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct entitydata {
    double *success;
    int node_cnt;
};


/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;
    char *filepath = NULL;
    int src, dst;
    double proba;
    FILE *file;
    char str[128];

    /* default values */
    filepath = "propagation.data";

    /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "file")) {
            filepath = param->value;
        }
    }

    /* open file */
    if ((file = fopen(filepath, "r")) == NULL) {
        fprintf(stderr, "filestatic: can not open file %s in init()\n", filepath);
        goto error;
    }

    /* extract link success probability */
    entitydata->node_cnt = get_node_count();
    entitydata->success = malloc(sizeof(double) * entitydata->node_cnt *entitydata->node_cnt);
    for (src = 0; src < entitydata->node_cnt; src++) {
        for (dst = 0; dst < entitydata->node_cnt; dst++) {
            
            *(entitydata->success + (src * entitydata->node_cnt) + dst) = MIN_DBM;
        }
    }
    fseek(file, 0L, SEEK_SET);
    while (fgets(str, 128, file) != NULL) {
        sscanf(str, "%d %d %lf\n",  &src, &dst, &proba);
        *(entitydata->success + (src * entitydata->node_cnt) + dst) = proba;
    }

    fclose(file);
    set_entity_private_data(c, entitydata);
    return 0;

 error:
    free(entitydata);
    return -1;
}

int destroy(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);
    free(entitydata->success);
    free(entitydata);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
double propagation(call_t *c, packet_t *packet, nodeid_t src, nodeid_t dst, double rxdBm) {
    struct entitydata *entitydata = get_entity_private_data(c);
    double success = *(entitydata->success + (src * entitydata->node_cnt) + dst);

    if (success == 1) {
        return rxdBm;
    } else if (success == 0) {
        return MIN_DBM;
    } else if (get_random_double() <= success) {
        return rxdBm;        
    } else {
        return MIN_DBM;
    }
}


/* ************************************************** */
/* ************************************************** */
propagation_methods_t methods = {propagation};
