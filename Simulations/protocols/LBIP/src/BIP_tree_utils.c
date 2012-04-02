
#include "BIP_tree_utils.h"


void computeBIPtree(call_t *c)
{
	struct nodedata *nodedata = get_node_private_data(c);
	struct protocoleData *entitydata = get_entity_private_data(c);
	
	int i, minNode;
	graphe* g = copieGraphe(nodedata->g2hop);
	double coutMax = 0;
	double* cle = malloc(g->nbSommets*sizeof(double));
	double* poids = malloc(g->nbSommets*sizeof(double));
	int* pere = malloc(g->nbSommets*sizeof(int));
	Heap* F = allocHeap(g->nbSommets, cle);
	voisin *trans, *trans2;
	
	for(i = 0 ; i < g->nbSommets ; i++)
	{
		cle[i] = DBL_MAX;
		poids[i] = DBL_MAX;
		pere[i] = -1;
		h_insertNode(F, i);
	}
	cle[g->s.num] = 0;
	
	arbre_add_pere(&(nodedata->BIP_tree), g->s.label);
	
	
	while(!h_isEmpty(F))
	{
		// mettre a jour les poids des sommets
		for(i = 0 ; i < g->nbSommets ; i++)
		{
			coutMax = 0;
			if(pere[i] != -1 || i == g->s.num) // le sommet est dans le bip tree
			{
				trans = nodedata->g2hop->listeVoisins[i];
				while(trans != 0)
				{
					if(pere[trans->v.num] != -1 || trans->v.num == g->s.num) // le voisin est dans le bip tree
					{
						if(trans->cout > coutMax)
							coutMax = trans->cout;
					}
					trans = trans->vSuiv;
				}
				poids[i] = coutMax;
			}
		}
		
		// mettre a jour les couts des aretes
		for(i = 0 ; i < g->nbSommets ; i++)
		{
			if(pere[i] != -1 || i == g->s.num) // le sommet est dans le bip tree
			{
				trans = g->listeVoisins[i];
				trans2 = nodedata->g2hop->listeVoisins[i];
				while(trans != 0 && trans2 != 0)
				{
					if(pere[trans->v.num] == -1 && trans->v.num != g->s.num) // le voisin n'est pas dans le bip tree
					{
						if(trans->cout != trans2->cout - poids[i])
						{
							changeUndirectedEdgeCost(g, g->sommets[i], trans->v.label, trans2->cout - poids[i]);
						}
					}
					trans = trans->vSuiv;
					trans2 = trans2->vSuiv;
				}
			}
		}
		
		minNode = h_remNode(F);
		
		// pour chacun des voisins de minNode dans le graphe
		trans = g->listeVoisins[minNode];
		while(trans != 0)
		{
			if((pere[trans->v.num] == -1 && trans->v.num != g->s.num) && trans->cout < cle[trans->v.num])
			{
				pere[trans->v.num] = minNode;
				arbre_add_fils(nodedata->BIP_tree, g->sommets[minNode], trans->v.label);
				if(cle[trans->v.num] != trans->cout)
				{
					h_changeLabel(F, trans->v.num, trans->cout);
				}
			}
			trans = trans->vSuiv;
		}
	}
	
	
	
	deleteGraphe(g);
	free(g);
	free(poids);
	free(pere);
	free(cle);
	freeHeap(F);
	
	printf("\tarbre de BIP de %d construit : \n", c->node);
	arbre_affiche(nodedata->BIP_tree);
}

void setRelayNodes(listeNodes** askedToRedirect, listeNodes** needsToBeCovered)
{
	
}

double calcul_energie(position_t A, position_t B, double alpha, double c, double* distance)
{
    double dAB;
    double par1;
	
    //Distance
    dAB =sqrt(((double)A.x-(double)B.x)*((double)A.x-(double)B.x)+
              ((double)A.y-(double)B.y)*((double)A.y-(double)B.y)+
              ((double)A.z-(double)B.z)*((double)A.z-(double)B.z));
	if(distance != 0)
		*distance = dAB;
	
    //la premier partie de la fonction
    par1=pow(dAB,alpha);
	
    return par1+c;
}

double getDistanceFromCout(double cout, double alpha, double c)
{
	double res = cout - c;
	res = pow(res, 1.0/alpha);
	return res;
}

double setRangeToFarestNeighbour(call_t *c)
{
	struct nodedata *nodedata = get_node_private_data(c);
	struct protocoleData *entitydata = get_entity_private_data(c);
	
	// calcul de la distance entre ce noeud et son voisin 1-hop (dans l'arbre de BIP) le plus eloigne
	list *fils = 0;
	double distMax = 0, dist;
	arbre_get_fils(&fils, nodedata->BIP_tree, c->node);
	while(fils != 0)
	{
		dist = getEdgeCost(nodedata->g2hop, c->node, fils->val);
		dist = getDistanceFromCout(dist, entitydata->alpha, entitydata->c);
		if(dist > distMax)
		{
			distMax = dist;
		}
		fils = fils->suiv;
	}
	
	
	nodedata->radius = ceil(distMax);
	
	// set le range du module propagation a la valeur desiree
	call_t c0 = {0, c->node, c->entity};
	free(get_entity_private_data(&c0));
	struct propagation_data
	{
		double range;
	};
	struct propagation_data* propdata = malloc(sizeof(struct propagation_data));
	propdata->range = ceil(distMax);
	set_entity_private_data(&c0, propdata);
	printf("rayon d'emission de %d fixe a %lf\n", c->node, nodedata->radius);
	
	return distMax;
}


