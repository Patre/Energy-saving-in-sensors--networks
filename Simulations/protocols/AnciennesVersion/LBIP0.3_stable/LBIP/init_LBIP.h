#ifndef INIT_LBIP_H
#define INIT_LBIP_H

#include <include/modelutils.h>

#include "RBH/list.h"
#include "RBH/list_de_list.h"
#include "RBH/arbre.h"
#include "RBH/time_RBH.h"

#include "struct_LBIP.h"

/*LES FONCTION DE CE FICHIER*/
void get_LBIP_init(call_t *c, double eps);
double   calcule_energie(int A,int B,double alpha,double c);
void prim_tree(int node,arbre **a,listC *l,list *g);
void prim_tree_lbip(int node,arbre **a, list *N1,list2 *N2,double alpha,double c);
void init_lbip_tree(call_t *c, void *args);


/* ************************************************** */
/* ************************************************** */
//INIT LBIP
void get_LBIP_init(call_t *c, double eps)
{
    int count=get_node_count()*2;
    uint64_t fin_two_hop=count*time_seconds_to_nanos(eps)+time_seconds_to_nanos(eps);
    uint64_t at=fin_two_hop;
    scheduler_add_callback(at, c, init_lbip_tree, NULL);
}

/* ************************************************** */
/* ************************************************** */
//calcul energie entre NODE A,NODE B
double   calcul_energie(int A,int B,double alpha,double c)
{
    double Ax,Ay,Az,Bx,By,Bz;
    double dAB;
    double par1;

    //RECuperer la postion de A
    Ax=get_node_position(A)->x;
    Ay=get_node_position(A)->y;
    Az=get_node_position(A)->z;

    //RECuperer la postion de B
    Bx=get_node_position(B)->x;
    By=get_node_position(B)->y;
    Bz=get_node_position(B)->z;

    //Distance
    dAB =sqrt((Ax-Bx)*(Ax-Bx)+
              (Ay-By)*(Ay-By)+
              (Az-Bz)*(Az-Bz));

    //la premier partie de la fonction
    par1=pow(dAB,alpha);

    return par1+c;
}

/* ************************************************** */
/* ************************************************** */
void prim_tree(int node,arbre **a,listC *l,list *g)
{

    int nbrNode=list_taille(g);
    list *deja_couvert=Nullptr(list);
    list_insert(&deja_couvert,node);


    listC *tmp=l;
    list *tmp2=deja_couvert;

    //list_con_affiche(tmp);
    //phase 1
    /*
      * Recuperer le minimum  a partir de node racine node
      */
    double min_poids=9999999999999999999999.00;
    int node_min=-1;
    int node_couvrant=-1;


    int i=0;
    //COUVRIR TOUS LES NOEUDS
    for(i=0;i<nbrNode;i++)
    {
        tmp2=deja_couvert;

        //RENIALISER LES PARAMETRE
        min_poids=999999999999999999999999999999999.00;
        node_min=-1;
        node_couvrant=-1;

        //parcourir des noeud deja couvert
        while(tmp2)
        {
            //recuperer le noeud couvert en cours
            node=tmp2->val;

            //printf("NR: %d comparer avec",node);
            tmp=l;


            //Parcourir les list de connection
            while(tmp)
            {

                ///Si une connection trouver avec comme literal un noued deja couvert et que le poids est minimal
                if((tmp->node1 == node || tmp->node2 == node)   && (tmp->poids<min_poids))
                {
                    //afficher l'extance de noued dans la list des connexion
                    //printf("(%d,%d),  ",tmp->node1,tmp->node2,tmp->poids);


                    //si le noeud literl 1 est le noeud source et que l'autre n'est pas deja couvert
                    if(tmp->node1==node && !list_recherche(deja_couvert,tmp->node2))
                    {
                        //Sauvgarder le node avec le minimum poids
                        node_couvrant=node;

                        node_min=tmp->node2;
                        min_poids=tmp->poids;
                    }
                    //sinon le contraire
                    else if(!list_recherche(deja_couvert,tmp->node1))
                    {
                        //Sauvgarder le node avec le minimum poids
                        node_couvrant=node;

                        node_min=tmp->node1;
                        min_poids=tmp->poids;
                    }
                }
                //ON passse a la connection suivante
                tmp=tmp->suiv;
            }
            //printf("Min is (%d,%d) %.2lf\n",node_couvrant, node_min,min_poids);
            //On passe a le noued couvert suivant
            tmp2=tmp2->suiv;
        }//*/



        //On ajoute l'element dans les couvert
        //printf("NR:%d NM:%d P:%.2lf\n",node_couvrant,node_min,min_poids);
        list_insert(&deja_couvert,node_min);

        //on ajoute dans l'arbre
        arbre_add_fils(*a,node_couvrant,node_min);
    }

    DEBUG;
    //printf("\n\n");
    //list_affiche(deja_couvert);
    //arbre_affiche(*a);
}

void prim_tree2(list *nodes,arbre **a,listC *l,list *g)
{

    int nbrNode=list_taille(g);
    list *deja_couvert=Nullptr(list);
    list_copy(&deja_couvert,nodes);


    listC *tmp=l;
    list *tmp2=deja_couvert;

    //list_con_affiche(tmp);
    //phase 1
    /*
      * Recuperer le minimum  a partir de node racine node
      */
    double min_poids=9999999999999999999999.00;
    int node_min=-1;
    int node_couvrant=-1;

    int node;

    int i=0;
    //COUVRIR TOUS LES NOEUDS
    for(i=0;i<nbrNode;i++)
    {
        tmp2=deja_couvert;

        //RENIALISER LES PARAMETRE
        min_poids=999999999999999999999999999999999.00;
        node_min=-1;
        node_couvrant=-1;

        //parcourir des noeud deja couvert
        while(tmp2)
        {
            //recuperer le noeud couvert en cours
            node=tmp2->val;

            //printf("NR: %d comparer avec",node);
            tmp=l;


            //Parcourir les list de connection
            while(tmp)
            {

                ///Si une connection trouver avec comme literal un noued deja couvert et que le poids est minimal
                if((tmp->node1 == node || tmp->node2 == node)   && (tmp->poids<min_poids))
                {
                    //afficher l'extance de noued dans la list des connexion
                    //printf("(%d,%d),  ",tmp->node1,tmp->node2,tmp->poids);


                    //si le noeud literl 1 est le noeud source et que l'autre n'est pas deja couvert
                    if(tmp->node1==node && !list_recherche(deja_couvert,tmp->node2))
                    {
                        //Sauvgarder le node avec le minimum poids
                        node_couvrant=node;

                        node_min=tmp->node2;
                        min_poids=tmp->poids;
                    }
                    //sinon le contraire
                    else if(!list_recherche(deja_couvert,tmp->node1))
                    {
                        //Sauvgarder le node avec le minimum poids
                        node_couvrant=node;

                        node_min=tmp->node1;
                        min_poids=tmp->poids;
                    }
                }
                //ON passse a la connection suivante
                tmp=tmp->suiv;
            }
            //printf("Min is (%d,%d) %.2lf\n",node_couvrant, node_min,min_poids);
            //On passe a le noued couvert suivant
            tmp2=tmp2->suiv;
        }//*/



        //On ajoute l'element dans les couvert
        //printf("NR:%d NM:%d P:%.2lf\n",node_couvrant,node_min,min_poids);
        list_insert(&deja_couvert,node_min);

        //on ajoute dans l'arbre
        arbre_add_fils(*a,node_couvrant,node_min);
    }

    DEBUG;
    //printf("\n\n");
    //list_affiche(deja_couvert);
    //arbre_affiche(*a);
}

//initailisation de l'arbre de LBIP
void prim_tree_lbip(int node,arbre **a, list *N1, list2 *N2, double alpha, double c)
{
    listC *connect=Nullptr(listC);
    list_to_listC(&connect,N1,node);
    list2_to_listC(&connect,N2);

    listC *tmp=connect;
    while(tmp)
    {
        double x=calcul_energie(tmp->node1,tmp->node2,alpha,c);
        list_set_poids(tmp,tmp->node1,tmp->node2,x);
        tmp=tmp->suiv;
    }


    list *g=Nullptr(list);
    list2_to_list(&g,N2);

    DEBUG;
    /*printf("\n*************************************************************\n"
           "TOUS ces VOISINAGE DE 1 HOPE et 2 HOP de NODE %d\n",node);
    list_affiche(g);
    printf("\nTOUS les connection LOCAL (node1,node2, POIDs)\n");
    list_con_affiche(connect);*/

    prim_tree(node,a,connect,g);

}

/* ************************************************** */
/* ************************************************** */
//initailisation de l'arbre de LBIP
void init_lbip_tree(call_t *c, void *args)
{
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydataLBIP *entitydata=get_entity_private_data(c);

    prim_tree_lbip(c->node,&nodedata->tree_LBIP,nodedata->N1,nodedata->N2, entitydata->alpha, entitydata->c);

    DEBUG;
    /*printf("INITIALISATION DE L'ARBRE LBIP pour le noeud %d at %lf\n",c->node,
          get_time_now_second());//*/
    //arbre_detruire(&nodedata->tree_LBIP);
}

#endif // INIT_LBIP_H
