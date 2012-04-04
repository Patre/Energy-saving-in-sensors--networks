#ifndef DEF_GRAPHE
#define DEF_GRAPHE

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

typedef struct sommet
{
	int num;
	int label;
} sommet;

typedef struct voisin
{
	int vLabel;
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
int deleteVertex(graphe* g, int labelSrc);
void purgeGrapheOfStables(graphe* g);
void addEdgeDirected(graphe* g, int labelU, int labelV, double cout);
void addEdgeUndirected(graphe* g, int labelU, int labelV, double cout);
void deleteEdgeDirected(graphe* g, int labelU, int labelV);
void deleteEdgeUndirected(graphe* g, int labelU, int labelV);
int getNumFromLabel(graphe* g, int label);
int getLabelFromNum(graphe* g, int num);
void afficherListeVoisins(graphe* g, voisin* liste);
void afficherGraphe(graphe* g);
graphe* copieGraphe(graphe* g);
voisin* getNeighboursFromLabel(graphe* g, int label);
void changeDirectedEdgeCost(graphe* g, int labelU, int labelV, double cost);
void changeUndirectedEdgeCost(graphe* g, int labelU, int labelV, double cost);
double getEdgeCost(graphe* g, int labelU, int labelV);


#endif // DEF_GRAPHE
