#ifndef STRUCT_MPR_RBH
#define STRUCT_MPR_RBH

#include <include/modelutils.h>
#include "../RBH/struct_RBH.h"
#include "../RBH/list.h"
#include "../RBH/list_de_list.h"
#include "list_paquet_MPR.h"


/* ************************************************** */
/* ************************************************** */
/*data des noeud dans l'application MPR*/
struct nodedata {
  int *overhead;
  double eps;
  int first_time;

  //packet REcus
  list_MPR *paquets;

  //MPR voisin
  list *Voi_MPR;

  //les voisin
  //1-hop
  list  *N1;
  list2 *N2;

  /* pour les statistiques */
  int packet_tx;
  int packet_rx;

  //ajouter par rabah
  int nbr_evenement;
};

/* ************************************************** */
/* ************************************************** */
/* VARIABLE de L'APPLICATION MPR */
struct entitydataMPR {
  uint64_t timeoutMax;      //timeOut Max de MPR
  uint64_t timeoutMin;      //timeOut Min de MPR
  uint64_t debut;           //l'instant de debut de l'application (detection de premier evenement
  uint64_t periodEVE;       //delta temps entre chaque evenement
};

/* ************************************************** */
/* ************************************************** */
/*Paquet de MPR*/
#ifndef PACKET_MPR_RBH
#define PACKET_MPR_RBH  ///IMPORTANT:  SI MODIFIER il faut MODIFIER aussi dans list_paquet_MPR.h

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

#endif //STRUCT_MPR_RBH
