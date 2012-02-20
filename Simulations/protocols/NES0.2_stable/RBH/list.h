#ifndef  _LIST_RBH
#define  _LIST_RBH


#define Nullptr(type) (type *)0

typedef struct _list list;
struct _list
{
    int val;
    list *suiv;
};

/***********************AJOUTE**************************/
void list_insert(list **l,int val)
{
    list *tmp=malloc(sizeof(list));
    if(!tmp)
    {
        printf("erreur de creation de la liste\n");
        exit(-5);
    }

    tmp->val=val;
    tmp->suiv=*l;
    *l=tmp;

}

/***********************AFFICHAGE**************************/
void list_affiche(list *l)
{
    while(l)
    {
        printf("%d\t",l->val);
        l=l->suiv;
    }
    printf("\n");
}
/***********************TAILLE**************************/
int list_taille(list *l)
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
int list_get(list *l,int index)
{
    if(index>=list_taille(l))	return Nullptr(list);
        else
        {
                while(index)
                {
                        l=l->suiv;
                        index--;
                }

                return l->val;
        }
}
/***********************DESTRUCTION**************************/
void list_detruire(list **l)
{
        list *tmp=*l;
        while(*l)
        {
             tmp = (*l)->suiv;
             free(*l);
             *l = tmp;
        }
}
/***********************RECHERCHE**************************/
int list_recherche(list *l,int val)
{
        int bol=0;
        while(l&&!bol)
        {
                if(l->val==val)	bol=1;
                l=l->suiv;
        }
        return bol;
}

/*********************************COPIER*******************************/
void list_copy(list **des,list *src)
{
    while(src)
        {
                list_insert(des,src->val);
                src=src->suiv;
        }
}



/*************************************SUPPRIME***********************/
int  list_delete(list **l, int val)
{
        if(!list_recherche(*l,val))	return 0;

        if((*l)->val==val)
        {
                list *tmp=*l;
                *l=(*l)->suiv;
                free(tmp);
                return 1;
        }
        else
        {
                list *tmp=*l;
                while(tmp->suiv->val!=val)	tmp=tmp->suiv;
                list *tmp2=tmp->suiv;
                tmp->suiv=tmp->suiv->suiv;
                free(tmp2);
                return 1;
        }//*/
        return 1;
}
/***************************INTERSECTION***********************/
void list_intersection(list **l1,list *l2)
{
        while(l2)
        {
                if(list_recherche(*l1,l2->val))	list_delete(l1,l2->val);
                l2=l2->suiv;
        }
}

#endif //_LIST_RBH
