
#include "Initialisation.h"
void get_PROTOCOLE_init(call_t *c, double eps)
{
    int count=get_node_count();
    uint64_t at=count*time_seconds_to_nanos(eps)+time_seconds_to_nanos(eps);
    scheduler_add_callback(at, c, init_lbop, NULL);
}


/* ************************************************** */
/* ************************************************** */
//initailisation
void init_lbop(call_t *c)
{

    struct nodedata *nodedata = get_node_private_data(c);
    struct protocoleData *entitydata = get_entity_private_data(c);
    if(entitydata->debug)
        DBG("LBOP - %d INITIALISATION AT %lf\n",c->node,get_time_now_second());

    list *N1=Nullptr(list);
    listeNodes_to_list(&N1,nodedata->oneHopNeighbourhood);

    //REcuperer le graph G
    list *graphElements=Nullptr(list);
    list_copy(&graphElements,N1);
    list_delete(&graphElements,c->node);

    //list de commencement
    list *debut=Nullptr(list);
    list_insert(&debut,c->node);

    //recuperer tout les connexion dans le graph
    listC *connexions=Nullptr(listC);
    listeNodes_to_listC(&connexions,nodedata->oneHopNeighbourhood,c->node);

    double range=get_range_Tr(c);

    listC *poi=connexions;
    while (poi)
    {
            //on ai dans le bip, nous avons le droit de fair Áa
            list_set_poids(connexions,poi->node1,poi->node2,distance(get_node_position(poi->node1),get_node_position(poi->node2)));
            poi=poi->suiv;
    }

    //recuperer tout les connexion LOCAL
    listeNodes *tmp1=nodedata->oneHopNeighbourhood;
    while(tmp1)
    {
        position_t pos1 = {tmp1->values.x,tmp1->values.y,tmp1->values.z};
        listeNodes *tmp2=nodedata->oneHopNeighbourhood;
        while(tmp2)
        {
            position_t pos2 = {tmp2->values.x,tmp2->values.y,tmp2->values.z};
            double dist=distance(&pos1,&pos2);
            if(dist < range  && tmp1->values.node != tmp2->values.node)
                if(!list_con_recherche(connexions,tmp1->values.node,tmp2->values.node))
                    list_con_insert(&connexions,tmp1->values.node,tmp2->values.node,dist);
            tmp2=tmp2->suiv;
        }
        tmp1=tmp1->suiv;
    }



    arbre *MST=Nullptr(arbre);
    arbre_add_pere(&MST,c->node);
    prim_tree(debut,&MST,connexions,graphElements);//*/
    arbre_get_fils(&nodedata->LMST_intial,MST,c->node);

//    list_copy(&nodedata->LMST_voisin,nodedata->LMST_intial);

    if(entitydata->debug)
    {
        DBG("LMST %d ",c->node);
        list_affiche(nodedata->LMST_intial);
    }

}
