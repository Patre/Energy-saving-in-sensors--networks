#ifndef DEF_GRAPHE
#define DEF_GRAPHE

typedef int sommet;

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
	sommet s;
} graphe;

#endif // DEF_GRAPHE
