#ifndef INCLUDE_RBH
#define INCLUDE_RBH


#define true 1
#define false 0

#define DEBUG   //l'instrection qui suit est utiliser pour le debug


//HELLO one HOP
#define HELLO       100             //le paquet hello pour calculer one Hop
#define REP_HELLO   101             //la reponse de paquet hello

//HELLO two HOP
#define HELLO2       110            //le paquet hello pour calculer Two Hop
#define REP_HELLO2   111            //la reponse de paquet hello


//NES
#define NES     200                 //le paquet NES

//MPR
#define MPR     300                 //le paquet MPR

//RBOP
#define RBOP   400                 //le paquet RBOP

//BIP
#define BIP    900

#define APP       1000  

/* ************************************************** */
/* ************************************************** */

struct _elmt
{
    int node;
    double x;
    double y;
    double z;
};

typedef struct _elmt element;

/*Paquet General*/
struct packet_general
{
    int type;
};




#endif //STRUCT_RBH
