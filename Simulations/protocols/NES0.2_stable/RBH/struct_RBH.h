#ifndef STRUCT_RBH
#define STRUCT_RBH

#include <include/modelutils.h>
#include "list.h"

#define true 1
#define false 0

#define DEBUG   //l'instrection qui suit est utiliser pour le debug
#define N 50    //Nobre de Voisin


//HELLO
#define HELLO       100             //le paquet hello
#define REP_HELLO   101             //la reponse de paquet hello

//NES
#define NES     200                 //le paquet NES
#define RE_NES  201                 //le paquet RED



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


#endif //STRUCT_RBH
