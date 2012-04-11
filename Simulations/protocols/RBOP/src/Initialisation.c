
#include "Initialisation.h"
void get_PROTOCOLE_init(call_t *c, double eps)
{
    int count=get_node_count();
    uint64_t at=count*time_seconds_to_nanos(eps)+time_seconds_to_nanos(1);
    printf("%lf\n",time_nanos_to_seconds(at));
    scheduler_add_callback(at, c, init_rbop, NULL);
}

void get_RNG_tree(int node,list **a,list *N1)
{
    list_copy(a,N1);
    list *tmp=N1;

    //la deuxieme boucle de l'algoritme
    while(tmp)
    {
        //la distance entre le Neud Source et un des un de ces voisin
        double dUV=distance(get_node_position(node),get_node_position(tmp->val));
        list *tmp2=N1;

        //la boucle qui cherche si ya pas un noued au milieu
        while(tmp2)
        {
            //il faut test que ce n'est pas le meme noeud
            if(tmp->val!=tmp2->val)
            {
                //la distance entre le pere et un noud choisi
                double dUW=distance(get_node_position(node),get_node_position(tmp2->val));


                //la distance entre le comparé et le nouveux
                double dVW=distance(get_node_position(tmp->val),get_node_position(tmp2->val));


                //le test de RNG
                if(dUV>dUW && dUV>dVW)
                    list_delete(a,tmp->val);
            }

            tmp2=tmp2->suiv;
        }

        tmp=tmp->suiv;
    }


}

//calcul energie entre NODE A,NODE B
/* ************************************************** */
/* ************************************************** */
//initailisation de l'arbre de LBIP
void init_rbop(call_t *c, void *args)
{

    struct nodedata *nodedata = get_node_private_data(c);

    list *N1=Nullptr(list);
    listeNodes_to_list(&N1,nodedata->oneHopNeighbourhood);

    get_RNG_tree(c->node,&nodedata->RNG,N1);

    printf(" Node %d  ",c->node);

    list_affiche(N1);
    printf(" RNG %d  ",c->node);

    list_affiche(nodedata->RNG);
}
