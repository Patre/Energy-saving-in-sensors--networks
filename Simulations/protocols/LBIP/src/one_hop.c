
#include "one_hop.h"



void init_one_hop(call_t *c, double eps)
{
	//long int rand = abs(get_random_integer());
	//rand %= 100000000;
	uint64_t at = get_time() + eps*1000000 + 110000*c->node;
    scheduler_add_callback(at, c, broadcast_hello, NULL);
	
	uint64_t timeFinish = get_time_now() + eps*1000000 + 110000*get_node_count();
	scheduler_add_callback(timeFinish, c, print_one_hop_neighbourhood, NULL);
	init_two_hop(c, eps);
}

int broadcast_hello(call_t *c, void *args) {
	//printf("broadcast hello from %d at %.2lfs\n", c->node, ((double)get_time()/1000000.0));
    struct nodedata *nodedata = get_node_private_data(c);
	
    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};
	
    //destination de paquet
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};
	
    //creation de paquet et initialisation de son data
    packet_t *packet = packet_create(c, nodedata->overhead + sizeof(packet_hello), -1);
    packet_hello *hello = (packet_hello *) (packet->data + nodedata->overhead);
	
    //initialiser les donnees
    hello->type=HELLO;
    hello->src=c->node;
    hello->src_pos.x=get_node_position(c->node)->x;
    hello->src_pos.y=get_node_position(c->node)->y;
    hello->src_pos.z=get_node_position(c->node)->z;
	
    if (SET_HEADER(&c0, packet, &destination) == -1) {
		packet_dealloc(packet);
		return -1;
    }
	
    DEBUG;
    //printf("BIP - Paquet de type %d envoye de %d a %d (at %lf s).\n", hello->type, c->node, destination.id, get_time_now_second());
    
    TX(&c0,packet);
	
    return 0;
}


int rx_hello(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
	struct protocoleData *entitydata = get_entity_private_data(c);
	
    packet_hello *hello = (packet_hello *) (packet->data + nodedata->overhead);
	//printf("%d recoit hello depuis %d at %.2lfs\n", c->node, hello->src, ((double)get_time()/1000000.0));
    // ajout du voisin dans la liste du 1-voisinage
    if(!listeNodes_recherche(nodedata->oneHopNeighbourhood,hello->src))
        listeNodes_insert_values(&nodedata->oneHopNeighbourhood,hello->src,hello->src_pos.x, hello->src_pos.y, hello->src_pos.z);
	
	// ajout de l'arete dans le graphe
	double distance, cout;
	addVertex(nodedata->g2hop, hello->src);
	cout = calcul_energie(*get_node_position(c->node), 
						  hello->src_pos, 
						  entitydata->alpha, 
						  entitydata->c, &distance);
	addEdgeUndirected(nodedata->g2hop, c->node, hello->src, cout);
	
	
	packet_dealloc(packet);
	
    return 1;
	
}

void print_one_hop_neighbourhood(call_t *c)
{
    struct nodedata *nodedata = get_node_private_data(c);
	printf("\t1-voisinage de %d recupere : ", c->node);
	listeNodes_affiche(nodedata->oneHopNeighbourhood);
}


