
#include "BIP_tree_utils.h"


void computeBIPtree(call_t *c)
{
	struct nodedata *nodedata = get_node_private_data(c);
	
	int i, minNode, devientEmetteur;
	graphe* g = nodedata->g2hop;
	double coutIncremental;
	double* cle = malloc(g->nbSommets*sizeof(double));
	double* poids = malloc(g->nbSommets*sizeof(double));
	int* pere = malloc(g->nbSommets*sizeof(int));
	Heap* F = allocHeap(g->nbSommets, cle); // tas gardant les couts des noeuds pas encore dans l'arbre
	voisin *trans;
	
	
	for(i = 0 ; i < g->nbSommets ; i++)
	{
		cle[i] = DBL_MAX;
		poids[i] = 0;
		pere[i] = -1;
		h_insertNode(F, i);
	}
	h_changeLabel(F, g->s.num, 0);
	poids[g->s.num] = getEdgeCost(g, c->node, getNearestNeighbour(c));
	
	while(!h_isEmpty(F))
	{
		minNode = h_remNode(F);
		if(pere[minNode] == -1)
		{
			arbre_add_pere(&(nodedata->BIP_tree), g->sommets[minNode]);
		}
		else
		{
			arbre_add_fils(nodedata->BIP_tree, g->sommets[pere[minNode]], g->sommets[minNode]);
		}
		
		devientEmetteur = 0;
		// pour chacun des voisins de minNode dans le graphe
		trans = g->listeVoisins[minNode];
		while(trans != 0)
		{
			coutIncremental = trans->cout - poids[minNode];
			if(!(arbre_recherche(nodedata->BIP_tree, trans->vLabel)) && (coutIncremental < cle[getNumFromLabel(g,trans->vLabel)]))
			{
				h_changeLabel(F, getNumFromLabel(g,trans->vLabel), coutIncremental);
				if(poids[minNode] < trans->cout)
				{
					if(poids[minNode] == 0)
						devientEmetteur = 1;
					poids[minNode] = trans->cout;
				}
				pere[getNumFromLabel(g,trans->vLabel)] = minNode;
			}
			if(devientEmetteur)
			{
				devientEmetteur = 0;
				trans = g->listeVoisins[minNode];
			}
			else
				trans = trans->vSuiv;
		}
	}
	
	
	free(poids);
	free(pere);
	free(cle);
	freeHeap(F);
	
	printf("arbre de BIP de %d construit : \n", c->node);
	arbre_affiche(nodedata->BIP_tree);
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

double getCoutFromDistance(double distance, double alpha, double c)
{
    double par1;
	
    //la premier partie de la fonction
    par1=pow(distance,alpha);
	
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

int getNearestNeighbour(call_t *c)
{
	struct nodedata *nodedata = get_node_private_data(c);
	double coutMin = DBL_MAX;
	int minNode = -1;
	graphe* g = nodedata->g2hop;
	voisin *trans = g->listeVoisins[g->s.num];
	while(trans != 0)
	{
		if(trans->cout < coutMin)
		{
			coutMin = trans->cout;
			minNode = trans->vLabel;
		}
		trans = trans->vSuiv;
	}
	return minNode;
}

void setRelayNodes(call_t *c, listeNodes** askedToRedirect, listeNodes** needsToBeCovered)
{
	struct nodedata *nodedata = get_node_private_data(c);
	
	list *fils = 0, *fils2 = 0;
	double cout, coutMax = 0;
	int nodeToCover = -1;
	
	arbre_get_fils(&fils, nodedata->BIP_tree, c->node);
	while(fils != 0)
	{
		if(!arbre_is_leaf(nodedata->BIP_tree, fils->val))
		{
			arbre_get_fils(&fils2, nodedata->BIP_tree, fils->val);
			listeNodes_insert(askedToRedirect, fils->val);
			coutMax = 0;
			nodeToCover = -1;
			while(fils2 != 0)
			{
				cout = getEdgeCost(nodedata->g2hop, fils->val, fils2->val);
				if(cout > coutMax)
				{
					coutMax = cout;
					nodeToCover = fils2->val;
				}
				fils2 = fils2->suiv;
			}
			listeNodes_insert(needsToBeCovered, nodeToCover);
		}
		fils = fils->suiv;
	}
	
	printf("Relayer : \n");
	listeNodes_affiche(*askedToRedirect);
	listeNodes_affiche(*needsToBeCovered);
}

void purgeGraphe(call_t* c, int farestNode, int fromNode)
{
	struct nodedata *nodedata = get_node_private_data(c);
	int ret;
	double costMax = getEdgeCost(nodedata->g2hop, c->node, farestNode), cost;
	
	// on enleve du graphe tous les voisins directs de fromNode
	// sauf celui qu'on doit toucher et nous meme
	// on garde egalement les noeuds voisins de fromNode qui sont plus pres de nous que farestNode
	voisin** voisinsFromNode = &(nodedata->g2hop->listeVoisins[getNumFromLabel(nodedata->g2hop, fromNode)]);
	while(*voisinsFromNode != 0)
	{
		if(
		   ((*voisinsFromNode)->vLabel != c->node)
		   &&
		   (getEdgeCost(nodedata->g2hop, c->node, (*voisinsFromNode)->vLabel) > costMax)
		   )
		{
			ret = deleteVertex(nodedata->g2hop, (*voisinsFromNode)->vLabel);
			if(ret == -1)
				break;
			voisinsFromNode = &(nodedata->g2hop->listeVoisins[getNumFromLabel(nodedata->g2hop, fromNode)]);
		}
		else
			voisinsFromNode = &((*voisinsFromNode)->vSuiv);
	}

	
	// on enleve du graphe celui qui nous a relaye le paquet
	deleteVertex(nodedata->g2hop, fromNode);
	
	// on enleve du graphe les voisins directs plus loins que farestNode
	voisin** fils = &(nodedata->g2hop->listeVoisins[getNumFromLabel(nodedata->g2hop, c->node)]);
	
	while(*fils != 0)
	{
		cost = (*fils)->cout;
		if(cost	> costMax)
		{
			afficherGraphe(nodedata->g2hop);
			ret = deleteVertex(nodedata->g2hop, (*fils)->vLabel);
			if(ret == -1)
				break;
			fils = &(nodedata->g2hop->listeVoisins[getNumFromLabel(nodedata->g2hop, c->node)]);
		}
		else
			fils = &((*fils)->vSuiv);
	}
	
	// on purge le graphe
	purgeGrapheOfStables(nodedata->g2hop);
}


