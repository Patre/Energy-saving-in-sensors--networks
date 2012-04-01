#ifndef DEF_GRAPHE
#define DEF_GRAPHE

#include <stdio.h>
#include <stdlib.h>

typedef struct sommet
{
	int num;
	int label;
} sommet;

typedef struct voisin
{
	sommet v;
	double cout;
	struct voisin * vSuiv;
} voisin;

typedef struct graphe
{
	int nbSommets;
	voisin** listeVoisins;
	int* sommets;
	sommet s;
} graphe;



void initGraphe(graphe* g, int labelSrc);
void deleteGraphe(graphe* g);
void addVertex(graphe* g, int labelSrc);
void addEdgeDirected(graphe* g, int labelU, int labelV, double cout);
void addEdgeUndirected(graphe* g, int labelU, int labelV, double cout);
int getNumFromLabel(graphe* g, int label);
void afficherListeVoisins(voisin* liste);
void afficherGraphe(graphe* g);
graphe* copieGraphe(graphe* g);
voisin* getNeighboursFromLabel(graphe* g, int label);
void changeDirectedEdgeCost(graphe* g, int labelU, int labelV, double cost);
void changeUndirectedEdgeCost(graphe* g, int labelU, int labelV, double cost);


#endif // DEF_GRAPHE
