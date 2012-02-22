#ifndef LIST_DE_LIST_H
#define LIST_DE_LIST_H
#include "list.h"

#define Nullptr(type) (type *)0

struct _list_of_list
{
    int node;
    list *peres;
    struct _list_of_list *suiv;
};

typedef struct _list_of_list list2;

/***********************AJOUTE**************************/
void list2_insert(list2 **l,int nodeid, list *peres)
{
    list2 *tmp=malloc(sizeof(list2));
    if(!tmp)
    {
        printf("erreur de creation de la liste\n");
        exit(-5);
    }

    tmp->node=nodeid;
    tmp->peres=Nullptr(list);
    list_copy(&tmp->peres,peres);
    tmp->suiv=*l;
    *l=tmp;

}

/***********************AFFICHAGE**************************/
void list2_affiche(list2 *l)
{
    while(l)
    {
        printf("%d\t",l->node);
        list_affiche(l->peres);
        l=l->suiv;
    }
    printf("\n\n");
}
/***********************TRENSFORMER**************************/
void list2_to_list(list **l,list2 *l2)
{
    while(l2)
    {
        if(!list_recherche(*l,l2->node)) list_insert(l,l2->node);
        int i=0;
        for(i=0;i<list_taille(l2->peres);i++)
        {
            int node= list_get(l2->peres,i);
            if(!list_recherche(*l,node)) list_insert(l,node);
        }

        l2=l2->suiv;
    }
}

/***********************TAILLE**************************/
int list2_taille(list2 *l)
{
        int nbr=0;
        while(l)
        {
                nbr++;
                l=l->suiv;
        }
        return nbr;
}
/***********************RECUPERER UN ELEMENT**************************/
list2 list2_get(list2 *l,int index)
{

                while(index)
                {
                        l=l->suiv;
                        index--;
                }

                list2 tmp;
                tmp.node=l->node;
                tmp.peres=Nullptr(list);
                list_copy(&tmp.peres,l->peres);
                tmp.suiv=Nullptr(list2);

                return tmp;

}
/***********************DESTRUCTION**************************/
void list2_detruire(list2 **l)
{
        list2 *tmp=*l;
        while(*l)
        {
             tmp = (*l)->suiv;
             list_detruire(&(*l)->peres);
             free(*l);
             *l = tmp;
        }
}
/***********************RECHERCHE**************************/
int list2_recherche(list2 *l,int val)
{
        int bol=0;
        while(l&&!bol)
        {
                if(l->node==val)	bol=1;
                l=l->suiv;
        }
        return bol;
}

int list2_recherche_pere(list2 *l,int val)
{
        int bol=0;
        while(l)
        {
                if(list_recherche(l->peres,val))	bol++;
                l=l->suiv;
        }
        return bol;
}

/*********************************COPIER*******************************/
void list2_copy(list2 **des,list2 *src)
{
    while(src)
        {
                list2_insert(des,src->node,src->peres);
                src=src->suiv;
        }
}



/*************************************SUPPRIME***********************/
int  list2_delete(list2 **l, int val)
{
        if(!list2_recherche(*l,val))	return 0;

        if((*l)->node ==val)
        {
                list2 *tmp=*l;
                *l=(*l)->suiv;
                list_detruire(&tmp->peres);
                free(tmp);
                return 1;
        }
        else
        {
                list2 *tmp=*l;
                while(tmp->suiv->node!=val)	tmp=tmp->suiv;
                list2 *tmp2=tmp->suiv;
                tmp->suiv=tmp->suiv->suiv;
                list_detruire(&tmp2->peres);
                free(tmp2);
                return 1;
        }//*/
        return 1;
}
//supression d'un pere
int  list2_delete_pere_from_fils(list2 *l,int fils, int pere)
{
    if(!list2_recherche_pere(   l,pere))	return 0;

    while(l)
    {
        if(l->node==fils)   list_delete(&l->peres,pere);
        l=l->suiv;
    }
}

#endif // LIST_DE_LIST_H
