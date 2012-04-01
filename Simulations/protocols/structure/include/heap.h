/*
 * =====================================================================================
 *
 *       Filename:  heap.h
 *
 *    Description:  Heap implementation
 *
 *        Version:  1.0
 *        Created:  26/03/2012 10:42:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include <stdio.h>

// {{{ Structures

/** \brief Basic implementation of a heap (max)
 *
 * This heap implementation uses a dynamically allocated array, but its size is fixed to the number
 * of node, so that we won't need to manage it during using it */
typedef struct h {
	int _max_size, _current_size;
	double *_lab;
	int *_nodes;
} Heap;
// }}}

// {{{ Macros
/** Macro which returns the size of the array representing the heap */
#define max(h) h->_max_size
/** Macro which returns the number of items in the heap */
#define cur(h) h->_current_size
/** Macro which returns left child of given item */
#define lchild(h,e) (h->_nodes)[((e+1)<<1)-1]
/** Macro which returns right child of given item */
#define rchild(h,e) (h->_nodes)[((e+1)<<1)]
/** Macro which returns father of given item */
#define fath(h,e) (h->_nodes[(e-1)/2])
/** Macro which returns nodes of given heap */
#define nodes(h) h->_nodes
/** Macro which returns node of given heap at the given index */
#define node(h,e) h->_nodes[e]

#define hlab(h,e) h->_lab[e]
#define htlab(h) h->_lab
// }}}

// {{{ Memory management
/** 
 * Allocate memory useful for a n heap 
 * \param n Max size of the heap
 * \return Pointer to the allocated memory 
 * */
Heap *allocHeap (int n, double *lab);

/**
 * Free memory used by a Heap
 * \param p Pointer to the heap to destroy
 * */
void freeHeap (Heap *p);
// }}}

// {{{ Building functions
/**
 * Insert an item in the heap
 * \param p pointer to the heap
 * \param node Item to insert
 * \param lab label of inserted node
 * \return index of inserted node in the array
 * */
int h_insertNode (Heap *p, int node);

/**
 * Remove the heap head
 * \param p pointer to the heap
 * \return head of the heap
 * */
int h_remNode (Heap *p);
// }}}

// {{{ Display function
void h_print (Heap *p);

int h_isEmpty(Heap *p);

int h_contains(Heap *p, int node);

void h_changeLabel(Heap *p, int node, double newLabel);

int h_getPos(Heap *p, int node);

#endif
