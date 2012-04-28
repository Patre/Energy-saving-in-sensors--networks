/**
 *  \file   theorie.c
 *  \brief  Theorique consumation of energie
 *  \author LAOUADI Rabah
 *  \date   2012
 **/
#include <include/modelutils.h>
#include <time_wsnet.h>
#include <list.h>

//#define ENERGY(x...)  { FILE *energ; energ=fopen("energy","a+"); fprintf(energ,x); fclose(energ);}
//#define PR(x...)  { FILE *energ; energ=fopen("lifetime","a+"); fprintf(energ,x); fclose(energ);}



void init_files()
{
    //REPLAY
    /*FILE *replay;
    replay=fopen("energy","w");
    fclose(replay);*/

    //POURCENTAGE des NEUD VIVANT
    /*FILE *por;
    por=fopen("lifetime","w");
    fclose(por);*/
	
	FILE* lt = fopen("lifetime", "w");
	fclose(lt);
}
/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Theorique consumation of energie",
    "LAOUADI Rabah",
    "0.1",
    MODELTYPE_ENERGY, 
    {NULL, 0}
};

/* Entity private data */
struct entitydata {
    int debug;
    double prMax;
	int nbDead;
	list* articulations;
	double range;
	int alpha;
	int c;
	int LC;
};

struct nodedata {
    double energy;
    double initial;
    double tx;
    double rx;
    double idle;
	
};


void getArticulationNodes(call_t* c)
{
    struct entitydata *entitydata = get_entity_private_data(c);
	
	FILE* grapheJava = fopen("grapheForJava", "w");
	int nbNoeuds = get_node_count(), i, j, aretes = 0;
	
	fprintf(grapheJava, "%d\n", nbNoeuds);
	for(i = 0 ; i < nbNoeuds ; i++)
	{
		if(!is_node_alive(i))
			continue;
		for(j = 0 ; j < nbNoeuds ; j++)
		{
			if(i != j && is_node_alive(j) && distance(get_node_position(i), get_node_position(j)) < entitydata->range)
			{
				aretes++;
				fprintf(grapheJava, "%d %d\n", i, j);
			}
		}
	}
	fclose(grapheJava);
	if(aretes != 0)
	{
		char javapath[1024];
		if(getenv("JAVAPATH") != NULL)
			strcpy(javapath, getenv("JAVAPATH"));
		else
			strcpy(javapath, "../ArticPointDFS");
		
		char add[1024];
		strcpy(add, "java -cp ");
		strcat(add, javapath);
		strcat(add, " ArticPointDFS grapheForJava\n");
		system(add);
		
		if(entitydata->articulations != 0)
			list_detruire(&entitydata->articulations);
		entitydata->articulations = 0;
		
		int aPoint, num;
		grapheJava = fopen("grapheForJava", "r");
		fscanf(grapheJava, "%d", &num);
		for(i = 0 ; i < num ; i++)
		{
			fscanf(grapheJava, "%d", &aPoint);
			//printf("\tajout dans liste : %d\n", aPoint);
			list_insert(&entitydata->articulations, aPoint);
		}
		fclose(grapheJava);
	}
	system("rm grapheForJava\n");
}

/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {


    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;

    /* init entity variables */
    entitydata->debug = 0;
    entitydata->prMax = 75.00;
	entitydata->nbDead = 0;
	entitydata->articulations = 0;
	entitydata->range = 30.0;
	entitydata->alpha = 1;
	entitydata->c = 0;
	entitydata->LC = 0;
	

    /* reading the "init" markup from the xml config file */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "debug")) {
            if (get_param_integer(param->value, &(entitydata->debug))) {
                goto error;
            }
        }
		if (!strcmp(param->key, "range")) {
            if (get_param_double(param->value, &(entitydata->range))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "PRNmin")) {
            if (get_param_double(param->value, &(entitydata->prMax))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "alpha")) {
            if (get_param_integer(param->value, &(entitydata->alpha))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "c")) {
            if (get_param_integer(param->value, &(entitydata->c))) {
                goto error;
            }
        }
    }

    init_files();
    set_entity_private_data(c, entitydata);
	printf("energie : alpha : %d ; c : %d\n", entitydata->alpha, entitydata->c);
	
	
    return 0;

error:
    free(entitydata);
    return -1;
}

int destroy(call_t *c) {
    return 0;
}




/* ************************************************** */
/* ************************************************** */
int setnode(call_t *c, void *params) {
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    param_t *param;

    /* default values */
    nodedata->energy  = 1000000.0;
    nodedata->initial = 1000000.0;
    nodedata->tx      = 2;
    nodedata->rx      = 2;
    nodedata->idle    = 1;

   /* get parameters */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "energy")) {
            if (get_param_double(param->value, &(nodedata->energy))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "tx")) {
            if (get_param_double(param->value, &(nodedata->tx))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "rx")) {
            if (get_param_double(param->value, &(nodedata->rx))) {
                goto error;
            }
        }
        if (!strcmp(param->key, "idle")) {
            if (get_param_double(param->value, &(nodedata->idle))) {
                goto error;
            }
        }
    }
    
    nodedata->initial = nodedata->energy;
    set_node_private_data(c, nodedata);
	printf("Energie initiale : %.1lf\n",nodedata->energy);
    return 0;
    
 error:
    free(nodedata);
    return -1;
}

int unsetnode(call_t *c) {
    struct entitydata *entitydata =get_entity_private_data(c);
	if(entitydata->articulations != 0)
		list_detruire(&entitydata->articulations);
    free(get_node_private_data(c));
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(call_t *c)
{
	
    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void consume_tx(call_t *c, uint64_t duration, double txdBm) {
    struct nodedata *nodedata = get_node_private_data(c);
    nodedata->energy -= duration * nodedata->tx;

    if (nodedata->energy <= 0) {
        nodedata->energy = 0;
        //printf("%d est Mort a %lf\n",c->node,get_time_now_second());
        //end_simulation();
        node_kill(c->node);
    }
    return;
}

void consume_rx(call_t *c, uint64_t duration) {
    struct nodedata *nodedata = get_node_private_data(c);
    nodedata->energy -= duration * nodedata->rx/10;

    if (nodedata->energy <= 0) {
        nodedata->energy = 0;
        //printf("%d est Mort a %lf\n",c->node,get_time_now_second());
        //end_simulation();
        node_kill(c->node);
    }
    return;
}

void consume_idle(call_t *c, uint64_t duration) {
    struct nodedata *nodedata = get_node_private_data(c);
    nodedata->energy -= duration * nodedata->idle; 
    if (nodedata->energy <= 0) {
        //printf("%d est Mort a %lf\n",c->node,get_time_now_second());
        //end_simulation();
        nodedata->energy = 0;
        node_kill(c->node);
    }
    return;
}

void consume(call_t *c, double energy) {
    if(!is_node_alive(c->node))
        return;


    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydata *entitydata =get_entity_private_data(c);

    nodedata->energy -= energy; 
    //ENERGY("%d R %lf %lf\n",c->node,get_time_now_second(),nodedata->energy);

    //if(entitydata->debug)
        printf("CONSUME (%d): consomme %lf, reste %lf\n",c->node,energy,nodedata->energy);

	
	if (nodedata->energy <= 0)
	{
		printf("Node %d kill\n", c->node);
		node_kill(c->node);
		return;
	}
	
    if (nodedata->energy < pow(entitydata->range, entitydata->alpha)+entitydata->c) {
		//if(entitydata->debug)
			printf("%d est Mort a %.1lf\n",c->node,get_time_now_second());
        nodedata->energy = 0;
		
		// Debut calcul lifetime
		double pourcentageAvant = ((double)entitydata->nbDead*100.0/(double) get_node_count());
		
		if(entitydata->nbDead == 0)
		{
			printf("Mesure de TTFF effectuee : %.1lf\n", get_time_now_second());
			FILE* lt = fopen("lifetime", "a");
			fprintf(lt, "TTFF %.1lf\n", get_time_now_second());
			fclose(lt);
		}
		entitydata->nbDead++;
		
		double pourcentageApres = ((double)entitydata->nbDead*100.0/(double) get_node_count());
		if(!entitydata->LC)
		{
			getArticulationNodes(c);
			if(list_recherche(entitydata->articulations, c->node))
			{
				printf("Mesure de LC effectuee : %.1lf\n", get_time_now_second());
				FILE* lt = fopen("lifetime", "a");
				fprintf(lt, "LC %.1lf\n", get_time_now_second());
				entitydata->LC = 1;
				
				
				if(pourcentageAvant <= (100-entitydata->prMax))
				{
					printf("Mesure de PCN effectuee : %.1lf\n", get_time_now_second());
					fprintf(lt, "PCN %.1lf\n", get_time_now_second());
					fclose(lt);
					//if(entitydata->debug)
						printf("Toutes les mesures ont ete effectuees...\n");
					end_simulation();
					return;
				}
				fclose(lt);
			}
		}
		if(pourcentageApres > (100-entitydata->prMax) && pourcentageAvant <= (100-entitydata->prMax))
		{
			printf("Mesure de PCN effectuee : %.1lf\n", get_time_now_second());
			FILE* lt = fopen("lifetime", "a");
			fprintf(lt, "PCN %.1lf\n", get_time_now_second());
			fclose(lt);
		}
		
		
		// Fin calcul lifetime
		
		
		if(pourcentageApres > (100-entitydata->prMax)
		   &&
		   entitydata->nbDead > 0
		   &&
		   entitydata->LC == 1)
		{
			//if(entitydata->debug)
				printf("Toutes les mesures ont ete effectuees...\n");
			end_simulation();
		}
		else if(entitydata->nbDead == get_node_count())
		{
			//if(entitydata->debug)
				printf("Plus aucun noeud en vie...\n");
			end_simulation();
		}
    }
    return;
}



double energy_consumed(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    return nodedata->initial - nodedata->energy;
}

double energy_remaining(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    return nodedata->energy;
}

double energy_status(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
    double status = nodedata->energy / nodedata->initial;
    if (nodedata->energy <= 0) {
        return 0;
    } else if ((status >= 0) && (status <= 1)) {
        return status;
    } else {
        return 0;
    }
}


/* ************************************************** */
/* ************************************************** */
energy_methods_t methods = {consume_tx, 
                            consume_rx,
                            consume_idle,
                            consume,
                            energy_consumed,
                            energy_remaining,
                            energy_status};
