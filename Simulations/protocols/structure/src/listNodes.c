

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
element listeNodes_getElement(listeNodes *listeNodese,int val)
{
    if(!listeNodes_recherche(listeNodese,val))
    {
        element inter;
        inter.node=-1;
        inter.x=-1;
        inter.y=-1;
        inter.z=-1;
        return inter;
    }
    else
    {
        while(listeNodese)
        {
            if(listeNodese->values.node==val) return listeNodese->values;
            listeNodese=listeNodese->suiv;
        }
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
int listeNodes_recherche(listeNodes *l, int val)
{
        int trouve=0, indice = -1;
        while(l && !trouve)
        {
                if(l->values.node==val)
					trouve=1;
                l=l->suiv;
			indice++;
        }
	return trouve;
}

//  ***********************RECHERCHE**************************
int listeNodes_get_index(listeNodes *l, int val)
{
	int trouve=0, indice = -1;
	while(l && !trouve)
	{
		if(l->values.node==val)
			trouve=1;
		l=l->suiv;
		indice++;
	}
	return indice;
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
	if(!listeNodes_recherche(*l,val))
		return 0;

		if(*l == 0)
			return -1;
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
			while(tmp->suiv != 0 && tmp->suiv->values.node!=val)	tmp=tmp->suiv;
			
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



