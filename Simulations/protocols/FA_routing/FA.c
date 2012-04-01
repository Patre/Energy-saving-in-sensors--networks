/**
 *  \file   epidemic.c
 *  \brief  An epidemic data dissemination protocol
 *  \author Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdio.h>
#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */

/* Defining module informations*/
model_t model =  {
    "RIP routing protocol",
    "Chloe Desdouits",
    "0.1",
    MODELTYPE_ROUTING, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */

/* Defining packet type */
#define HELLO_PACKET 0
#define DATA_PACKET  1

#define DELAY 1000000000


/* ************************************************** */
/* ************************************************** */

struct route {
	nodeid_t nodeReachable;
	double prevCost;
};

struct entreeTable {
	nodeid_t destination;
	nodeid_t prochain;
	double cout;
};

/* Node private data */
struct nodedata {
	int nbEntrees;
	struct entreeTable* tableRoutage;
	int overhead;
	
	/* stats */
    int hello_tx;
    int hello_rx;
    int data_tx;
    int data_rx;
    int data_noroute;
    int data_forward;
	
};

/* Entity private data */
struct entitydata {
	//double p;
	//double q;
};


/* ************************************************** */
/* ************************************************** */

struct routing_header {
	int nbRoutes;
	struct route * routes;
	double energy_init;
	double energy_rest;
	
	nodeid_t dst;
    position_t dst_pos;
    nodeid_t src;
    position_t src_pos;
	nodeid_t last;
    position_t last_pos;
    int type;
};

/* ************************************************** */
/* ************************************************** */
int hello_callback(call_t *c, void *args);


/* ************************************************** */
/* ************************************************** */

/* Here we read the entity variables from the xml config file*/
int init(call_t *c, void *params) {
	return 0;
}

int destroy(call_t *c) {
    return 0;
}


/* ************************************************** */
/* ************************************************** */

/* Here we read the node variables from the xml config file*/
int setnode(call_t *c, void *params) {
	printf("[SETNODE] [node %d]", c->node);
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    int i = get_entity_links_down_nbr(c);
    param_t *param;
    
    /* default values */
	nodedata->overhead = -1;
	nodedata->tableRoutage = malloc(sizeof(struct entreeTable));
	nodedata->nbEntrees = 1;
	nodedata->tableRoutage[0].destination = c->node;
	nodedata->tableRoutage[0].prochain = c->node;
	nodedata->tableRoutage[0].cout = 0;
	
	nodedata->hello_tx = 0;
    nodedata->hello_rx = 0;
    nodedata->data_tx = 0;
    nodedata->data_rx = 0;
    nodedata->data_forward = 0;
    nodedata->data_noroute = 0;
	
    /* reading the "default" markup from the xml config file */
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
		printf("(param %s = %s) ", param->key, param->value);
    }
    
    set_node_private_data(c, nodedata);
	
	printf("\n");
    return 0;
	
error:
    free(nodedata);
    return -1;
}

int unsetnode(call_t *c) {
    struct nodedata *nodedata = get_node_private_data(c);
	printf("[UNSETNODE] Node %d\n", c->node);
	call_t cen = {-1, c->node, -1};
	double enRemPost = battery_remaining(&cen);
	printf("\tBattery remaining %.0lf\n", enRemPost);
	printf("\tStats hello_tx %d ; hello_rx %d ; data_tx %d ; data_rx %d ; data_noroute %d ; data_forward %d\n", nodedata->hello_tx, nodedata->hello_rx, nodedata->data_tx, nodedata->data_rx, nodedata->data_noroute, nodedata->data_forward);
	
	
	printf("\tRoutes :\n");
	int j;
	for(j = 0 ; j < nodedata->nbEntrees ; j++)
	{
		printf("\t\t[Noeud %d ; Next %d ; Cout %lf]\n", nodedata->tableRoutage[j].destination, nodedata->tableRoutage[j].prochain,nodedata->tableRoutage[j].cout);
	}
	
    /* we print node stats before exit */
	free(nodedata->tableRoutage);
    free(nodedata);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(call_t *c)
{
	int i;
	printf("[BOOTSTRAP node %d] ",c->node);
	
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    
    /* get mac header overhead */
    nodedata->overhead = GET_HEADER_SIZE(&c0);
	
    /*  hello packet */
	uint64_t start = get_time() + get_random_double() * DELAY;
	printf("time %u ; callback set at %u ", get_time(), start);
	scheduler_add_callback(start, c, hello_callback, NULL); // TODO ameliorer la repartition
	
	printf("\n");
    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}

int hello_callback(call_t *c, void *args)
{
	printf("[HELLO from node %d at %u] ", c->node, get_time());
	int i;
	struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};
    packet_t *packet = packet_create(c, nodedata->overhead + sizeof(struct routing_header), -1);
    struct routing_header *header = (struct routing_header*) (packet->data + nodedata->overhead);
	
    /* set routing header */
    header->dst = BROADCAST_ADDR;
    header->dst_pos.x = -1;
    header->dst_pos.y = -1;
    header->dst_pos.z = -1;
    header->src = c->node;
    header->src_pos.x = get_node_position(c->node)->x;
    header->src_pos.y = get_node_position(c->node)->y;
    header->src_pos.z = get_node_position(c->node)->z;
	header->last = c->node;
	header->last_pos = *(get_node_position(c->node));
    header->type = HELLO_PACKET;
	call_t cen = {-1,c->node,-1};
	header->energy_rest = battery_remaining(&cen);
	header->energy_init = header->energy_rest + battery_consumed(&cen);
	header->nbRoutes = nodedata->nbEntrees;
	header->routes = malloc(nodedata->nbEntrees*sizeof(struct route));
	for(i = 0 ; i < nodedata->nbEntrees ; i++)
	{
		header->routes[i].nodeReachable = nodedata->tableRoutage[i].destination;
		header->routes[i].prevCost = nodedata->tableRoutage[i].cout;
	}
	
	
    /* set mac header */
    if (SET_HEADER(&c0, packet, &destination) == -1) {
        packet_dealloc(packet);
        return -1;
    }
    
    /* send hello */
    TX(&c0, packet);
    nodedata->hello_tx++;
	
    /* check neighbors timeout  */
    //das_selective_delete(nodedata->neighbors, neighbor_timeout, (void *) c);
	
    /* schedules hello */
    //scheduler_add_callback(get_time() + nodedata->period, c, advert_callback, NULL);
	printf("\n");
    return 0;
}

double cout(position_t* pi, position_t* pj, struct routing_header *header)
{
	double eij, eresi, einiti, x1 = 1.0/5.0, x2 = 50.0/5.0, x3 = 50.0/5.0;
	
	eresi = header->energy_rest;
	einiti = header->energy_init;
	eij = distance(pi, pj); // TODO coef distance
	
	return pow(eij, x1)*pow(eresi, -x2)*pow(einiti, x3);
}

void forward(call_t *c, packet_t *packet)
{  
    struct nodedata *nodedata = get_node_private_data(c);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    struct routing_header *header = (struct routing_header *) (packet->data + nodedata->overhead);
	header->last = c->node;
	header->last_pos = *(get_node_position(c->node));
	
    destination_t destination;    
	/* set mac header */
    destination.position.x = -1;
    destination.position.y = -1;
    destination.position.z = -1;
	destination.id = -1;
	
	int i;
	for(i = 0 ; i < nodedata->nbEntrees ; i++)
	{
		if(header->dst == nodedata->tableRoutage[i].destination)
		{
			destination.id = nodedata->tableRoutage[i].prochain;
			break;
		}
	}
	
	if(destination.id == -1)
	{
		packet_dealloc(packet);
		return;
	}
	
    if (SET_HEADER(&c0, packet, &destination) == -1) {
        packet_dealloc(packet);
        return;
    }
    
    /* forwarding packet */
    nodedata->data_forward++;
    TX(&c0, packet);
}

/* ************************************************** */
/* ************************************************** */
void rx(call_t *c, packet_t *packet) {
	double coutij;
    struct nodedata *nodedata = get_node_private_data(c);
    array_t *up = get_entity_bindings_up(c);
    int i = up->size, j, k, destConnu;
    struct routing_header *header = (struct routing_header *) (packet->data + nodedata->overhead);
	printf("[RX from %d to %d ", header->last, c->node);
	if(header->type == DATA_PACKET)
		printf("DATA_PACKET] ");
	else if(header->type ==HELLO_PACKET)
		printf("HELLO_PACKET] ");
		
	call_t cen = {-1, c->node, -1};
	double enRemPost = battery_remaining(&cen);
	printf("\t[Node %d battery %.0lf] ", c->node, enRemPost);
	
	for(j = 0 ; j < header->nbRoutes ; j++)
	{
		printf("\n\troute (%d … -> %d -> … -> %d -> %d) : ", header->routes[j].nodeReachable, header->src, header->last, c->node);
		coutij = cout(&header->last_pos, get_node_position(c->node), header);
		destConnu = -1;
		for(k = 0 ; k < nodedata->nbEntrees ; k++)
		{
			if(header->routes[j].nodeReachable == nodedata->tableRoutage[k].destination)
				destConnu = k;
		}
		if(destConnu == -1)
		{
			printf("dest non connue ");
			nodedata->nbEntrees++;
			nodedata->tableRoutage = realloc(nodedata->tableRoutage, nodedata->nbEntrees*sizeof(struct entreeTable));
			nodedata->tableRoutage[nodedata->nbEntrees-1].destination = header->routes[j].nodeReachable;
			nodedata->tableRoutage[nodedata->nbEntrees-1].prochain = header->last;
			nodedata->tableRoutage[nodedata->nbEntrees-1].cout = coutij;
			
			/*if(header->type == HELLO_PACKET)
			{
				uint64_t start = get_time() + get_random_double() * DELAY;
				printf("time %u ; callback set at %u ", get_time(), start);
				scheduler_add_callback(start, c, hello_callback, NULL); // TODO ameliorer la repartition
			}*/
		}
		else
		{
			printf("dest connue ");
			switch(header->type) {
				case DATA_PACKET: 
					if(nodedata->tableRoutage[destConnu].prochain == header->last)
					{
						printf("cout maj (%.0lf -> %.0lf) ", nodedata->tableRoutage[destConnu].cout, header->routes[j].prevCost + coutij);
						nodedata->tableRoutage[destConnu].cout = header->routes[j].prevCost + coutij;
					}
					else if(nodedata->tableRoutage[destConnu].cout > header->routes[j].prevCost + coutij)
					{
						printf("next hop remplace par %d ", header->last);
						printf("cout maj (%.0lf -> %.0lf) ", nodedata->tableRoutage[destConnu].cout, header->routes[j].prevCost + coutij);
						nodedata->tableRoutage[destConnu].prochain = header->last;
						nodedata->tableRoutage[destConnu].cout = header->routes[j].prevCost + coutij;
					}
					break;
				case HELLO_PACKET: 
					nodedata->hello_rx++;
					if(nodedata->tableRoutage[destConnu].cout > header->routes[j].prevCost + coutij)
					{
						nodedata->tableRoutage[destConnu].prochain = header->last;
						nodedata->tableRoutage[destConnu].cout = header->routes[j].prevCost + coutij;
						
						uint64_t start = get_time() + get_random_double() * DELAY;
						printf("time %u ; callback set at %u ", get_time(), start);
						scheduler_add_callback(start, c, hello_callback, NULL); // TODO ameliorer la repartition
						//free(header->routes); //TODO free
						//packet_dealloc(packet);
					}
					break;
				default : 
					break; 
			}
		}
	}
	printf("\n\t");
	
	if(header->type == DATA_PACKET)
	{
		if ((header->dst != BROADCAST_ADDR) && (header->dst != c->node) ) {
			printf("forward to %d", header->dst);
			forward(c, packet);
		}
		else
		{
			nodedata->data_rx++;
			printf("couches sup ");
			while (i--)
			{
				call_t c_up = {up->elts[i], c->node, c->entity};
				packet_t *packet_up;	     
				if (i > 0) {
					packet_up = packet_clone(packet);         
				} else {
					packet_up = packet;
				}
				RX(&c_up, packet_up);
			}
			free(header->routes); // TODO free
			//packet_dealloc(packet);
		}
	}
	
	/*printf("\n\tRoutes :\n");
	for(j = 0 ; j < nodedata->nbEntrees ; j++)
	{
		printf("\t\t[Noeud %d ; Next %d ; Cout %lf]\n", nodedata->tableRoutage[j].destination, nodedata->tableRoutage[j].prochain,nodedata->tableRoutage[j].cout);
	}*/
	printf("\n");
    return;
}

void tx( call_t *c , packet_t * packet )
{
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
    TX(&c0, packet);
}

int set_header( call_t *c , packet_t * packet , destination_t * dst )
{
	//printf("[SET HEADER]\n");
	int i, voisinConnu = -1;
	struct nodedata *nodedata = get_node_private_data(c);
    destination_t destination;
    struct routing_header *header = (struct routing_header *) (packet->data + nodedata->overhead);
    call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};
	
	for(i = 0 ; i < nodedata->nbEntrees ; i++)
	{
		if(nodedata->tableRoutage[i].destination == dst->id)
			voisinConnu = i;
	}
    /* if no route, return -1 */
    if (dst->id != BROADCAST_ADDR && voisinConnu == -1) {
        nodedata->data_noroute++;
        return -1;
    }
	
    /* set routing header */
    header->dst = dst->id;
    header->dst_pos.x = dst->position.x;
    header->dst_pos.y = dst->position.y;
    header->dst_pos.z = dst->position.z;
    header->src = c->node;
    header->src_pos.x = get_node_position(c->node)->x;
    header->src_pos.y = get_node_position(c->node)->y;
    header->src_pos.z = get_node_position(c->node)->z;
    header->type = DATA_PACKET;
	call_t cen = {-1, c->node, -1};
	header->energy_rest = battery_remaining(&cen);
	header->energy_init = header->energy_rest + battery_consumed(&cen);
	header->nbRoutes = nodedata->nbEntrees;
	header->routes = malloc(nodedata->nbEntrees*sizeof(struct route));
	for(i = 0 ; i < nodedata->nbEntrees ; i++)
	{
		header->routes[i].nodeReachable = nodedata->tableRoutage[i].destination;
		header->routes[i].prevCost = nodedata->tableRoutage[i].cout;
	}
   
	
    /* Set mac header */
    destination.id = nodedata->tableRoutage[voisinConnu].prochain;
    destination.position.x = -1;
    destination.position.y = -1;
    destination.position.z = -1;
    return SET_HEADER(&c0, packet, &destination);
}

int get_header_size( call_t * c )
{
	//printf("[GET HEADER SIZE]\n");
	struct nodedata *nodedata = get_node_private_data(c);
	
    if (nodedata->overhead == -1) {
        call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};        
        nodedata->overhead = GET_HEADER_SIZE(&c0);
    }
    
    return nodedata->overhead + sizeof(struct routing_header);
}

int get_header_real_size( call_t * c )
{
	//printf("[GET HEADER REAL SIZE]\n");
	struct nodedata *nodedata = get_node_private_data(c);
	
    if (nodedata->overhead == -1) {
        call_t c0 = {get_entity_bindings_down(c)->elts[0], c->node, c->entity};        
        nodedata->overhead = GET_HEADER_REAL_SIZE(&c0);
    }
    
    return nodedata->overhead + sizeof(struct routing_header);
}


/* ************************************************** */
/* ************************************************** */
routing_methods_t methods = {rx, tx, set_header, get_header_size, get_header_real_size};
