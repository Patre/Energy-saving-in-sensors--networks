#ifndef LIST_PAQUET_LBIP_H
#define LIST_PAQUET_LBIP_H

#define Nullptr(type) (type *)0

#ifndef PACKET_LBIP_RBH
#define PACKET_LBIP_RBH
struct _packet_lbip
{
    int     type;               //pour le type
    int     source;             //la source de paquet
    int     seq;                //la sequance de paquet
    int     redirected_by;      //le noeud qui a rediriger  le paquet

    list    *N1;
};
typedef struct _packet_lbip packet_LBIP;

#endif //PACKET LBIP_RBH

//LA structure d'une list de packet
struct _list_lbip
{
    packet_LBIP packet;
    struct _list_LBIP *suiv;
};
typedef struct _list_lbip list_LBIP;

/***********************AJOUTE**************************/
void list_LBIP_insert(list_LBIP **l,packet_LBIP val)
{
    list_LBIP *tmp=malloc(sizeof(list_LBIP));
    if(!tmp)
    {
        printf("erreur de creation de la liste\n");
        exit(-5);
    }


    tmp->packet.type=val.type;
    tmp->packet.seq=val.seq;
    tmp->packet.source=val.source;
    tmp->packet.redirected_by=val.redirected_by;

    tmp->suiv=*l;
    *l=tmp;

}

/***********************AFFICHAGE**************************/
void list_LBIP_affiche(list_LBIP *l)
{
    while(l)
    {
        printf("PACKET: T:%d\t S:%d\t Se%d\t R%d\t\n ",l->packet.type,
               l->packet.source,
               l->packet.seq,
               l->packet.redirected_by);

        l=l->suiv;
    }
    printf("*************************************\n");
}

/***********************TAILLE**************************/
int list_LBIP_taille(list_LBIP *l)
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
packet_LBIP list_LBIP_get(list_LBIP *l,int index)
{
        if(index>=list_LBIP_taille(l)||index<0) Nullptr(list_LBIP);
        else
        {
                while(index)
                {
                        l=l->suiv;
                        index--;
                }

                return l->packet;
        }
}

/***********************DESTRUCTION**************************/
void list_LBIP_detruire(list_LBIP **l)
{
        list_LBIP *tmp=*l;
        while(*l)
        {
             tmp = (*l)->suiv;
             free(*l);
             *l = tmp;
        }
}
/***********************RECHERCHE**************************/
int list_LBIP_recherche(list_LBIP *l,packet_LBIP val)
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


#endif // LIST_PAQUET_LBIP_H
