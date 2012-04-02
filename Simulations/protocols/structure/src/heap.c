/*
 * =====================================================================================
 *
 *       Filename:  heap.c
 *
 *    Description:  Implementation of heap
 *
 *        Version:  1.0
 *        Created:  26/03/2012 11:29:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "heap.h"

Heap *allocHeap (int n, double *lab) {
	Heap *h = (Heap *) malloc (sizeof (Heap));
	if (!h) {
		fprintf (stderr, "Erreur malloc heap.\n");
		return NULL;
	}

	max(h) = n;
	cur(h)= 0;
	htlab(h) = lab;

	nodes(h) = (int *) calloc (n, sizeof(int));
	if (!nodes(h)) {
		fprintf (stderr, "Erreur malloc array.\n");
		free (h);
	}

	return h;
}

void freeHeap (Heap *p) {
	free(nodes(p));
	free(p);
}

int h_insertNode (Heap *p, int node) {
	if (cur(p) >= max(p))
		return -1;

	int index = cur(p);

	nodes(p)[cur(p)++] = node;
	while (index && hlab(p,node) > hlab(p,fath(p,index))) {
		int tmp = fath(p,index);
		fath(p,index) = node(p,index);
		node(p,index) = tmp;
		index = index >> 1;
	}

	return index;
}

void entasser_min(Heap *p, int index)
{
	int minInd;
	int node = node(p,index);
	double label = hlab(p,node);
	int indr = (index + 1) * 2;
	int indl = indr - 1;
	double labl = -1, labr = -1;
	if (indl < cur(p))
		labl = hlab(p,lchild(p,index));
	if (indr < cur(p))
		labr = hlab(p,rchild(p,index));
	
	if(indl < cur(p) && labl < label)
		minInd = indl;
	else
		minInd = index;
	if(indr < cur(p) && labr < hlab(p, node(p, minInd)))
		minInd = indr;
	
	if(minInd != index)
	{
		node(p,index) = node(p, minInd);
		node(p, minInd) = node;
		entasser_min(p, minInd);
	}
}

int h_remNode (Heap *p) {
	if(cur(p) == 0)
		return -1;
	int min = node(p,0);
	node(p,0) = node(p,cur(p)-1);
	cur(p)--;
	entasser_min(p, 0);
	return min;
}

void h_print(Heap *p){
	int compteur = 0;
	int max = 1;
	int i;
	for (i=0; i < cur(p); i++) {
		printf("(%d;", node(p,i));
		if( hlab(p,node(p,i)) < 1000000000)
			printf("%.0lf) ", hlab(p,node(p,i)));
		else
			printf("beaucoup) ");
		compteur ++;
		if (compteur == max){
			compteur = 0;
			max *= 2;
			printf("\n");
		}
	}
	printf ("\n\n");
}

int h_isEmpty(Heap *p)
{
	return (cur(p) == 0);
}


int h_contains(Heap *p, int node)
{
	int i;
	for(i = 0 ; i < cur(p) ; i++)
	{
		if(nodes(p)[i] == node)
			return 1;
	}
	return 0;
}


void h_changeLabel(Heap *p, int node, double newLabel)
{
	if (!cur(p))
		return;
	
	int i = h_getPos(p, node);
	p->_lab[node] = newLabel;
	int nodePere = fath(p,i);
	int indPere = (i-1)/2;
	
	
	while(i > 0 && p->_lab[nodePere] > p->_lab[node])
	{
		//printf("echange %d avec %d\n", nodePere, node);
		node(p,i) = nodePere;
		fath(p,i) = node;
		i = indPere;
		indPere = (i-1)/2;
		node = node(p,i);
		nodePere = node(p,indPere);
	}
}

int h_getPos(Heap *p, int node)
{
	int i;
	for(i = 0 ; i < cur(p) ; i++)
	{
		if(node(p,i) == node)
			return i;
	}
	return -1;
}


