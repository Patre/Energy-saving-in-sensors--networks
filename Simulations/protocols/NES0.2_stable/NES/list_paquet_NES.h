#ifndef LIST_PAQUET_NES_H
#define LIST_PAQUET_NES_H

#define Nullptr(type) (type *)0

#ifndef PACKET_NES_RBH
#define PACKET_NES_RBH
struct packet_nes
{
    int     type;               //pour le type
    int     source;             //la source de paquet
    int     seq;                //la sequance de paquet
    int     redirected_by;      //le noeud qui a rediriger  le paquet
    list    *N1;
};
typedef struct packet_nes packet_NES;

#endif //PACKET NES RBH


//LA structure d'une list de packet
struct _list_NES
{
    packet_NES packet;
    struct _list_NES *suiv;
};
typedef struct _list_NES list_NES;

/***********************AJOUTE**************************/
void list_NES_insert(list_NES **l,packet_NES val)
{
    list_NES *tmp=malloc(sizeof(list_NES));
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
void list_NES_affiche(list_NES *l)
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
int list_NES_taille(list_NES *l)
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
packet_NES list_NES_get(list_NES *l,int index)
{
        if(index>=list_NES_taille(l)) Nullptr(list_NES);
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
void list_NES_detruire(list_NES **l)
{
        list_NES *tmp=*l;
        while(*l)
        {
             tmp = (*l)->suiv;
             free(*l);
             *l = tmp;
        }
}
/***********************RECHERCHE**************************/
int list_NES_recherche(list_NES *l,packet_NES val)
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


#endif // LIST_PAQUET_NES_H
