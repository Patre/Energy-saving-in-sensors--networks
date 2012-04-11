
#include "two_hop.h"



void init_two_hop(call_t *c, double eps)
{
    struct nodedata *nodedata = get_node_private_data(c);
	
	long long int at = get_time_now() + eps*1000000 + 110000*get_node_count();
	
    uint64_t time = at + 100 + 110000*c->node;
    scheduler_add_callback(time, c, broadcast_hello2, NULL);
    uint64_t timeFinish = at+110000*get_node_count();
    scheduler_add_callback(timeFinish, c, print_two_hop_neighbourhood, NULL);
	//scheduler_add_callback(timeFinish, c, afficherGraphe, nodedata->g2hop);
}

int broadcast_hello2(call_t *c, void *args) {
	printf("broadcast hello 2 from %d at %.2lfs\n", c->node, ((double)get_time()/1000000.0));
    struct nodedata *nodedata = get_node_private_data(c);

    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node};

    //destination du paquet
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};

    //creation du paquet
    packet_t *packet = packet_create(c, nodedata->overhead + sizeof(packet_hello2), -1);
    packet_hello2 *hello = (packet_hello2 *) (packet->data + nodedata->overhead);

    //initialiser les donnees
    hello->type = HELLO2;
    hello->src = c->node;
    hello->src_pos.x=get_node_position(c->node)->x;
    hello->src_pos.y=get_node_position(c->node)->y;
    hello->src_pos.z=get_node_position(c->node)->z;
	hello->oneHopNeighbourhood = Nullptr(listeNodes);
	listeNodes_copy(&hello->oneHopNeighbourhood, nodedata->oneHopNeighbourhood);

    if (SET_HEADER(&c0, packet, &destination) == -1) {
            packet_dealloc(packet);
            return -1;
    }
	
	DEBUG
	//printf("BIP - Paquet de type %d envoye de %d a %d (at %lf s).\n", hello->type, c->node, destination.id, get_time_now_second());
    
    TX(&c0,packet);
	
    return 1;
}

int rx_two_hop(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
	struct protocoleData *entitydata = get_entity_private_data(c);
    packet_hello2 *hello = (packet_hello2*) (packet->data + nodedata->overhead);
	printf("%d recoit hello2 depuis %d at %.2lfs\n", c->node, hello->src, ((double)get_time()/1000000.0));
	
	// copier le 1-voisinage recu dans le 2-voisinage de ce noeud
	listeNodes_union(&(nodedata->twoHopNeighbourhood), hello->oneHopNeighbourhood);
	listeNodes_intersection(&(nodedata->twoHopNeighbourhood), nodedata->oneHopNeighbourhood);
	listeNodes_delete(&(nodedata->twoHopNeighbourhood), c->node);
	
	/* ajout du 2-voisinage de ce noeud dans le graphe */
	double cout, distance;
	position_t pos;
	listeNodes* tmp = hello->oneHopNeighbourhood;
	
	while(tmp != 0)
	{
		addVertex(nodedata->g2hop, hello->src);
		addVertex(nodedata->g2hop, tmp->values.node);
		pos.x = tmp->values.x;
		pos.y = tmp->values.y;
		pos.z = tmp->values.z;
		
		cout = calcul_energie(hello->src_pos, 
							  pos, 
							  entitydata->alpha, 
							  entitydata->c, 
							  &distance);
		
		addEdgeUndirected(nodedata->g2hop, hello->src, tmp->values.node, cout);
		tmp = tmp->suiv;
	}
	
    //liberer le packet
    packet_dealloc(packet);
	
    return 1;
}

void print_two_hop_neighbourhood(call_t *c)
{
    struct nodedata *nodedata = get_node_private_data(c);
	printf("\t2-voisinage de %d recupere : ", c->node);
	listeNodes_affiche(nodedata->twoHopNeighbourhood);
}


