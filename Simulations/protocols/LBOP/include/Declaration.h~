#ifndef BIP_DEC_RBH
#define BIP_DEC_RBH

#include <include/modelutils.h>
#include <listNodes.h>
#include <list_de_listNodes.h>
#include <list_paquet.h>
#include <arbre.h>
#include <graphe.h>

#define SHOW_GRAPH(x...)  { FILE *topo; topo=fopen("graphRBOP","a+"); fprintf(topo,x); fclose(topo);}
/* ************************************************** */
/* ************************************************** */
/*data des noeud dans l'application BIP*/




struct nodedata {
	int overhead;
	
	//packet REcus
	list_PACKET *paquets;
	

        //Integration a la version clhoé
        listeNodes* oneHopNeighbourhood;

	//PROTOCOLE
        list *RNG;
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
    int     type;  
    int     src;
    position_t src_pos;
    int     seq;                //la sequence de paquet
    int     redirected_by;      //le noeud qui a redirigé le paquet

    /**************************************************/
    list    *destinations;            //les noeuds de destination de packet
	
	
};
typedef struct _packet_bip packet_PROTOCOLE;

/* ************************************************** */
/* ************************************************** */
//PROPAGATION

struct propagation_data
{
        double range;
};

#endif //PROTOCOLE DECLARATION
