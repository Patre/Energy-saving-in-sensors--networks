#include "arbre.h"


/***********************AJOUTE**************************/
void arbre_add_pere(arbre **a, int val)
{
    *a=malloc(sizeof(arbre));
    (*a)->node=val;
    (*a)->size=0;
    int i=FILS_MAX-1;
    while(i--)  (*a)->fils[i]=Nullptr(arbre);

}

/**************************************************************/
void arbre_add_fils(arbre *l,int in,int fils)
{
    if(l->node==in)
    {
        //printf("je suis la ajoute %d dans le pere %d a l'indice %d\n",fils,in,l->size);
        arbre_add_pere(&l->fils[l->size],fils);
        l->size++;
        return;
    }
    else
    {
        //printf("je veux inserer %d dans %d\n",fils,in);

        int i=l->size;
        while(i--)
            arbre_add_fils(l->fils[i],in, fils);

    }

}

/***************affichage *****************/
void arbre_affiche(arbre *l)
{
    printf("N %d : {",l->node);
    int i=l->size;
    while(i--)
		printf("%d,",l->fils[i]->node);
    printf("}\n");

    i=l->size;
    while(i--)
		arbre_affiche(l->fils[i]);
    //printf("\n");
}

/*******************************DESTRUCTION************************/
void arbre_detruire(arbre **l)
{
    if((*l)->size==0)
    {
        //printf("je detrruit le Noeud %d\n",(*l)->node);
        free(*l);
        return;
    }
    else
    {
        int i=0;
        i=(*l)->size;
        while(i--)      arbre_detruire(&(*l)->fils[i]);
        //printf("je detrruit le Noeud %d\n",(*l)->node);
        free(*l);
    }

}

/*******************************SUPPRISSON************************/
void arbre_delete_fils(arbre *l1,int val)
{
    int i=0;
    int deleted=0;
    arbre *l=l1;
    for(i=0;i<l->size && !deleted;i++)
    {
        if(l->fils[i]->node==val)
        {
            arbre *tmp=l->fils[i];
            int j=0;
            for(j=i;j<l->size-1;j++)    l->fils[j]=l->fils[j+1];
            i=l->size;
            l->size--;
            deleted=1;
            arbre_detruire(&tmp);
        }
    }
    if(!deleted)
    {
        for(i=0;i<l->size;i++)
        {
            arbre_delete_fils(l->fils[i],val);
        }
    }
}

/*******************************ARbre moins list************************/
void arbre_moins_list(arbre *a,list *l)
{

    while(l)
    {
        arbre_delete_fils(a,l->val);
        l=l->suiv;
    }
}


/*********************COPY******************************/
void arbre_copy_fils(arbre *a, arbre *c,int pere)
{
    if(c == Nullptr(arbre)) return;
    else
    {
        if(c->node!=pere)   arbre_add_fils(a,pere,c->node);
        int i=c->size;
        i=c->size;
        while(i--)      arbre_copy_fils(a,c->fils[i],c->node);
    }

}

void arbre_copy(arbre **a,arbre *c)
{
    arbre_add_pere(a,c->node);
    arbre_copy_fils(*a,c,c->node);
}

/***************************************RECHERCE****************/
int arbre_recherche(arbre *a,int node)
{
    if(a == Nullptr(arbre)) return 0;
    else if(a->node==node)   return 1;
    else
    {
        int bol =0,i=0;
        i=a->size;
        while(i--)      bol+=arbre_recherche(a->fils[i],node);
        return bol;
    }

}

/***********************GET FILS************/
void arbre_get_fils(list **l, arbre *a,int pere)
{
    if(a->node==pere)
    {
        int i =0;
        i=a->size;
        while(i--)      list_insert(l,a->fils[i]->node);
    }
    else
    {
        int i =0;
        i=a->size;
        while(i--)      arbre_get_fils(l,a->fils[i],pere);
    }
}

int arbre_is_leaf(arbre *a, int node)
{
    if(a->node==node)
    {
        return (a->size == 0);
    }
    else
    {
        int i = 0, ret;
        i=a->size;
        while(i--)
		{
			ret = arbre_is_leaf(a->fils[i], node);
			if(ret > 0)
				return ret;
		}
    }
	return 0;
}


