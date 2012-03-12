#ifndef INIT_LBIP_H
#define INIT_LBIP_H

#include <include/modelutils.h>

#include "RBH/list.h"
#include "RBH/list_de_list.h"
#include "RBH/arbre.h"
#include "RBH/time_RBH.h"

#include "struct_RBOP.h"

/*LES FONCTION DE CE FICHIER*/
void get_RBOP_init(call_t *c, double eps);
void init_rbop(call_t *c, void *args);
void get_RNG_tree(int node,list **a,list *N1);

/* ************************************************** */
/* ************************************************** */
//INIT LBIP
void get_RBOP_init(call_t *c, double eps)
{
    int count=get_node_count()*2;
    uint64_t fin_two_hop=count*time_seconds_to_nanos(eps)+time_seconds_to_nanos(eps);
    uint64_t at=fin_two_hop;
    scheduler_add_callback(at, c, init_rbop, NULL);
}

/* ************************************************** */
/* ************************************************** */

///GEneration de l'arbre RNG
void get_RNG_tree(int node,list **a,list *N1)
{
    list_copy(a,N1);
    list *tmp=N1;

    DEBUG;
    printf("le voisinage avant RNG ");
    list_affiche(N1);//*/


    //la deuxieme bucle de l'algoritme
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

    DEBUG;
    printf("le voisinage apres RNG");
    list_affiche(*a);//*/

}

//calcul energie entre NODE A,NODE B
/* ************************************************** */
/* ************************************************** */
//initailisation de l'arbre de LBIP
void init_rbop(call_t *c, void *args)
{
    struct nodedata *nodedata = get_node_private_data(c);


    get_RNG_tree(c->node,&nodedata->RNG,nodedata->N1);

    /*DEBUG;
    printf("Node %d ->",c->node);
    list_affiche(nodedata->N1);//*/
    //arbre_detruire(&nodedata->tree_LBIP);
}

#endif // INIT_LBIP_H
