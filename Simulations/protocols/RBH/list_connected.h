#ifndef LIST_CONNECTED_H
#define LIST_CONNECTED_H

#define Nullptr(type) (type *)0

struct _list_con
{
    int node1;
    int node2;
    double poids;
    struct _list_con *suiv;
};

typedef struct _list_con listC;

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


#endif // LIST_CONNECTED_H
