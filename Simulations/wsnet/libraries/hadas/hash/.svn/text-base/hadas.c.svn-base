/**
 *  \file   hadas.c
 *  \brief  Hashed DAta Structure
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <include/mem_fs.h>
#include <include/hadas.h>


/* ************************************************** */
/* ************************************************** */
#define HASH_SIZE 32


/* ************************************************** */
/* ************************************************** */
typedef struct _hadas_elt {
    void *key;
    void *data;
    struct _hadas_elt *next;
    struct _hadas_elt *previous;
} hadas_elt_t;

typedef struct _sodas {
    int size;
    hash_hash_t hash;
    hash_equal_t equal;
    hadas_elt_t *elts[HASH_SIZE];
} hadas_t;


/* ************************************************** */
/* ************************************************** */
static void *mem_hadas = NULL;      /* memory slice for hadas */
static void *mem_hadas_elts = NULL; /* memory slice for hadas elements */


/* ************************************************** */
/* ************************************************** */
int hadas_init(void) {
    if ((mem_hadas = mem_fs_slice_declare(sizeof(hadas_t))) == NULL) {
        return -1;
    }

    if ((mem_hadas_elts = mem_fs_slice_declare(sizeof(hadas_elt_t))) == NULL) {
        return -1;
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
void *hadas_create(hash_hash_t hash, hash_equal_t equal) {
    hadas_t *hadas;
    int i;
    
    if ((hadas = (hadas_t *) mem_fs_alloc(mem_hadas)) == NULL) {
        return NULL;
    }
    
    hadas->size = 0;
    hadas->equal = equal;
    hadas->hash = hash;

    for(i=0;i<HASH_SIZE;i++){
      hadas->elts[i] = NULL;
    }
    
    return (void *) hadas;
}

void hadas_destroy(void *s) {
    hadas_t *hadas = (hadas_t *) s;
    hadas_elt_t *elt;
    int i;

    if (hadas != NULL) {
      for (i = 0; i < HASH_SIZE; i++) {
        while ((elt = hadas->elts[i]) != NULL) {
	  hadas->elts[i] = elt->next;
	  mem_fs_dealloc(mem_hadas_elts, elt);       
        }
      }
      mem_fs_dealloc(mem_hadas, hadas);
    }
}


/* ************************************************** */
/* ************************************************** */
void hadas_insert(void *s, void *key, void *data){
    hadas_t *hadas = (hadas_t *) s;
    hadas_elt_t *elt;
    unsigned long hash = hadas->hash(key) % HASH_SIZE;
    
    
    if ((elt = (hadas_elt_t *) mem_fs_alloc(mem_hadas_elts)) == NULL) {
        return;
    }
    
    elt->key = key;
    elt->data = data;
    elt->next = NULL;
    elt->previous = NULL;

    if (hadas->elts[hash] == NULL) {
        hadas->elts[hash] = elt;
    } else {
        elt->next = hadas->elts[hash];
        hadas->elts[hash]->previous = elt;
        hadas->elts[hash] = elt;
    }

    hadas->size++;
}

void *hadas_get(void *h, void *key) {
    hadas_t *hadas = (hadas_t *) h;
    unsigned long hash = hadas->hash(key) % HASH_SIZE;
    hadas_elt_t *elt = hadas->elts[hash];
    
    while (elt != NULL) {
        if (hadas->equal(key, elt->key)) {
            return elt->data;
        }
        elt = elt->next;
    }
    
    return NULL;
}

void hadas_delete(void *h, void *key) {
    hadas_t *hadas = (hadas_t *) h;
    unsigned long hash = hadas->hash(key) % HASH_SIZE;
    hadas_elt_t *elt = hadas->elts[hash], *o_elt = NULL;
    
    while (elt != NULL) {
        if (hadas->equal(elt->key, key)) {
            /* delete from the begining*/
            if (o_elt == NULL) {
                hadas->elts[hash] = elt->next;
                if (hadas->elts[hash] != NULL) {
                    hadas->elts[hash]->previous = NULL;
                }
            } else {
                o_elt->next = elt->next;
                if (elt->next != NULL){ 
                    elt->next->previous = o_elt;
                }
            }
            
            hadas->size--;
            mem_fs_dealloc(mem_hadas_elts, elt);
            return;
        }
        o_elt = elt;
        elt = elt->next;
    }
    
    return;
}


