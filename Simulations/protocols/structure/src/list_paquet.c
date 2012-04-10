#include "list_paquet.h"

/***********************AJOUTE**************************/
void list_PACKET_insert(list_PACKET **l,packet_LIST_element val)
{
    list_PACKET *tmp=malloc(sizeof(list_PACKET));
    if(!tmp)
    {
        printf("erreur de creation de la liste\n");
        exit(-5);
    }


    tmp->packet.seq=val.seq;
    tmp->packet.source=val.source;
    tmp->packet.redirected_by=val.redirected_by;

    tmp->suiv=*l;
    *l=tmp;

}
////////////ajoute d'un packet
void list_PACKET_insert_tout(list_PACKET **l,int source,int seq, int redirected_by)
{
    list_PACKET *tmp=malloc(sizeof(list_PACKET));
    if(!tmp)
    {
        printf("erreur de creation de la liste\n");
        exit(-5);
    }


    tmp->packet.seq=seq;
    tmp->packet.source=source;
    tmp->packet.redirected_by=redirected_by;

    tmp->suiv=*l;
    *l=tmp;

}

/***********************AFFICHAGE**************************/
void list_PACKET_affiche(list_PACKET *l)
{
    int x=list_PACKET_taille(l);
    printf("{");
    while(l)
    {
        printf("(%d,%d)",l->packet.source,
               l->packet.seq);

        l=l->suiv;
    }
    printf("}(totale: %d)\n",x);
}

/***********************TAILLE**************************/
int list_PACKET_taille(list_PACKET *l)
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
void list_PACKET_detruire(list_PACKET **l)
{
        list_PACKET *tmp=*l;
        while(*l)
        {
             tmp = (*l)->suiv;
             free(*l);
             *l = tmp;
        }
}
/***********************RECHERCHE**************************/
int list_PACKET_recherche(list_PACKET *l,packet_LIST_element val)
{
        int bol=0;
        while(l&&!bol)
        {
                if(l->packet.seq == val.seq &&
                        l->packet.source==val.source)	bol=1;
                l=l->suiv;
        }
        return bol;
}
//****Recherche avec tout les parmatres
int list_PACKET_recherche_tout(list_PACKET *l,int source,int seq)
{
        int bol=0;
        while(l)
        {
                if(l->packet.source == source &&
                        l->packet.seq==seq)	bol=1;
                l=l->suiv;
        }
        return bol;
}


