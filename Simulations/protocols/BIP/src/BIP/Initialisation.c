
#include "BIP/Initialisation.h"

/* ************************************************** */
/* ************************************************** */
//INIT LBIP
/*void get_PROTOCOLE_init(call_t *c, double eps)
{
    int count=get_node_count()*2;
    uint64_t fin_two_hop=count*time_seconds_to_nanos(eps)+c->node*time_seconds_to_nanos(eps);
    uint64_t at=fin_two_hop;
    scheduler_add_callback(at, c, init_bip_tree, NULL);
}*/


/* ************************************************** */
/* ************************************************** */
//initailisation de l'arbre de LBIP
/*void init_bip_tree(call_t *c, void *args)
{
    struct nodedata *nodedata = get_node_private_data(c);
	
	
    //RECUPERER L4ENSEMBLE DE GRAPH
    int i=0;
    call_t *inter=malloc(sizeof(call_t));
    inter->entity=c->entity;
    inter->from=c->from;
	
    for(i=0;i<get_node_count();i++)
    {
        inter->node=i;
        struct nodedata *interdata=get_node_private_data(inter);
        list2N_insert_values(&nodedata->NodesV1,inter->node,get_node_position(inter->node)->x,
                             get_node_position(inter->node)->y, get_node_position(inter->node)->z,interdata->N1);
    }
	
    //REcuperer le graph G
    list *graphElements=Nullptr(list);
    for(i=0;i<get_node_count();i++) list_insert(&graphElements,i);
    list_delete(&graphElements,c->node);
	
    //list de commencement
    list *debut=Nullptr(list);
    list_insert(&debut,c->node);
	
    //recuperer tout les connexion dans le graph
    listC *connexions=Nullptr(listC);
    list2N_to_listC(&connexions,nodedata->NodesV1);
	
    listC *poi=connexions;
    while (poi)
    {
        //on ai dans le bip, nous avons le droit de fair Áa
        list_set_poids(connexions,poi->node1,poi->node2,distance(get_node_position(poi->node1),get_node_position(poi->node2)));
        poi=poi->suiv;
    }
	
    prim_tree(debut,&nodedata->tree_BIP,connexions,graphElements);
}*/


