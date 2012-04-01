#include "list_connected.h"

/***********************AJOUTE**************************/
void list_con_insert(listC **l,int node1,int node2,double poids)
{
    listC *tmp=malloc(sizeof(listC));
    tmp->node1=node1;
    tmp->node2=node2;
    tmp->poids=poids;
    tmp->suiv=*l;
    *l=tmp;

}


/***********************RECHERCHE**************************/
int list_con_recherche(listC *l,int node1,int node2)
{
        int bol=0;
        while(l&&!bol)
        {
                if((l->node1==node1 && l->node2==node2)||
                        (l->node1==node2 && l->node2==node1))	bol=1;
                l=l->suiv;
        }
        return bol;
}
/***********************AFFICHAGE**************************/
void list_con_affiche(listC *l)
{
    printf("{");
    while(l)
    {
        printf("(%d,%d,%.2lf)",l->node1,l->node2,l->poids);
        l=l->suiv;
    }
    printf("} \n");
}
/*******************SUPPRIME*****************/
int  list_con_delete(listC **l, int node1,int node2)
{
        if(!list_con_recherche(*l,node1,node2))	return 0;

        if(((*l)->node1==node1 && (*l)->node2==node2)||
                ((*l)->node1==node2 && (*l)->node2==node1))
        {
                listC *tmp=*l;
                *l=(*l)->suiv;
                free(tmp);
                return 1;
        }
        else
        {
                listC *tmp=*l;
                while((tmp->suiv->node1==node1 && tmp->suiv->node2==node2)||
                    (tmp->suiv->node1==node2 && tmp->suiv->node2==node1))	tmp=tmp->suiv;
                listC *tmp2=tmp->suiv;
                tmp->suiv=tmp->suiv->suiv;
                free(tmp2);
                return 1;
        }//*/
        return 1;
}

///

/***********************Modfier le poids**************************/
//POUR INITAILISER LE POIDs
void list_set_poids(listC *l,int node1,int node2,double poids)
{
    if(!list_con_recherche(l,node1,node2))  return;
    while(l)
    {
        if((l->node1==node1 && l->node2==node2)||
                (l->node1==node2 && l->node2==node1))   l->poids=poids;
        l=l->suiv;
    }
}

