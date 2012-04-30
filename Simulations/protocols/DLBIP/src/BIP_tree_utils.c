
#include "BIP_tree_utils.h"


arbre* computeBIPtree(call_t *c, graphe* g, listeNodes* askedToRedirect, listeNodes* needsToBeCovered, int debug)
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
	h_changeLabel(F, g->s.num, 0);
	if(askedToRedirect == 0 || needsToBeCovered == 0)
	{
		poids[g->s.num] = getEdgeCost(g, c->node, getNearestNeighbour(c, g));
	}
	else
	{
		if(debug)
			printf("Fixation des valeurs de range deduites du paquet...\n");
		listeNodes *tmp = askedToRedirect, *tmp2 = needsToBeCovered;
		while(tmp != 0)
		{
			if(getEdgeCost(g, tmp->values.node, tmp2->values.node) != DBL_MAX)
			{
				//printf("poids[%d] = %.1lf\n", tmp->values.node, getEdgeCost(g, tmp->values.node, tmp2->values.node));
				poids[getNumFromLabel(g,tmp->values.node)] = getEdgeCost(g, tmp->values.node, tmp2->values.node);
			}
			tmp = tmp->suiv;
			tmp2 = tmp2->suiv;
		}
	}
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
			int numPere = getNumFromLabel(g, pere[numMinNode]);
			if(poids[numPere] < getEdgeCost(g, pere[numMinNode], minNode))
			{
				if(debug)
					printf("\tChangement du poids de %d\n", pere[numMinNode]);
				poids[numPere] = getEdgeCost(g,pere[numMinNode], minNode);
			}
		}
		// pour chacun des noeuds pas dans l'arbre ie : dans le tas
		for(i = 0 ; i < g->nbSommets ; i++)
		{
			if(h_contains(F, i))
				h_changeLabel(F, i, DBL_MAX);
		}
		
		// pour chacun des noeuds dans l'arbre ie : pas dans le tas
		for(i = 0 ; i < g->nbSommets ; i++)
		{
			if(!h_contains(F, i))
			{
				// pour chacun des voisins dans le tas ie : pas dans l'arbre
				if(debug)
					printf("Recuperation du voisinage...\n");
				trans = getNeighboursFromLabel(g,getLabelFromNum(g,i));
				while(trans != 0)
				{
					numVoisin = getNumFromLabel(g,trans->vLabel);
					if(h_contains(F, numVoisin))
					{
						if(debug)
							printf("\tVoisin pas dans l'arbre : label : %d ; num : %d\n", trans->vLabel, numVoisin);
						coutIncremental = trans->cout - poids[i];
						if(coutIncremental < cle[numVoisin])
						{
							if(debug)
							{
								printf("\tCout a ameliorer : %.1lf < %.1lf\n", coutIncremental, cle[numVoisin]);
							}
							h_changeLabel(F, numVoisin, coutIncremental);
							if(debug)
								printf("\tpere[%d] devient %d...\n", trans->vLabel, getLabelFromNum(g,i));
							pere[numVoisin] = getLabelFromNum(g,i);
						}
					}
					trans = trans->vSuiv;
				}
			}
		}
	}
	
	
	free(poids);
	free(pere);
	free(cle);
	freeHeap(F);
	if(debug)
	{
		printf("arbre de BIP de %d construit : \n", c->node);
		arbre_affiche(bipTree);
		printf("Graphe de voisinage : \n");
		afficherGraphe(nodedata->g2hop);
		printf("Sortie de computeBipTree\n");
	}
	
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
	struct nodedata *nodedata = get_node_private_data(c);
	struct protocoleData *entitydata = get_entity_private_data(c);
	
	// calcul de la distance entre ce noeud et son voisin 1-hop (dans l'arbre de BIP) le plus eloigne
	list *fils = 0;
	double distMax = 0, dist;
	position_t pos;
	arbre_get_fils(&fils, bipTree, c->node);
	while(fils != 0)
	{
		pos = listeNodes_getPos(nodedata->oneHopNeighbourhood, fils->val);
		dist = distance(get_node_position(c->node), &pos);
		if(dist > distMax)
		{
			distMax = dist;
		}
		fils = fils->suiv;
	}
	
	
	// set le range du module propagation a la valeur desiree
	set_range_Tr(c, distMax);
	//printf("rayon d'emission de %d fixe a %lf\n", c->node, macdata->range);
	
	return distMax;
}

double getRange(call_t *c)
{
	struct macnodedata {
		void *buffer;
		double range;
#ifdef ONE_PACKET_AT_A_TIME
		int scheduler;
#endif
	};
	
	// set le range du module propagation a la valeur desiree
	array_t *mac=get_mac_entities(c);
	call_t c0 = {mac->elts[0], c->node, c->entity};
	struct macnodedata* macdata = get_node_private_data(&c0);
	return macdata->range;
}

int getNearestNeighbour(call_t *c, graphe* g)
{
	struct nodedata *nodedata = get_node_private_data(c);
	double coutMin = DBL_MAX;
	int minNode = -1;
	voisin *trans = getNeighboursFromLabel(g, g->s.label);
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

void setRelayNodes(call_t *c, graphe* g, arbre* bipTree, listeNodes** askedToRedirect, listeNodes** needsToBeCovered, int node)
{
	struct nodedata *nodedata = get_node_private_data(c);
	
	list *fils = 0, *fils2 = 0;
	double cout, coutMax = 0;
	int nodeToCover = -1;
	position_t pos;
	
	arbre_get_fils(&fils, bipTree, node);
	while(fils != 0)
	{
		if(!arbre_is_leaf(bipTree, fils->val))
		{
			arbre_get_fils(&fils2, bipTree, fils->val);
			pos = listeNodes_getPos(nodedata->oneHopNeighbourhood, fils->val);
			if(pos.x == -1 && pos.y == -1 && pos.z == -1)
				pos = listeNodes_getPos(nodedata->twoHopNeighbourhood, fils->val);
			listeNodes_insert_values(askedToRedirect, fils->val, pos.x, pos.y, pos.z);
			coutMax = 0;
			nodeToCover = -1;
			while(fils2 != 0)
			{
				cout = getEdgeCost(g, fils->val, fils2->val);
				if(cout > coutMax)
				{
					coutMax = cout;
					nodeToCover = fils2->val;
				}
				fils2 = fils2->suiv;
			}
			nodedata->energiesRem[fils->val] -= coutMax;
			pos = listeNodes_getPos(nodedata->oneHopNeighbourhood, nodeToCover);
			if(pos.x == -1 && pos.y == -1 && pos.z == -1)
				pos = listeNodes_getPos(nodedata->twoHopNeighbourhood, nodeToCover);
			listeNodes_insert_values(needsToBeCovered, nodeToCover, pos.x, pos.y, pos.z);
			setRelayNodes(c, g, bipTree, askedToRedirect, needsToBeCovered, fils->val);
		}
		fils = fils->suiv;
	}
	
	/*printf("Relayer : \n");
	listeNodes_affiche(*askedToRedirect);
	listeNodes_affiche(*needsToBeCovered);*/
}

graphe* purgeGraphe(call_t* c, int farestNode, int fromNode, int predNode)
{
	struct nodedata *nodedata = get_node_private_data(c);
	int ret;
	graphe* g = copieGraphe(nodedata->g2hop);
	double costMax = getEdgeCost(g, c->node, farestNode), cost;
	
	// on enleve du graphe tous les voisins directs de fromNode
	// sauf celui qu'on doit toucher et nous meme
	// on garde egalement les noeuds voisins de fromNode qui sont plus pres de nous que farestNode
	
	voisin* voisinsFromNode = getNeighboursFromLabel(g, fromNode);
	while(voisinsFromNode != 0)
	{
		if(
		   (voisinsFromNode->vLabel != c->node)
		   &&
		   (getEdgeCost(g, c->node, voisinsFromNode->vLabel) > costMax)
		   )
		{
			//printf("1 - purge vertex %d\n", voisinsFromNode->vLabel);
			ret = deleteVertex(g, voisinsFromNode->vLabel);
			if(ret == -1)
				break;
			
			voisinsFromNode = getNeighboursFromLabel(g, fromNode);
		}
		else
		{
			voisinsFromNode = voisinsFromNode->vSuiv;
		}
	}
	
	// on enleve du graphe celui qui nous a envoye le paquet
	deleteVertex(g, fromNode);
	
	
	voisin* voisinsPredNode = getNeighboursFromLabel(g, predNode);
	while(voisinsPredNode != 0)
	{
		if(
		   (voisinsPredNode->vLabel != c->node)
		   &&
		   (getEdgeCost(g, c->node, voisinsPredNode->vLabel) > costMax)
		   )
		{
			//printf("1 - purge vertex %d\n", voisinsFromNode->vLabel);
			ret = deleteVertex(g, voisinsPredNode->vLabel);
			if(ret == -1)
				break;
			
			voisinsPredNode = getNeighboursFromLabel(g, predNode);
		}
		else
		{
			voisinsPredNode = voisinsPredNode->vSuiv;
		}
	}
	
	// on enleve du graphe celui qui nous a relaye le paquet
	deleteVertex(g, predNode);
	//printf("2 - purge vertex %d\n", fromNode);
	
	// on enleve du graphe les voisins directs plus loins que farestNode
	voisin* fils = getNeighboursFromLabel(g, c->node);
	
	while(fils != 0)
	{
		cost = fils->cout;
		if(cost	> costMax)
		{
			//printf("3 - purge vertex %d\n", fils->vLabel);
			ret = deleteVertex(g, fils->vLabel);
			if(ret == -1)
				break;
			fils = getNeighboursFromLabel(g, c->node);
		}
		else
			fils = fils->vSuiv;
	}
	
	// on purge le graphe
	purgeGrapheOfStables(g);
	
	return g;
}

void forward(call_t* c, packet_t *packet)
{
	struct nodedata *nodedata = get_node_private_data(c);
	array_t *down = get_entity_bindings_down(c);
	packet_PROTOCOLE *data = (packet_PROTOCOLE *) (packet->data + nodedata->overhead);
	
	//printf("%d doit relayer depuis %d\n", c->node, data->pred);
	
	// construire le bip tree pour relayer a partir des infos du paquet
	int indice = listeNodes_get_index(data->askedToRedirect, c->node);
	graphe* g = purgeGraphe(c, listeNodes_get(data->needsToBeCovered, indice), data->src, data->pred);
	// ajouter au nouveau graphe les valeurs d'energie
	int i;
	voisin* trans;
	for(i = 0 ; i < g->nbSommets ; i++)
	{
		trans = g->listeVoisins[i];
		while(trans != 0)
		{
			trans->cout /= nodedata->energiesRem[g->sommets[i]];
			trans = trans->vSuiv;
		}
	}
	/*printf("data->askedToRedirect : ");
	 listeNodes_affiche(data->askedToRedirect);
	 printf("data->needsToBeCovered : ");
	 listeNodes_affiche(data->needsToBeCovered);*/
	
	arbre* bipTree = computeBIPtree(c, g, data->askedToRedirect, data->needsToBeCovered, 0);
	
	// relayer le paquet
	destination_t dst = {-1,{-1,-1,-1}};
	double cout = setRangeToFarestNeighbour(c, nodedata->g2hop, bipTree);
	
	//free(data->askedToRedirect);
	data->askedToRedirect = 0;
	//free(data->needsToBeCovered);
	data->needsToBeCovered = 0;
	data->pred = c->node;
	
	//call_t c0 = {-1,c->node,-1};
	//data->energyRem = battery_remaining(&c0) - cout;
	//nodedata->energiesRem[c->node] = data->energyRem;
	
	setRelayNodes(c, g, bipTree, &data->askedToRedirect, &data->needsToBeCovered, c->node);
	call_t c_down = {down->elts[0], c->node, c->entity};
	SET_HEADER(&c_down, packet, &dst);
	c_down.entity = c->entity;
	c_down.node = c->node;
	c_down.from = -1;
	tx(&c_down, packet);
	arbre_detruire(&bipTree);
	deleteGraphe(g);
	free(g);
}

void set_range_Tr(call_t *c, double range)
{
	range = (floor(range*10)+1)/10.0;
    array_t *mac=get_mac_entities(c);
    call_t c0 = {mac->elts[0], c->node, c->entity};
    struct macnodedata* macdata = get_node_private_data(&c0);
    macdata->range = range;
}


