


#include "structure/one_hop.h"
/***********************************************************************************************/
/***********************************************************************************************/
// LANCEMENT DE L'APPLICATION

void get_one_hop(call_t *c, double eps)
{
    uint64_t at=c->node*time_seconds_to_nanos(eps);
    scheduler_add_callback(at, c, init_one_hop, NULL);
}

/***********************************************************************************************/
/***********************************************************************************************/
// ENVOI DE PACKET HELLO
int init_one_hop(call_t *c, void *args) {
    struct nodedata *nodedata = get_node_private_data(c);
	
    //recuperer le support de communication DOWN
    entityid_t *down = get_entity_links_down(c);
    call_t c0 = {down[0], c->node, c->entity};
	
    //destination de paquet
    destination_t destination = {BROADCAST_ADDR, {-1, -1, -1}};
	
    //creation de paquet et initialisation de son data
    packet_t *packet = packet_create(c, nodedata->overhead + sizeof(packet_PROTOCOLE), -1);
    packet_PROTOCOLE *hello = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
	
    //initilailser les données
    hello->type=HELLO;
    hello->src=c->node;
    hello->src_pos.x=get_node_position(c->node)->x;
    hello->src_pos.y=get_node_position(c->node)->y;
    hello->src_pos.z=get_node_position(c->node)->z;
	hello->dst = -1;
	
    if (SET_HEADER(&c0, packet, &destination) == -1) {
		packet_dealloc(packet);
		return -1;
    }
	
    DEBUG;
    /*printf("Node %d (%lf %lf %lf) broadcast a packet hello, at %lf\n", c->node,                                 //id de Noeud
	 get_node_position(c->node)->x,get_node_position(c->node)->y,get_node_position(c->node)->z,           //la position x, y,z de Noeud
	 get_time_now_second());//*/                                                                              //l'instant d'envoi.
	
	printf("BIP - Paquet de type %d envoye de %d a %d.\n", hello->type, c->node, destination.id);
    //L'envoi
    TX(&c0,packet);
    //tous c'est bien passé
    return 1;
}



/***********************************************************************************************/
/***********************************************************************************************/
//RECEPTION et REPONDRE AU PACKET HELLO
int rx_one_hop(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
	
    //RECEPTION  DE PACKET HELLO
    packet_PROTOCOLE *hello = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
	
    DEBUG;
    /*printf("NOde %d a recu un HELLO de %d (%lf %lf %lf) at %lf\n",c->node,
	 hello->source,
	 get_node_position(hello->source)->x,get_node_position(hello->source)->y,get_node_position(hello->source)->z,
	 get_time_now_second());//*/
	
	
    //l'ajoute de voisin
    if(!listeNodes_recherche(nodedata->N1,hello->src))
        listeNodes_insert_values(&nodedata->N1,hello->src,hello->src_pos.x, hello->src_pos.y, hello->src_pos.z);
	
	
    //REPONSE DE PAKET HELLO
	
    //recuperer le support de communication MAC
	entityid_t *down = get_entity_links_down(c);
	call_t c0 = {down[0], c->node};
	
	//destination de paquet
	destination_t destination = {hello->src, {hello->src_pos.x, hello->src_pos.y, hello->src_pos.z}};
	
	//creation de paquet et initialisation de son data
	packet_t *rpacket = packet_create(c, nodedata->overhead + sizeof(packet_PROTOCOLE), -1);
	packet_PROTOCOLE* rhello = (packet_PROTOCOLE *) (rpacket->data + nodedata->overhead);
	
	//initilailser les données
	rhello->type     =   REP_HELLO;
	rhello->src   =   c->node;
	rhello->src_pos.x=get_node_position(c->node)->x;
	rhello->src_pos.y=get_node_position(c->node)->y;
	rhello->src_pos.z=get_node_position(c->node)->z;
	rhello->dst = destination.id;
	
	if (SET_HEADER(&c0, rpacket, &destination) == -1) {
		packet_dealloc(rpacket);
		return -1;
	}
	
	DEBUG;
	/*printf("Node %d (%lf %lf %lf) repond a %d  packet hello, at %lf\n", c->node,                               //id de Noeud de noeud encours
	 get_node_position(c->node)->x,get_node_position(c->node)->y,get_node_position(c->node)->z,           //la position x, y,z de Noeud
	 hello->source,                                                                                       //id de noued de destination
	 get_time_now_second()); //*/                                                                             //l'instant d'envoi.
	
	printf("BIP - Paquet de type %d envoye de %d a %d.\n", rhello->type, c->node, rhello->dst);
	//L'envoi
	TX(&c0,rpacket);
	
    //liberer le packet
    packet_dealloc(packet);
	
    //tous c'est bien passé
    return 1;
	
}


/***********************************************************************************************/
/***********************************************************************************************/
//RECEPTION DE LA REPONSE DE PAKET HELLO
void rx_one_hop_reponse(call_t *c, packet_t *packet) {
    struct nodedata *nodedata = get_node_private_data(c);
	
	packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
	
	
    DEBUG;
    /*printf("je suis le noeud %d j'ai recu la REPONSE de %d \n",c->node,
	 data->source);//*/
	
    //l'ajoute de voisin
    if(!listeNodes_recherche(nodedata->N1,data->src))
        listeNodes_insert_values(&nodedata->N1,data->src,data->src_pos.x, data->src_pos.y,data->src_pos.z);
	
    //tous c'est bien passé
    return;
}

