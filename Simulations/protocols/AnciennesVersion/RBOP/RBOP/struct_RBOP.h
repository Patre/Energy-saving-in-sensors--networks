#ifndef STRUCT_RBOP_RBH
#define STRUCT_RBOP_RBH

#include <include/modelutils.h>
#include "../RBH/struct_RBH.h"
#include "../RBH/list.h"
#include "../RBH/list_de_list.h"
#include "../RBH/arbre.h"
#include "list_paquet.h"


/* ************************************************** */
/* ************************************************** */
#ifndef NODEDATA_RBH
#define NODEDATA_RBH
/*data des noeud dans l'application LBIP*/

struct nodedata {
  int *overhead;
  double eps;

  //packet REcus
  list_PACKET *paquets;


  //les voisin
  //1-hop
  list  *N1;

  //Arbre RNG
  list *RNG;

  /* pour les statistiques */
  int packet_tx;
  int packet_rx;

  //ajouter par rabah
  int nbr_evenement;
};
#endif // NODEDATA_RBH
/* ************************************************** */
/* ************************************************** */
/* VARIABLE de L'APPLICATION LBIP */
struct entitydataRBOP {
  double    alpha;           //alpha de modele d'energie
  double    c;                 //le C de modele d'energie
  uint64_t  debut;           //l'instant de debut de l'application (detection de premier evenement
  uint64_t  periodEVE;       //delta temps entre chaque evenement
};

/* ************************************************** */
/* ************************************************** */
/*Paquet de RBOP*/
#ifndef PACKET_RBOP_RBH
#define PACKET_RBOP_RBH

struct _packet_rbop
{
    int     type;               //pour le type
    int     source;             //la source racine de paquet
    int     seq;                //la sequance de paquet
    int     redirected_by;      //le noeud qui a rediriger  le paquet

    /**************************************************/
    list    *destinations;            //les noeuds de destination de packet

};
typedef struct _packet_rbop packet_RBOP;

#endif //PACKET RBOP RBH

#endif //STRUCT_RBOP_RBH
