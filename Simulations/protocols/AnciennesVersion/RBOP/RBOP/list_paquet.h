#ifndef LIST_PAQUET_H
#define LIST_PAQUET_H

#define Nullptr(type) (type *)0

#ifndef PACKET_LIST_RBH
#define PACKET_LIST_RBH
struct _packet_
{
    int     source;             //la source de paquet
    int     seq;                //la sequance de paquet
    int     redirected_by;      //le noeud qui a rediriger  le paquet
};
typedef struct _packet_ packet_LIST_element;


//LA structure d'une list de packet
struct _list_paquet
{
    packet_LIST_element packet;
    struct _list_paquet *suiv;
};
typedef struct _list_paquet list_PACKET;

#endif //PACKET _RBH
/*****LES FONCTION DANS CE FICHIER*****/
void list_PACKET_insert(list_PACKET **l,packet_LIST_element val);
void list_PACKET_insert_tout(list_PACKET **l,int source,int seq, int redirected_by);
void list_PACKET_affiche(list_PACKET *l);
int list_PACKET_taille(list_PACKET *l);
void list_PACKET_detruire(list_PACKET **l);
int list_PACKET_recherche(list_PACKET *l,packet_LIST_element val);



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
        while(l && !bol)
        {
                if(l->packet.seq == seq && l->packet.source==source)	bol=1;
                l=l->suiv;
        }
        return bol;
}


#endif // LIST_PAQUET_LBIP_H
