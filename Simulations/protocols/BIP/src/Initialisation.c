
#include "Initialisation.h"

/* ************************************************** */
/* ************************************************** */
//INIT LBIP
void get_PROTOCOLE_init(call_t *c, double eps)
{
    int count=get_node_count()*2;
    uint64_t fin_two_hop=count*time_seconds_to_nanos(eps)+c->node*time_seconds_to_nanos(eps);
    uint64_t at=fin_two_hop;
    scheduler_add_callback(at, c, init_bip_tree, NULL);
}


/* ************************************************** */
/* ************************************************** */
//initailisation de l'arbre de LBIP
void init_bip_tree(call_t *c, void *args)
{
    struct nodedata *nodedata = get_node_private_data(c);
    struct protocoleData *entitydata = get_entity_private_data(c);

    int i,j;
    for(i=0;i<get_node_count();i++)
        if(i!=c->node)        addVertex(nodedata->g2hop,i);

    double range = get_range_Tr(c);

    //DEBUG
    //if(c->node==0)  printf("RANGE = > %lf\n",range);

    double dist;
    for(i=0;i<get_node_count();i++)
        for(j=0;j<get_node_count();j++)
        {
            dist=distance(get_node_position(i),get_node_position(j));
            if(i!=j && dist<= range)
            {
                double cout = calcul_energie(*get_node_position(i),*get_node_position(j),entitydata->alpha,entitydata->c,&dist);
                addEdgeUndirected(nodedata->g2hop, i, j, cout);
            }
        }

}





