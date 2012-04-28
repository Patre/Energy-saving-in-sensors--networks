
#include "AddedFunction.h"



/* ************************************************** */
/* ************************************************** */
void prim_tree(list *nodes,arbre **a,listC *l,list *g)
{
	
    int nbrNode=list_taille(g);
    list *deja_couvert=Nullptr(list);
    list_copy(&deja_couvert,nodes);
	
	
    listC *tmp=l;
    list *tmp2=deja_couvert;
	
    //list_con_affiche(tmp);
    //phase 1
    /*
	 * Recuperer le minimum  a partir de node racine node
	 */
    double min_poids=DBL_MAX;
    int node_min=-1;
    int node_couvrant=-1;
	
    int node;
	
    int i=0;
    //COUVRIR TOUS LES NOEUDS
    for(i=0;i<nbrNode;i++)
    {
        tmp2=deja_couvert;
		
        //RENIALISER LES PARAMETRE
        min_poids=DBL_MAX;
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
        }//*/
		
		
		
        //On ajoute l'element dans les couvert
        //printf("NR:%d NM:%d P:%.2lf\n",node_couvrant,node_min,min_poids);
        list_insert(&deja_couvert,node_min);
		
        //on ajoute dans l'arbre
        arbre_add_fils(*a,node_couvrant,node_min);
    }
	

}



//RANGE
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
