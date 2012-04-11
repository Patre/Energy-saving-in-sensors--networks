#ifndef PROTOCOLE_DEC_RBH
#define PROTOCOLE_DEC_RBH

#include <include/modelutils.h>
#include <listNodes.h>
#include <list_de_listNodes.h>
#include <list_paquet.h>
#include <arbre.h>
#include <conversion.h>
#include <graphe.h>

#define SHOW_GRAPH(x...)  { FILE *topo; topo=fopen("graphFLOOD","a+"); fprintf(topo,x); fclose(topo);}

//HELLO one HOP
#define HELLO       100             //le paquet hello pour calculer one Hop

//FLOOD
#define FLOOD   150                 //le paquet FLOOD

/* ************************************************** */
/* ************************************************** */
/*data des noeud dans l'application BIP*/




struct nodedata {
	int overhead;
	//packet REcus
        list_PACKET *paquets;
        //ajouter par rabah
        listeNodes *oneHopNeighbourhood;

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

struct _packet_flood
{
    int     type;  
    int     src;
    int     seq;                //la sequence de paquet
    int     redirected_by;      //le noeud qui a redirigé le paquet
    listeNodes    *destinations;

};
typedef struct _packet_flood packet_PROTOCOLE;

/* ************************************************** */
/* ************************************************** */
//PROPAGATION

struct propagation_data
{
        double range;
};


//MAC

struct mac_data {
    int debug;
    double range;        // Communication range (m)
    double bandwidth;    // Data bandwidth (KB/s)
};

#endif //PROTOCOLE DECLARATION
