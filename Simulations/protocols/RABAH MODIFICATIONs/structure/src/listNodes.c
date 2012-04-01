

#include "listNodes.h"

/***********************AJOUTE**************************/
void listeNodes_insert(listeNodes **l,int node)
{
    listeNodes *tmp=malloc(sizeof(listeNodes));
    if(!tmp)
    {
        printf("erreur de creation de la listeNodese\n");
        exit(-5);
    }

    tmp->values.node=node;
    tmp->values.x=0;
    tmp->values.y=0;
    tmp->values.z=0;

    tmp->suiv=*l;
    *l=tmp;
}

void listeNodes_insert_values(listeNodes **liste, int node, double x, double y, double z)
{
    listeNodes *tmp=malloc(sizeof(listeNodes));
    if(!tmp)
    {
        printf("erreur de creation de la listeNodese\n");
        exit(-5);
    }

    tmp->values.node=node;
    tmp->values.x=x;
    tmp->values.y=y;
    tmp->values.z=z;

    tmp->suiv=*liste;
    *liste=tmp;

}
void listeNodes_insert_element(listeNodes **liste, element node)
{
    listeNodes *tmp=malloc(sizeof(listeNodes));
    if(!tmp)
    {
        printf("erreur de creation de la listeNodese\n");
        exit(-5);
    }

    tmp->values.node=node.node;
    tmp->values.x=node.x;
    tmp->values.y=node.y;
    tmp->values.z=node.z;

    tmp->suiv=*liste;
    *liste=tmp;

}

//  ***********************AFFICHAGE**************************
void listeNodes_affiche(listeNodes *l)
{
    int x=listeNodes_taille(l);
    printf("{");
	if(l != NULL)
	{
		printf("%d",l->values.node);
        l=l->suiv;
	}
		
    while(l)
    {
        printf(",%d",l->values.node);
        l=l->suiv;
    }
    printf("} (total : %d)\n",x);
}
// --***********************TAILLE**************************
int listeNodes_taille(listeNodes *l)
{
        int nbr=0;
        while(l)
        {
                nbr++;
                l=l->suiv;
        }
        return nbr;
}
//  ***********************RECUPERER UN ELEMENT**************************
int listeNodes_get(listeNodes *l,int index)
{
    if(index>=listeNodes_taille(l))	return -1;
        else
        {
                while(index)
                {
                        l=l->suiv;
                        index--;
                }

                return l->values.node;
        }
}
//  ***********************DESTRUCTION**************************
void listeNodes_detruire(listeNodes **l)
{
        listeNodes *tmp=*l;
        while(*l)
        {
             tmp = (*l)->suiv;
             free(*l);
             *l = tmp;
        }
}
//  ***********************RECHERCHE**************************
int listeNodes_recherche(listeNodes *l,int val)
{
        int trouve=0;
        while(l && !trouve)
        {
                if(l->values.node==val)
					trouve=1;
                l=l->suiv;
        }
        return trouve;
}

//  *********************************COPIER*******************************
void listeNodes_copy(listeNodes **des,listeNodes *src)
{
    while(src)
        {
                listeNodes_insert_values(des,src->values.node,src->values.x,
                                         src->values.y,src->values.z);
                src=src->suiv;
        }
}



//  *************************************SUPPRIME***********************
int  listeNodes_delete(listeNodes **l, int val)
{
        if(!listeNodes_recherche(*l,val))	return 0;

        if((*l)->values.node==val)
        {
                listeNodes *tmp=*l;
                *l=(*l)->suiv;
                free(tmp);
                return 1;
        }
        else
        {
                listeNodes *tmp=*l;
                while(tmp->suiv->values.node!=val)	tmp=tmp->suiv;
                listeNodes *tmp2=tmp->suiv;
                tmp->suiv=tmp->suiv->suiv;
                free(tmp2);
                return 1;
        }
        return 1;
}


//  ***************************INTERSECTION***********************
void listeNodes_intersection(listeNodes **l1,listeNodes *l2)
{
        while(l2)
        {
                if(listeNodes_recherche(*l1,l2->values.node))	listeNodes_delete(l1,l2->values.node);
                l2=l2->suiv;
        }
}
//  ***********************TRENSFORMER**************************
void listeNodes_to_listC(listC **l,listeNodes *l2,int node)
{
    while(l2)
    {
        if(!list_con_recherche(*l,node,l2->values.node)) list_con_insert(l,node,l2->values.node,0);
        l2=l2->suiv;
    }
}

void listC_to_listeNodes(listeNodes **l,listC *c)
{
    while(c)
    {
        if(!listeNodes_recherche(*l,c->node1)) listeNodes_insert(l,c->node1);
        if(!listeNodes_recherche(*l,c->node2)) listeNodes_insert(l,c->node2);
        c=c->suiv;
    }
}

//  *****************UNION******************************
void listeNodes_union(listeNodes **l1,listeNodes *l2)
{
        while(l2)
        {
                if(!listeNodes_recherche(*l1,l2->values.node))	listeNodes_insert_element(l1,l2->values);
                l2=l2->suiv;
        }
}
//*/


//  Converstion

void listeNodes_to_list(list **liste, listeNodes *list_nodes)
{
    while(list_nodes)
    {
        list_insert(liste, list_nodes->values.node);
        list_nodes=list_nodes->suiv;
    }
}
