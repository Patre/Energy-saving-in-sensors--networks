#ifndef STRUCT_LBIP_RBH
#define STRUCT_LBIP_RBH

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
  int firsttime;

  //packet REcus
  list_PACKET *paquets;

  //LBIP voisin
  arbre *tree_LBIP;     //l'arbre de lbip original
  arbre *tree_LBIP_re;     //l'arbre de lbip redirection

  //list pere LBIP
  list *source_packet;

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
#endif // NODEDATA_RBH
/* ************************************************** */
/* ************************************************** */
/* VARIABLE de L'APPLICATION LBIP */
struct entitydataLBIP {
  double    alpha;           //alpha de modele d'energie
  double    c;                 //le C de modele d'energie
  uint64_t  debut;           //l'instant de debut de l'application (detection de premier evenement
  uint64_t  periodEVE;       //delta temps entre chaque evenement
};

/* ************************************************** */
/* ************************************************** */
/*Paquet de LBIP*/
#ifndef PACKET_LBIP_RBH
#define PACKET_LBIP_RBH

struct _packet_lbip
{
    int     type;               //pour le type
    int     source;             //la source racine de paquet
    int     seq;                //la sequance de paquet
    int     redirected_by;      //le noeud qui a rediriger  le paquet

    /**************************************************/
    list    *destinations;            //les noeuds de destination de packet
    arbre   *pere_arbre;              //ARBRE of pere (redirected_by)


};
typedef struct _packet_lbip packet_LBIP;

#endif //PACKET LBIP RBH

#endif //STRUCT_LBIP_RBH