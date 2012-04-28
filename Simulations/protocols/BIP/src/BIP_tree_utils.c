
#include "BIP_tree_utils.h"

void init_graphe(call_t *c)
{
    struct nodedata *nodedata = get_node_private_data(c);
    struct protocoleData *entitydata = get_entity_private_data(c);
	
	double cout;
    int i,j;
    for(i=0 ; i<get_node_count() ; i++)
        if(i!=c->node)
			addVertex(nodedata->g,i);
	
    double range = get_range_Tr(c);
	
    double dist;
    for(i=0 ; i<get_node_count() ; i++)
	{
        for(j=0 ; j<get_node_count() ; j++)
        {
            dist=distance(get_node_position(i),get_node_position(j));
            if(i != j && dist <= range)
            {
                cout = calcul_energie(*get_node_position(i),*get_node_position(j),entitydata->alpha,entitydata->c,&dist);
                addEdgeUndirected(nodedata->g, i, j, cout);
            }
        }
	}
	
	/*if(c->node == 0)
	{
		printf("Graphe : ");
		afficherGraphe(nodedata->g);
	}*/
}

void init_bip_tree(call_t *c, int noeudRacine)
{
    struct nodedata *nodedata = get_node_private_data(c);
    struct protocoleData *entitydata = get_entity_private_data(c);
	
	if(nodedata->BIP_tree != 0)
		arbre_detruire(&nodedata->BIP_tree);
	nodedata->BIP_tree = computeBIPtree(c, nodedata->g, noeudRacine, 0);
	/*printf("Arbre de BIP de %d : \n", c->node);
	arbre_affiche(nodedata->BIP_tree);*/
	
	setRangeToFarestNeighbour(c, nodedata->g, nodedata->BIP_tree);
	//printf("Range de %d : %.1lf\n", c->node, get_range_Tr(c));
}


arbre* computeBIPtree(call_t *c, graphe* g, int noeudRacine, int debug)
{
	struct nodedata *nodedata = get_node_private_data(c);
	arbre* bipTree = 0;
	int i, minNode, devientEmetteur, numMinNode, numVoisin;
	double coutIncremental;
	double* cle = malloc(g->nbSommets*sizeof(double));
	double* poids = malloc(g->nbSommets*sizeof(double));
	int* pere = malloc(g->nbSommets*sizeof(int));
	Heap* F = allocHeap(g->nbSommets, cle); // tas gardant les couts des noeuds pas encore dans l'arbre
	voisin *trans;
	
	if(debug)
		printf("Fixation des valeurs initiales...\n");
	for(i = 0 ; i < g->nbSommets ; i++)
	{
		cle[i] = DBL_MAX;
		poids[i] = 0;
		pere[i] = -1;
		h_insertNode(F, i);
	}
	h_changeLabel(F, getNumFromLabel(nodedata->g, noeudRacine), 0);
	
	poids[noeudRacine] = getEdgeCost(g, noeudRacine, getNearestNeighbour(c, g, noeudRacine));
	
	if(debug)
		printf("Debut de l'algorithme...\n");
	while(!h_isEmpty(F))
	{
		if(debug)
			printf("Recuperation du min du tas...\n");
		numMinNode = h_remNode(F);
		minNode = getLabelFromNum(g,numMinNode);
		if(debug)
		{
			printf("minNode label : %d\n", minNode);
			printf("\tnum : %d\n", numMinNode);
			printf("Ajout du noeud dans l'arbre...\n");
		}
		if(pere[numMinNode] == -1)
		{
			arbre_add_pere(&(bipTree), minNode);
		}
		else
		{
			arbre_add_fils(bipTree, pere[numMinNode], minNode);
		}
		devientEmetteur = 0;
		// pour chacun des voisins de minNode dans le graphe
		if(debug)
			printf("Recuperation du voisinage...\n");
		trans = getNeighboursFromLabel(g,minNode);
		while(trans != 0)
		{
			numVoisin = getNumFromLabel(g,trans->vLabel);
			if(debug)
				printf("\tVoisin label : %d ; num : %d\n", trans->vLabel, numVoisin);
			coutIncremental = trans->cout - poids[numMinNode];
			if(!(arbre_recherche(bipTree, trans->vLabel)) && (coutIncremental < cle[getNumFromLabel(g,trans->vLabel)]))
			{
				if(debug)
					printf("\tVoisin pas dans l'arbre et cout a ameliorer.\n");
				h_changeLabel(F, getNumFromLabel(g,trans->vLabel), coutIncremental);
				if(poids[numMinNode] < trans->cout)
				{
					if(debug)
						printf("\tAmelioration du poids de minNode\n");
					if(poids[numMinNode] == 0)
						devientEmetteur = 1;
					poids[numMinNode] = trans->cout;
				}
				if(debug)
					printf("\tpere[%d] devient %d...\n", trans->vLabel, minNode);
				pere[numVoisin] = minNode;
			}
			if(devientEmetteur)
			{
				if(debug)
					printf("\tOn repart au debut de la liste de voisins...\n");
				devientEmetteur = 0;
				trans = getNeighboursFromLabel(g,minNode);
			}
			else
			{
				trans = trans->vSuiv;
			}
		}
	}
	
	
	free(poids);
	free(pere);
	free(cle);
	freeHeap(F);
	
	/*printf("Graphe de voisinage complet : \n");
	 printf("Graphe de voisinage : \n");
	 afficherGraphe(nodedata->g2hop);
	 printf("arbre de BIP de %d construit : \n", c->node);
	 arbre_affiche(bipTree);*/
	
	return bipTree;
}

double calcul_energie(position_t A, position_t B, double alpha, double c, double* dist)
{
    double dAB;
    double par1;
	
    //Distance
	dAB = distance (&A, &B);
    /*dAB =sqrt(((double)A.x-(double)B.x)*((double)A.x-(double)B.x)+
              ((double)A.y-(double)B.y)*((double)A.y-(double)B.y)+
              ((double)A.z-(double)B.z)*((double)A.z-(double)B.z));*/
	if(dist != 0)
		*dist = dAB;
	
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

double setRangeToFarestNeighbour(call_t *c, graphe* g, arbre* bipTree)
{
	struct protocoleData *entitydata = get_entity_private_data(c);
	
	// calcul de la distance entre ce noeud et son voisin 1-hop (dans l'arbre de BIP) le plus eloigne
	list *fils = 0;
	double distMax = 0, dist;
	arbre_get_fils(&fils, bipTree, c->node);
	while(fils != 0)
	{
		dist = distance(get_node_position(c->node),get_node_position(fils->val));
		if(dist > distMax)
		{
			distMax = dist;
		}
		fils = fils->suiv;
	}
	
	
	// set le range du module propagation a la valeur desiree
	array_t *mac=get_mac_entities(c);
	call_t c0 = {mac->elts[0], c->node, c->entity};
	struct macnodedata* macdata = get_node_private_data(&c0);
	macdata->range = distMax;
	//printf("rayon d'emission de %d fixe a %lf\n", c->node, macdata->range);
	
	return distMax;
}

int getNearestNeighbour(call_t *c, graphe* g, int noeudRacine)
{
	struct nodedata *nodedata = get_node_private_data(c);
	double coutMin = DBL_MAX;
	int minNode = -1;
	voisin *trans = getNeighboursFromLabel(g, noeudRacine);
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

double get_range_Tr(call_t *c)
{
    array_t *mac=get_mac_entities(c);
    call_t c0 = {mac->elts[0], c->node, c->entity};
    struct macnodedata* macdata = get_node_private_data(&c0);
    return macdata->range;
}

void set_range_Tr(call_t *c,double range)
{
    array_t *mac=get_mac_entities(c);
    call_t c0 = {mac->elts[0], c->node, c->entity};
    struct macnodedata* macdata = get_node_private_data(&c0);
    macdata->range = range;
}


