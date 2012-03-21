#include "../list_de_listNodes.h"


/***********************AJOUTE**************************/
void list2N_insert(list2N **l,int nodeid, listeNodes *peres)
{
    list2N *tmp=malloc(sizeof(list2N));
    if(!tmp)
    {
        printf("erreur de creation de la liste\n");
        exit(-5);
    }

    tmp->values.node=nodeid;
    tmp->values.x=0;
    tmp->values.y=0;
    tmp->values.z=0;

    tmp->peres=Nullptr(listeNodes);
    listeNodes_copy(&tmp->peres,peres);
    tmp->suiv=*l;
    *l=tmp;

}

void list2N_insert_values(list2N **l, int nodeid, double x, double y, double z, listeNodes *peres)
{
    list2N *tmp=malloc(sizeof(list2N));
    if(!tmp)
    {
        printf("erreur de creation de la liste\n");
        exit(-5);
    }

    tmp->values.node=nodeid;
    tmp->values.x=x;
    tmp->values.y=y;
    tmp->values.z=z;

    tmp->peres=Nullptr(listeNodes);
    listeNodes_copy(&tmp->peres,peres);
    tmp->suiv=*l;
    *l=tmp;
}

void list2N_insert_element(list2N **l, element node, listeNodes *peres)
{
    list2N *tmp=malloc(sizeof(list2N));
    if(!tmp)
    {
        printf("erreur de creation de la liste\n");
        exit(-5);
    }

    tmp->values.node=node.node;
    tmp->values.x=node.x;
    tmp->values.y=node.y;
    tmp->values.z=node.z;

    tmp->peres=Nullptr(listeNodes);
    listeNodes_copy(&tmp->peres,peres);
    tmp->suiv=*l;
    *l=tmp;

}

/***********************AFFICHAGE**************************/

void list2N_affiche(list2N *l)
{
    while(l)
    {
        printf("%d\t",l->values.node);
        listeNodes_affiche(l->peres);
        l=l->suiv;
    }
    printf("\n\n");
}

/***********************TAILLE**************************/

int list2N_taille(list2N *l)
{
        int nbr=0;
        while(l)
        {
                nbr++;
                l=l->suiv;
        }
        return nbr;
}
/***********************DESTRUCTION**************************/

void list2N_detruire(list2N **l)
{
        list2N *tmp=*l;
        while(*l)
        {
             tmp = (*l)->suiv;
             listeNodes_detruire(&(*l)->peres);
             free(*l);
             *l = tmp;
        }
}
/***********************RECHERCHE**************************/
int list2N_recherche(list2N *l,int val)
{
        int bol=0;
        while(l&&!bol)
        {
                if(l->values.node==val)	bol=1;
                l=l->suiv;
        }
        return bol;
}
int list2N_recherche_pere(list2N *l,int val)
{
        int bol=0;
        while(l)
        {
                if(listeNodes_recherche(l->peres,val))	bol++;
                l=l->suiv;
        }
        return bol;
}

/*********************************COPIER*******************************/
void list2N_copy(list2N **des,list2N *src)
{
    while(src)
        {
                list2N_insert(des,src->values.node,src->peres);
                src=src->suiv;
        }
}



/*************************************SUPPRIME***********************/

int  list2N_delete(list2N **l, int val)
{
        if(!list2N_recherche(*l,val))	return 0;

        if((*l)->values.node ==val)
        {
                list2N *tmp=*l;
                *l=(*l)->suiv;
                listeNodes_detruire(&tmp->peres);
                free(tmp);
                return 1;
        }
        else
        {
                list2N *tmp=*l;
                while(tmp->suiv->values.node!=val)	tmp=tmp->suiv;
                list2N *tmp2=tmp->suiv;
                tmp->suiv=tmp->suiv->suiv;
                listeNodes_detruire(&tmp2->peres);
                free(tmp2);
                return 1;
        }//*/
        return 1;
}
//supression d'un pere

int  list2N_delete_pere_from_fils(list2N *l,int fils, int pere)
{
    if(!list2N_recherche_pere(l,pere))	return 0;

    while(l)
    {
        if(l->values.node==fils)   listeNodes_delete(&l->peres,pere);
        l=l->suiv;
    }
    return 1;
}
/***********************TRENSFORMER**************************/
void list2N_to_list(list **l,list2N *l2)
{
    while(l2)
    {
        if(!list_recherche(*l,l2->values.node)) list_insert(l,l2->values.node);
        int i=0;


        for(i=0;i<listeNodes_taille(l2->peres);i++)
        {
            int node= listeNodes_get(l2->peres,i);
            if(!list_recherche(*l,node)) list_insert(l,node);
        }

        l2=l2->suiv;
    }
}
//LIST CONNECTER
void list2N_to_listC(listC **l,list2N *l2)
{
    while(l2)
    {
        int i=0;
        for(i=0;i<listeNodes_taille(l2->peres);i++)
        {
            int node= listeNodes_get(l2->peres,i);
            if(!list_con_recherche(*l,l2->values.node,node)) list_con_insert(l,l2->values.node,node,0);
        }

        l2=l2->suiv;
    }
}

