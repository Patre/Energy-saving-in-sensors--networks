#ifndef ARBRE_RBH
#define ARBRE_RBH


#define FILS_MAX    20

struct _arbre
{
    int node;
    int size;
    struct _arbre *fils[FILS_MAX];
};


/*******************/

typedef struct _arbre arbre;
/***********************AJOUTE**************************/

void arbre_add_pere(arbre **a, int val)
{
    *a=malloc(sizeof(arbre));
    (*a)->node=val;
    (*a)->size=0;
    int i=FILS_MAX-1;
    while(i--)  (*a)->fils[i]=Nullptr(arbre);

}

void arbre_add_fils(arbre *l,int in,int fils)
{
    if(l->node==in)
    {
        printf("je suis la ajoute %d dans le pere %d a l'indice %d\n",fils,in,l->size);
        arbre_add_pere(&l->fils[l->size],fils);
        l->size++;
        return;
    }
    else
    {
        printf("je suis la MM\n");

        int i=l->size;
        while(i--)
            arbre_add_fils(l->fils[i],in, fils);

    }

}

/***************affichage *****************/
void arbre_affiche(arbre *l)
{
    printf("(%d,%d) ", l->node,l->size);

    int i=l->size;
    while(i--)      arbre_affiche(l->fils[i]);
    //printf("\n");
}




#endif // ARBRE_RBH
