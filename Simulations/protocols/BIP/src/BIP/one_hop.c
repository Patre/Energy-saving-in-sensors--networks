
#include "BIP/one_hop.h"



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
	
    packet_hello *hello = (packet_hello *) (packet->data + nodedata->overhead);
	
    // ajout du voisin
    if(!listeNodes_recherche(nodedata->oneHopNeighbourhood,hello->src))
        listeNodes_insert_values(&nodedata->oneHopNeighbourhood,hello->src,hello->src_pos.x, hello->src_pos.y, hello->src_pos.z);
	
	packet_dealloc(packet);
	
    return 1;
	
}


