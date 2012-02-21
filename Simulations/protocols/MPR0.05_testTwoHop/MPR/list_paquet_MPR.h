#ifndef LIST_PAQUET_MPR_H
#define LIST_PAQUET_MPR_H

#define Nullptr(type) (type *)0

#ifndef PACKET_MPR_RBH
#define PACKET_MPR_RBH
struct packet_mpr
{
    int     type;               //pour le type
    int     source;             //la source de paquet
    int     seq;                //la sequance de paquet
    int     redirected_by;      //le noeud qui a rediriger  le paquet
    list    *N1;
};
typedef struct packet_mpr packet_MPR;

#endif //PACKET MPR RBH

//LA structure d'une list de packet
struct _list_MPR
{
    packet_MPR packet;
    struct _list_MPR *suiv;
};
typedef struct _list_MPR list_MPR;

/***********************AJOUTE**************************/
void list_MPR_insert(list_MPR **l,packet_MPR val)
{
    list_MPR *tmp=malloc(sizeof(list_MPR));
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
void list_MPR_affiche(list_MPR *l)
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
int list_MPR_taille(list_MPR *l)
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
packet_MPR list_MPR_get(list_MPR *l,int index)
{
        if(index>=list_MPR_taille(l)||index<0) Nullptr(list_MPR);
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
void list_MPR_detruire(list_MPR **l)
{
        list_MPR *tmp=*l;
        while(*l)
        {
             tmp = (*l)->suiv;
             free(*l);
             *l = tmp;
        }
}
/***********************RECHERCHE**************************/
int list_MPR_recherche(list_MPR *l,packet_MPR val)
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


#endif // LIST_PAQUET_MPR_H
