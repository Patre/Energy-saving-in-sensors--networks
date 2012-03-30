
#include "two_hop.h"



void init_two_hop(call_t *c, double eps)
{
	long long int rand = abs(get_random_integer());
    int count=get_node_count();
	rand %= 100000000;
	rand *= c->node;
	rand += 100000000*count;
	
    uint64_t time = get_time_now() + time_seconds_to_nanos(eps) + rand;
    scheduler_add_callback(time, c, broadcast_hello2, NULL);
}

int broadcast_hello2(call_t *c, void *args) {
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
	printf("BIP - Paquet de type %d envoye de %d a %d (at %lf s).\n", hello->type, c->node, destination.id, get_time_now_second());
    
    TX(&c0,packet);

	
    return 1;
}

int rx_two_hop(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
	struct protocoleData *entitydata = get_entity_private_data(c);
    packet_hello2 *hello = (packet_hello2*) (packet->data + nodedata->overhead);
	
    //recuperer le support de communication MAC
	entityid_t *down = get_entity_links_down(c);
	call_t c0 = {down[0], c->node};
	
	// copier le 1-voisinage recu dans le 2-voisinage de ce noeud
	listeNodes_union(&(nodedata->twoHopNeighbourhood), hello->oneHopNeighbourhood);
	listeNodes_intersection(&(nodedata->twoHopNeighbourhood), nodedata->oneHopNeighbourhood);
	listeNodes_delete(&(nodedata->twoHopNeighbourhood), c->node);
	
	/* ajout du 2-voisinage de ce noeud dans le graphe */
	double cout, distance;
	position_t pos, pos2;
	listeNodes* tmp = nodedata->oneHopNeighbourhood;
	listeNodes* tmp2;
	while(tmp != 0)
	{
		pos.x = tmp->values.x;
		pos.y = tmp->values.y;
		pos.z = tmp->values.z;
		tmp2 = nodedata->twoHopNeighbourhood;
		while(tmp2 != 0)
		{
			if(tmp->values.node != tmp2->values.node)
			{
				addVertex(nodedata->g2hop, tmp2->values.node);
				pos2.x = tmp2->values.x;
				pos2.y = tmp2->values.y;
				pos2.z = tmp2->values.z;
				
				cout = calcul_energie(pos, 
									  pos2, 
									  entitydata->alpha, 
									  entitydata->c, 
									  &distance);
				if(distance < entitydata->range)
					addEdgeUndirected(nodedata->g2hop, tmp->values.node, tmp2->values.node, cout);
			}
			tmp2 = tmp2->suiv;
		}
		
		tmp = tmp->suiv;
	}
	
    //liberer le packet
    packet_dealloc(packet);
	
    return 1;
}


