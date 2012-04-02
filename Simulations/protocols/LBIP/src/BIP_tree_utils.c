
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

/*void prim_tree(int node,arbre **a,listC *l,list *g)
{
	
    int nbrNode=list_taille(g);
    list *deja_couvert=Nullptr(list);
    list_insert(&deja_couvert,node);
	
	
    listC *tmp=l;
    list *tmp2=deja_couvert;
	
    //list_con_affiche(tmp);
    //phase 1
    
	// Recuperer le minimum  a partir de node racine node
    double min_poids=9999999999999999999999.00;
    int node_min=-1;
    int node_couvrant=-1;
	
	
    int i=0;
    //COUVRIR TOUS LES NOEUDS
    for(i=0;i<nbrNode;i++)
    {
        tmp2=deja_couvert;
		
        //RENIALISER LES PARAMETRE
        min_poids=999999999999999999999999999999999.00;
        node_min=-1;
        node_couvrant=-1;
		
        //parcourir des noeud deja couvert
        while(tmp2)
        {
            //recuperer le noeud couvert en cours
            node=tmp2->val;
			
            //printf("NR: %d comparer avec",node);
            tmp=l;
			
			
            //Parcourir les list de connection
            while(tmp)
            {
				
                ///Si une connection trouver avec comme literal un noued deja couvert et que le poids est minimal
                if((tmp->node1 == node || tmp->node2 == node)   && (tmp->poids<min_poids))
                {
                    //afficher l'extance de noued dans la list des connexion
                    //printf("(%d,%d),  ",tmp->node1,tmp->node2,tmp->poids);
					
					
                    //si le noeud literl 1 est le noeud source et que l'autre n'est pas deja couvert
                    if(tmp->node1==node && !list_recherche(deja_couvert,tmp->node2))
                    {
                        //Sauvgarder le node avec le minimum poids
                        node_couvrant=node;
						
                        node_min=tmp->node2;
                        min_poids=tmp->poids;
                    }
                    //sinon le contraire
                    else if(!list_recherche(deja_couvert,tmp->node1))
                    {
                        //Sauvgarder le node avec le minimum poids
                        node_couvrant=node;
						
                        node_min=tmp->node1;
                        min_poids=tmp->poids;
                    }
                }
                //ON passse a la connection suivante
                tmp=tmp->suiv;
            }
            //printf("Min is (%d,%d) %.2lf\n",node_couvrant, node_min,min_poids);
            //On passe a le noued couvert suivant
            tmp2=tmp2->suiv;
        }
		
		
		
        //On ajoute l'element dans les couvert
        //printf("NR:%d NM:%d P:%.2lf\n",node_couvrant,node_min,min_poids);
        list_insert(&deja_couvert,node_min);
		
        //on ajoute dans l'arbre
        arbre_add_fils(*a,node_couvrant,node_min);
    }
	
    DEBUG;
    //printf("\n\n");
    //list_affiche(deja_couvert);
    //arbre_affiche(*a);
}

void prim_tree2(list *nodes,arbre **a,listC *l,list *g)
{
	
    int nbrNode=list_taille(g);
    list *deja_couvert=Nullptr(list);
    list_copy(&deja_couvert,nodes);
	
	
    listC *tmp=l;
    list *tmp2=deja_couvert;
	
    //list_con_affiche(tmp);
    //phase 1
    
	//Recuperer le minimum  a partir de node racine node
    double min_poids=9999999999999999999999.00;
    int node_min=-1;
    int node_couvrant=-1;
	
    int node;
	
    int i=0;
    //COUVRIR TOUS LES NOEUDS
    for(i=0;i<nbrNode;i++)
    {
        tmp2=deja_couvert;
		
        //RENIALISER LES PARAMETRE
        min_poids=999999999999999999999999999999999.00;
        node_min=-1;
        node_couvrant=-1;
		
        //parcourir des noeud deja couvert
        while(tmp2)
        {
            //recuperer le noeud couvert en cours
            node=tmp2->val;
			
            //printf("NR: %d comparer avec",node);
            tmp=l;
			
			
            //Parcourir les list de connection
            while(tmp)
            {
				
                ///Si une connection trouver avec comme literal un noued deja couvert et que le poids est minimal
                if((tmp->node1 == node || tmp->node2 == node)   && (tmp->poids<min_poids))
                {
                    //afficher l'extance de noued dans la list des connexion
                    //printf("(%d,%d),  ",tmp->node1,tmp->node2,tmp->poids);
					
					
                    //si le noeud literl 1 est le noeud source et que l'autre n'est pas deja couvert
                    if(tmp->node1==node && !list_recherche(deja_couvert,tmp->node2))
                    {
                        //Sauvgarder le node avec le minimum poids
                        node_couvrant=node;
						
                        node_min=tmp->node2;
                        min_poids=tmp->poids;
                    }
                    //sinon le contraire
                    else if(!list_recherche(deja_couvert,tmp->node1))
                    {
                        //Sauvgarder le node avec le minimum poids
                        node_couvrant=node;
						
                        node_min=tmp->node1;
                        min_poids=tmp->poids;
                    }
                }
                //ON passse a la connection suivante
                tmp=tmp->suiv;
            }
            //printf("Min is (%d,%d) %.2lf\n",node_couvrant, node_min,min_poids);
            //On passe a le noued couvert suivant
            tmp2=tmp2->suiv;
        }
		
		
		
        //On ajoute l'element dans les couvert
        //printf("NR:%d NM:%d P:%.2lf\n",node_couvrant,node_min,min_poids);
        list_insert(&deja_couvert,node_min);
		
        //on ajoute dans l'arbre
        arbre_add_fils(*a,node_couvrant,node_min);
    }
	
    DEBUG;
    //printf("\n\n");
    //list_affiche(deja_couvert);
    //arbre_affiche(*a);
}

//initailisation de l'arbre de LBIP
void prim_tree_lbip(int node,arbre **a, list *N1, list2 *N2, double alpha, double c)
{
    listC *connect=Nullptr(listC);
    list_to_listC(&connect,N1,node);
    list2_to_listC(&connect,N2);
	
    listC *tmp=connect;
    while(tmp)
    {
        double x=calcul_energie(tmp->node1,tmp->node2,alpha,c);
        list_set_poids(tmp,tmp->node1,tmp->node2,x);
        tmp=tmp->suiv;
    }
	
	
    list *g=Nullptr(list);
    list2_to_list(&g,N2);
	
    DEBUG;
    printf("\n*************************************************************\n"
	 "TOUS ces VOISINAGE DE 1 HOPE et 2 HOP de NODE %d\n",node);
	 list_affiche(g);
	 printf("\nTOUS les connection LOCAL (node1,node2, POIDs)\n");
	 list_con_affiche(connect);
	
    prim_tree(node,a,connect,g);
	
}

//initailisation de l'arbre de LBIP
void init_lbip_tree(call_t *c, void *args)
{
    struct nodedata *nodedata = get_node_private_data(c);
    struct entitydataLBIP *entitydata=get_entity_private_data(c);
	
    prim_tree_lbip(c->node,&nodedata->tree_LBIP,nodedata->N1,nodedata->N2, entitydata->alpha, entitydata->c);
	
    DEBUG;
    printf("INITIALISATION DE L'ARBRE LBIP pour le noeud %d at %lf\n",c->node,
	 get_time_now_second());
    //arbre_detruire(&nodedata->tree_LBIP);
}*/


