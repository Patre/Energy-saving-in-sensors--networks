
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

graphe* copieGraphe(graphe* g)
{
	graphe* g2 = malloc(sizeof(graphe));
	int i;
	voisin *trans1, **trans2;
	
	g2->nbSommets = g->nbSommets;
	g2->listeVoisins = malloc(g2->nbSommets*sizeof(voisin*));
	g2->sommets = malloc(g2->nbSommets*sizeof(int));
	g2->s = g->s;
	
	for(i = 0 ; i < g->nbSommets ; i++)
	{
		g2->sommets[i] = g->sommets[i];
		trans1 = g->listeVoisins[i];
		trans2 = &(g2->listeVoisins[i]);
		
		while(trans1 != 0)
		{
			*trans2 = malloc(sizeof(voisin));
			(*trans2)->v = trans1->v;
			(*trans2)->cout = trans1->cout;
			(*trans2)->vSuiv = 0;
			trans2 = &((*trans2)->vSuiv);
			
			trans1 = trans1->vSuiv;
		}
		*trans2 = 0;
	}
	return g2;
}

voisin* getNeighboursFromLabel(graphe* g, int label)
{
	return g->listeVoisins[getNumFromLabel(g, label)];
}

void changeDirectedEdgeCost(graphe* g, int labelU, int labelV, double cost)
{
	sommet u;
	u.num = getNumFromLabel(g, labelU);
	u.label = labelU;
	voisin* trans = g->listeVoisins[u.num];
	while(trans != 0)
	{
		if(trans->v.label == labelV)
		{
			trans->cout = cost;
			break;
		}
		trans = trans->vSuiv;
	}	
}

void changeUndirectedEdgeCost(graphe* g, int labelU, int labelV, double cost)
{
	changeDirectedEdgeCost(g, labelU, labelV, cost);
	changeDirectedEdgeCost(g, labelV, labelU, cost);
}


double getEdgeCost(graphe* g, int labelU, int labelV)
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
			return (*tmp)->cout;
		}
		
		tmp = &((*tmp)->vSuiv);
	}
	return DBL_MAX;
}


