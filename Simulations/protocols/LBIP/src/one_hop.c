
#include "one_hop.h"



void init_one_hop(call_t *c, double eps)
{
	long int rand = abs(get_random_integer());
	rand %= 100000000;
	uint64_t at = get_time_now() + time_seconds_to_nanos(eps) + rand*c->node;
    scheduler_add_callback(at, c, broadcast_hello, NULL);
	
	init_two_hop(c, eps);
}

int broadcast_hello(call_t *c, void *args) {
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
    printf("BIP - Paquet de type %d envoye de %d a %d (at %lf s).\n", hello->type, c->node, destination.id, get_time_now_second());
    
    TX(&c0,packet);
	
    return 0;
}


int rx_hello(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
	struct protocoleData *entitydata = get_entity_private_data(c);
	
    packet_hello *hello = (packet_hello *) (packet->data + nodedata->overhead);
	
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
	
	
	
	// ajout des liens entre le nouveau voisin et le reste du 1-voisinage
	listeNodes* tmp = nodedata->oneHopNeighbourhood;
	position_t pos;
	while(tmp != 0)
	{
		if(tmp->values.node != hello->src)
		{
			pos.x = tmp->values.x;
			pos.y = tmp->values.y;
			pos.z = tmp->values.z;
			
			cout = calcul_energie(pos, 
								  hello->src_pos, 
								  entitydata->alpha, 
								  entitydata->c, 
								  &distance);
			
			if(distance < entitydata->range)
				addEdgeUndirected(nodedata->g2hop, tmp->values.node, hello->src, cout);
		}
		tmp = tmp->suiv;
	}
	
	
	packet_dealloc(packet);
	
    return 1;
	
}


