#include "AddedFunction.h"

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

    //printf("%lf\n",par1+c);
    return par1+c;
}

arbre* computeBIPtree(call_t *c, graphe* g)
{
        arbre* bipTree = 0;
        int i, minNode, devientEmetteur;
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
                        arbre_add_pere(&(bipTree), g->sommets[minNode]);
                }
                else
                {
                        arbre_add_fils(bipTree, g->sommets[pere[minNode]], g->sommets[minNode]);
                }

                devientEmetteur = 0;
                // pour chacun des voisins de minNode dans le graphe
                trans = g->listeVoisins[minNode];
                while(trans != 0)
                {
                        coutIncremental = trans->cout - poids[minNode];
                        if(!(arbre_recherche(bipTree, trans->vLabel)) && (coutIncremental < cle[getNumFromLabel(g,trans->vLabel)]))
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


        return bipTree;
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

double setRangeToFarestNeighbour(call_t *c, graphe* g, arbre* bipTree)
{
        struct protocoleData *entitydata = get_entity_private_data(c);

        // calcul de la distance entre ce noeud et son voisin 1-hop (dans l'arbre de BIP) le plus eloigne
        list *fils = 0;
        double distMax = 0, dist;
        arbre_get_fils(&fils, bipTree, c->node);
        while(fils != 0)
        {
                dist = getEdgeCost(g, c->node, fils->val);
                dist = getDistanceFromCout(dist, entitydata->alpha, entitydata->c);
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

void setRelayNodes(call_t *c, graphe* g, arbre* bipTree, listeNodes** askedToRedirect, listeNodes** needsToBeCovered)
{

        list *fils = 0, *fils2 = 0;
        double cout, coutMax = 0;
        int nodeToCover = -1;

        arbre_get_fils(&fils, bipTree, c->node);
        while(fils != 0)
        {
                if(!arbre_is_leaf(bipTree, fils->val))
                {
                        arbre_get_fils(&fils2, bipTree, fils->val);
                        listeNodes_insert(askedToRedirect, fils->val);
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
                        listeNodes_insert(needsToBeCovered, nodeToCover);
                }
                fils = fils->suiv;
        }

        printf("Relayer : \n");
        listeNodes_affiche(*askedToRedirect);
        listeNodes_affiche(*needsToBeCovered);
}

graphe* purgeGraphe(call_t* c, int farestNode, int fromNode)
{
        struct nodedata *nodedata = get_node_private_data(c);
        int ret;
        graphe* g = copieGraphe(nodedata->g2hop);
        double costMax = getEdgeCost(g, c->node, farestNode), cost;

        // on enleve du graphe tous les voisins directs de fromNode
        // sauf celui qu'on doit toucher et nous meme
        // on garde egalement les noeuds voisins de fromNode qui sont plus pres de nous que farestNode
        voisin** voisinsFromNode = &(g->listeVoisins[getNumFromLabel(g, fromNode)]);
        while(*voisinsFromNode != 0)
        {
                if(
                   ((*voisinsFromNode)->vLabel != c->node)
                   &&
                   (getEdgeCost(g, c->node, (*voisinsFromNode)->vLabel) > costMax)
                   )
                {
                        ret = deleteVertex(g, (*voisinsFromNode)->vLabel);
                        if(ret == -1)
                                break;
                        voisinsFromNode = &(g->listeVoisins[getNumFromLabel(g, fromNode)]);
                }
                else
                        voisinsFromNode = &((*voisinsFromNode)->vSuiv);
        }


        // on enleve du graphe celui qui nous a relaye le paquet
        deleteVertex(g, fromNode);

        // on enleve du graphe les voisins directs plus loins que farestNode
        voisin** fils = &(g->listeVoisins[getNumFromLabel(g, c->node)]);

        while(*fils != 0)
        {
                cost = (*fils)->cout;
                if(cost	> costMax)
                {
                        afficherGraphe(g);
                        ret = deleteVertex(g, (*fils)->vLabel);
                        if(ret == -1)
                                break;
                        fils = &(g->listeVoisins[getNumFromLabel(g, c->node)]);
                }
                else
                        fils = &((*fils)->vSuiv);
        }

        // on purge le graphe
        purgeGrapheOfStables(g);

        return g;
}


