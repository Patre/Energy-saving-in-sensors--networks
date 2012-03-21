#ifndef INCLUDE_RBH
#define INCLUDE_RBH

#include <include/modelutils.h>
#include "structure/list.h"
#include "structure/list_connected.h"
#include "structure/list_de_list.h"
#include "structure/list_paquet.h"
#include "structure/time_wsnet.h"

#include "structure/arbre.h"

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

//BIP
#define BIP    900

/* ************************************************** */
/* ************************************************** */
/*Paquet General*/
struct packet_general
{
    int type;
};

/* Paquet HELLO */
struct packet_hello {
  int       type;
  int       source;
};

/*Paquet HELLO2*/
struct packet_hello2 {
  int       type;
  int       source;
  list      *N1;
};


#endif //STRUCT_RBH
