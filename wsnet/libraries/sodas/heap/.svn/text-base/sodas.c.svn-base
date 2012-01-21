/**
 *  \file   sodas.c
 *  \brief  SOrted DAta Structure : heap implementation 
 *  \author Guillaume Chelius & Elyes Ben Hamida & Ibrahim Amadou
 *  \date   2007
 **/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <include/mem_fs.h>
#include <include/sodas.h>


/* ************************************************** */
/* ************************************************** */
#define HEAP_MAX 100
#define HEAP_MIN 100


/* ************************************************** */
/* ************************************************** */
typedef struct _sodas_elt {
    void *key;
    void *data; 
} sodas_elt_t;


typedef struct _sodas {
    int             m_capacity;
    int             capacity;
    int             size;
    sodas_compare_t compare;
    sodas_elt_t    *elts;
} sodas_t;


/* ************************************************** */
/* ************************************************** */
static void *mem_sodas = NULL; /* memory slice for sodas */


/* ************************************************** */
/* ************************************************** */
int sodas_init(void) {
    if ((mem_sodas = mem_fs_slice_declare(sizeof(sodas_t))) == NULL) {
        return -1;
    }
    
    return 0;
}


/* ************************************************** */
/* ************************************************** */
void *sodas_create(sodas_compare_t compare) {
    sodas_t *sodas;
    
    if ((sodas = (sodas_t *) mem_fs_alloc(mem_sodas)) == NULL) {
        return NULL;
    }
    
    if ((sodas->elts = (sodas_elt_t *) malloc((HEAP_MAX + 1) * sizeof(sodas_elt_t))) == NULL) {
        return NULL;
    }
    sodas->m_capacity = HEAP_MIN;
    sodas->capacity = HEAP_MAX;
    sodas->size = 0;
    sodas->compare = compare;
    sodas->elts[0].key = NULL;
    sodas->elts[0].data = NULL;
    
    return (void *) sodas;
}

void sodas_destroy(void *s) {
    sodas_t *sodas = (sodas_t *) s;

    free(sodas->elts);
    mem_fs_dealloc(mem_sodas, sodas);
}


/* ************************************************** */
/* ************************************************** */
void sodas_insert(void *s, void *key, void *data){
    sodas_t *sodas = (sodas_t *) s;
    int i = 0;
    
    /* double the heap capacity if it is full */
    if (sodas->capacity == sodas->size) {
        sodas_elt_t *elts;
        
        if ((elts = (sodas_elt_t *) malloc((sodas->capacity * 2 + 1) * sizeof(sodas_elt_t))) == NULL) {
            return;
        }
        memcpy(elts, sodas->elts, (sodas->size + 1) * sizeof(sodas_elt_t));
        free(sodas->elts);
        sodas->elts = elts;
        sodas->capacity *= 2;
    }
    
    /* add the new element */
    for (i = ++(sodas->size); sodas->compare(sodas->elts[i/2].key, key) == -1; i /= 2) {
        sodas->elts[i].key = sodas->elts[i/2].key;
        sodas->elts[i].data = sodas->elts[i/2].data;
    }
    sodas->elts[i].key = key;
    sodas->elts[i].data = data;
}

void *sodas_pop(void *s) {
    sodas_t *sodas = (sodas_t *) s;
    sodas_elt_t m_elt, l_elt;
    int i, child, n_capacity = (2 * sodas->capacity) / 3;
    
    if (sodas->size == 0) {
        return NULL;
    }
  
    /* reduce the heap capacity if 2/3 of the queue is empty */
    if ((sodas->capacity >= (3 * sodas->size)) 
        && ((n_capacity) >= sodas->m_capacity)) {
        sodas_elt_t *elts;
        
        if ((elts = (sodas_elt_t *) malloc((n_capacity + 1) * sizeof(sodas_elt_t))) == NULL) {
            return NULL;
        }
        
        memcpy(elts, sodas->elts, (sodas->size + 1) * sizeof(sodas_elt_t));
        free(sodas->elts);
        sodas->elts = elts;
        sodas->capacity = n_capacity;
    }
    
    /* pop the first element */
    m_elt.key = sodas->elts[1].key;
    m_elt.data = sodas->elts[1].data;
    l_elt.key = sodas->elts[sodas->size].key;
    l_elt.data = sodas->elts[sodas->size].data;
    sodas->size--;

    for(i = 1; i * 2 <= sodas->size; i = child) {
        child = i * 2;
        
        if ((child != sodas->size) 
            && (sodas->compare(sodas->elts[child+1].key, sodas->elts[child].key) == 1))
            child++;
        
        if (sodas->compare(l_elt.key, sodas->elts[child].key) == -1) {
            sodas->elts[i].key = sodas->elts[child].key;
            sodas->elts[i].data = sodas->elts[child].data;
        } else {
            break;
        }
    }
    sodas->elts[i].key = l_elt.key;
    sodas->elts[i].data = l_elt.data;
    
    return (void *) m_elt.data;
}

void *sodas_see_first(void *s) {
    sodas_t *sodas = (sodas_t *) s;
    
    if (sodas->size == 0) {
        return NULL;
    }
  
    return  sodas->elts[1].data;
}


/* ************************************************** */
/* ************************************************** */

/* edit by Ibrahim Amadou <ibrahim.amadou@insa-lyon.fr> */
void *sodas_delete(void *S, void *key) {
    sodas_t *sodas = (sodas_t *) S;
    sodas_elt_t elts, temp;
    int i = 0, p = 0, child;
  
    if(sodas->size == 0) {
      return NULL;
    }

    if(sodas->compare(sodas->elts[1].key, key) == 0) {      
       return sodas_pop(S);
    }else if(sodas->compare(sodas->elts[sodas->size].key, key) == 0) {
      elts.key  = sodas->elts[sodas->size].key;
      elts.data = sodas->elts[sodas->size].data;      
      sodas->size--;      
      return (void *) elts.data;
    }else{
         while(p != 1) {
	   
           for(i = 1; 2*i <= sodas->size; i++) {
                  child = 2*i;
                  if((child != sodas->size) && (sodas->compare(sodas->elts[child].key, key) == 0)) {
		    temp.key  = sodas->elts[i].key;
		    temp.data = sodas->elts[i].data;
		    sodas->elts[i].key = sodas->elts[child].key;
		    sodas->elts[i].data = sodas->elts[child].data; 
		    sodas->elts[child].key = temp.key;
		    sodas->elts[child].data = temp.data;
		    p = i;
		    break;
		  }
		  child++;
		  
		  if(sodas->compare(sodas->elts[child].key, key) == 0) {
		    temp.key  = sodas->elts[i].key;
		    temp.data = sodas->elts[i].data;
		    sodas->elts[i].key = sodas->elts[child].key;
		    sodas->elts[i].data = sodas->elts[child].data; 
		    sodas->elts[child].key = temp.key;
		    sodas->elts[child].data = temp.data;
		    p = i;
		    break;
		  }
           }
           if(p == 1) {
	     return sodas_pop(S);
           }else if(p == 0) {
             p = 1;
           }
	 }
	 return NULL;       
    }
}
/* end of edition */
