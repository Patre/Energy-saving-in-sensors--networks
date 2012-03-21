#include "structure/conversion.h"


void list_to_listC(listC **l,list *l2,int node)
{
    while(l2)
    {
        if(!list_con_recherche(*l,node,l2->val)) list_con_insert(l,node,l2->val,0);
        l2=l2->suiv;
    }
}

void listC_to_list(list **l,listC *c)
{
    while(c)
    {
        if(!list_recherche(*l,c->node1)) list_insert(l,c->node1);
        if(!list_recherche(*l,c->node2)) list_insert(l,c->node2);
        c=c->suiv;
    }
}

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


