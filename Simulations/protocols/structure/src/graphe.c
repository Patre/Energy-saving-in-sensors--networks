
#include "graphe.h"


void initGraphe(graphe* g, int labelSrc)
{
	g->nbSommets = 0;
	g->listeVoisins = 0;
	g->sommets = 0;
	g->s.num = 0;
	g->s.label = labelSrc;
	addVertex(g, labelSrc);
}

void deleteGraphe(graphe* g)
{
	int i;
	
	for(i = 0 ; i < g->nbSommets ; i++)
	{
		if(g->listeVoisins[i] != 0)
			free(g->listeVoisins[i]);
	}
	free(g->sommets);
	free(g->listeVoisins);
}

void addVertex(graphe* g, int labelSrc)
{
	if(getNumFromLabel(g, labelSrc) == -1)
	{
		g->nbSommets++;
		g->listeVoisins = realloc(g->listeVoisins, g->nbSommets*sizeof(voisin*));
		g->listeVoisins[g->nbSommets-1] = 0;
		g->sommets = realloc(g->sommets, g->nbSommets*sizeof(int));
		g->sommets[g->nbSommets-1] = labelSrc;
	}
}

void addEdgeDirected(graphe* g, int labelU, int labelV, double cout)
{
	sommet u, v;
	u.num = getNumFromLabel(g, labelU);
	v.num = getNumFromLabel(g, labelV);
	u.label = labelU;
	v.label = labelV;
	if(u.num >= g->nbSommets || v.num >= g->nbSommets)
	{
		fprintf(stderr, "Numeros de sommets invalides %d %d.\n", u.label, v.label);
		return;
	}
	
	voisin** tmp = &(g->listeVoisins[u.num]);
	while(*tmp != 0)
	{
		if((*tmp)->v.num == v.num)
		{
			//fprintf(stderr, "Arete (%d,%d) remplacee.\n", u.label, v.label);
			(*tmp)->cout = cout;
			break;
		}
		
		tmp = &((*tmp)->vSuiv);
	}
	if((*tmp) == 0)
	{
		(*tmp) = malloc(sizeof(voisin));
		(*tmp)->v = v;
		(*tmp)->cout = cout;
		(*tmp)->vSuiv = 0;
	}
}

void addEdgeUndirected(graphe* g, int labelU, int labelV, double cout)
{
	// ajouter v aux voisins de u
	addEdgeDirected(g, labelU, labelV, cout);
	
	// ajouter u aux voisins de v
	addEdgeDirected(g, labelV, labelU, cout);
}

int getNumFromLabel(graphe* g, int label)
{
	int i, num = -1;
	for(i = 0 ; i < g->nbSommets ; i++)
	{
		if(g->sommets[i] == label)
		{
			num = i;
			break;
		}
	}
	return num;
}

void afficherListeVoisins(voisin* liste)
{
	voisin*	trans = liste;
	while(trans != 0)
	{
		printf("{%d ; %.1lf} ", trans->v.label, trans->cout);
		trans = trans->vSuiv;
	}
	printf("\n");
}

void afficherGraphe(graphe* g)
{
	int i;
	
	if(g->nbSommets == 0)
		printf("\tvide\n");
	for(i = 0 ; i < g->nbSommets ; i++)
	{
		printf("\tvoisins du sommet %d : ", g->sommets[i]);
		afficherListeVoisins(g->listeVoisins[i]);
	}
}


