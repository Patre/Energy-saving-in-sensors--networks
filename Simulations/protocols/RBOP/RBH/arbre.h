#ifndef ARBRE_RBH
#define ARBRE_RBH

#include "list.h"
#define FILS_MAX    20

struct _arbre
{
    int node;
    int size;
    struct _arbre *fils[FILS_MAX];
};
typedef struct _arbre arbre;

/*******************/
/*FONCTION DANS Ce FICHIER*/
void arbre_add_pere(arbre **a, int val);
void arbre_add_fils(arbre *a,int in,int fils);
void arbre_affiche(arbre *a);
void arbre_detruire(arbre **a);
void arbre_delete_fils(arbre *a,int val);
void arbre_moins_list(arbre *a,list *l);
void arbre_to_list(list **lst,arbre *a);
void arbre_to_list_fils(list **lst,arbre *a);
void arbre_copy_fils(arbre *a, arbre *c,int pere);
void arbre_copy(arbre **a,arbre *c);


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
    while(i--)      printf("%d,",l->fils[i]->node);
    printf("}\n");

    i=l->size;
    while(i--)      arbre_affiche(l->fils[i]);
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


/****************TRANSFORMATION********************************/
//TOUs l'arbre
void arbre_to_list(list **lst,arbre *a)
{
    list_insert(lst,a->node);
    int i=a->size;
    i=a->size;
    while(i--)      arbre_to_list(lst,a->fils[i]);
}
////TOUs l'arbre moins celui de val
void arbre_to_list_sauf(list **lst,arbre *a,int val)
{
    if(a->node!=val)
    {
        list_insert(lst,a->node);
        int i=a->size;
        i=a->size;
        while(i--)      arbre_to_list_sauf(lst,a->fils[i],val);
    }
}

//avec ces fils de premier seulement
void arbre_to_list_fils(list **lst,arbre *a)
{
    int i=a->size;
    i=a->size;
    while(i--)      list_insert(lst,a->fils[i]->node);
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

#endif // ARBRE_RBH
