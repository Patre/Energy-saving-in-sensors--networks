
#include "BIP/BIP_tree_utils.h"


void computeBIPtree2Hop(call_t *c)
{
	struct nodedata *nodedata = get_node_private_data(c);
	
	graphe g;
	computeGrapheFromNeighbours(c, &g);
}

void computeGrapheFromNeighbours(call_t *c, graphe* g)
{
	struct nodedata *nodedata = get_node_private_data(c);
	
	listeNodes* oneHop = nodedata->oneHopNeighbourhood;
	listeNodes* twoHop = nodedata->twoHopNeighbourhood;
}

void setRelayNodes(listeNodes** askedToRedirect, listeNodes** needsToBeCovered)
{
	
}



/*void init_bip_tree(call_t *c, void *args)
{
    struct nodedata *nodedata = get_node_private_data(c);
	
	
    //RECUPERER L4ENSEMBLE DE GRAPH
    int i=0;
    call_t *inter=malloc(sizeof(call_t));
    inter->entity=c->entity;
    inter->from=c->from;
	
    for(i=0;i<get_node_count();i++)
    {
        inter->node=i;
        struct nodedata *interdata=get_node_private_data(inter);
        list2N_insert_values(&nodedata->NodesV1,inter->node,get_node_position(inter->node)->x,
                             get_node_position(inter->node)->y, get_node_position(inter->node)->z,interdata->N1);
    }
	
    //REcuperer le graph G
    list *graphElements=Nullptr(list);
    for(i=0;i<get_node_count();i++) list_insert(&graphElements,i);
    list_delete(&graphElements,c->node);
	
    //list de commencement
    list *debut=Nullptr(list);
    list_insert(&debut,c->node);
	
    //recuperer tout les connexion dans le graph
    listC *connexions=Nullptr(listC);
    list2N_to_listC(&connexions,nodedata->NodesV1);
	
    listC *poi=connexions;
    while (poi)
    {
        //on ai dans le bip, nous avons le droit de fair Áa
        list_set_poids(connexions,poi->node1,poi->node2,distance(get_node_position(poi->node1),get_node_position(poi->node2)));
        poi=poi->suiv;
    }
	
    prim_tree(debut,&nodedata->tree_BIP,connexions,graphElements);
	
	free(inter);
}


double   calcul_energie(element A,element B,double alpha,double c)
{
    double dAB;
    double par1;
	
    //Distance
    dAB =sqrt((A.x-B.x)*(A.x-B.x)+
              (A.y-B.y)*(A.y-B.y)+
              (A.z-B.z)*(A.z-B.z));
	
    //la premier partie de la fonction
    par1=pow(dAB,alpha);
	
    return par1+c;
}

void prim_tree(list *nodes,arbre **a,listC *l,list *g)
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
}*/


