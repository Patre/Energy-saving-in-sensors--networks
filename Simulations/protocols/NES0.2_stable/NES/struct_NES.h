#ifndef STRUCT_NES_RBH
#define STRUCT_NES_RBH

#include <include/modelutils.h>
#include "../RBH/struct_RBH.h"
#include "list_paquet_NES.h"
#include "../RBH/list.h"


/* ************************************************** */
/* ************************************************** */
/*data des noeud dans l'application NES*/
struct nodedata {
  int *overhead;
  double eps;
  int first_time;

  //packet REcus
  list_NES *paquets;

  //NES voisin
  list *Voi_NES;

  //les voisin
  //1-hop
  list  *N1;

  /* pour les statistiques */
  int packet_tx;
  int packet_rx;

  //ajouter par rabah
  int nbr_evenement;
};

/* ************************************************** */
/* ************************************************** */
/* VARIABLE de L'APPLICATION NES */
struct entitydataNES {
  uint64_t timeoutMax;      //timeOut Max de NES
  uint64_t timeoutMin;      //timeOut Min de NES
  uint64_t debut;           //l'instant de debut de l'application (detection de premier evenement
  uint64_t periodEVE;       //delta temps entre chaque evenement
};

/* ************************************************** */
/* ************************************************** */
/*Paquet de NES*/
#ifndef PACKET_NES_RBH
#define PACKET_NES_RBH  ///IMPORTANT:  SI MODIFIER il faut MODIFIER aussi dans list_paquet_NES.h

struct packet_nes
{
    int     type;               //pour le type
    int     source;             //la source de paquet
    int     seq;                //la sequance de paquet
    int     redirected_by;      //le noeud qui a rediriger  le paquet
    list    *N1;

};

#endif //PACKET NES RBH

#endif //STRUCT_NES_RBH
