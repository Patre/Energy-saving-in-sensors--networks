#ifndef BIP_DEC_RBH
#define BIP_DEC_RBH

#include <include/modelutils.h>
#include "structure/listNodes.h"
#include "structure/list_de_listNodes.h"
#include "structure/list_paquet.h"
#include "structure/arbre.h"

#define SHOW_GRAPH(x...)  { FILE *topo; topo=fopen("graphBIP","a+"); fprintf(topo,x); fclose(topo);}
/* ************************************************** */
/* ************************************************** */
/*data des noeud dans l'application BIP*/


//Functions


struct nodedata {
	int overhead;
	
	//packet REcus
	list_PACKET *paquets;
	
	//les voisin
	//1-hop
	listeNodes  *N1;
	
	//PROTOCOLE
	arbre *tree_BIP;     //l'arbre de bip original
	//l'ensemeble des connection de graph
	list2N *NodesV1;
	
	//ajouter par rabah
	int nbr_evenement;
};

/* ************************************************** */
/* ************************************************** */
/* VARIABLE de L'APPLICATION LBIP */
struct protocoleData {
	double    alpha;           //alpha de modele d'energie
	double    c;                 //le C de modele d'energie
	double    eps;              //la duree de ropos (ordonneunceur de l'initialisation)
	uint64_t  debut;           //l'instant de debut de l'application (detection de premier evenement
	uint64_t  periodEVE;       //delta temps entre chaque evenement
};

/* ************************************************** */
/* ************************************************** */

/*Paquet de BIP*/

struct _packet_bip
{
    int     type;               //pour le type
    int     seq;                //la sequence de paquet
    int     redirected_by;      //le noeud qui a redirigé le paquet
	nodeid_t dst;
    position_t dst_pos;
    nodeid_t src;
    position_t src_pos;
	
    /**************************************************/
    list    *destinations;            //les noeuds de destination de packet
    arbre   *pere_arbre;              //ARBRE du pere (redirected_by)
	
	
};
typedef struct _packet_bip packet_PROTOCOLE;


#endif //PROTOCOLE DECLARATION