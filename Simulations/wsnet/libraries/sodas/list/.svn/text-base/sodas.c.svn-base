/**
 *  \file   sodas.c
 *  \brief  SOrted DAta Structure : sorted list implementation 
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <include/mem_fs.h>
#include <include/sodas.h>


/* ************************************************** */
/* ************************************************** */
typedef struct _sodas_elt {
    void              *key;
    void              *data;
    struct _sodas_elt *next;
    struct _sodas_elt *previous;
} sodas_elt_t;

typedef struct _sodas {
    int             size;
    sodas_compare_t compare;
    sodas_elt_t    *elts;
} sodas_t;


/* ************************************************** */
/* ************************************************** */
static void *mem_sodas = NULL;      /* memory slice for sodas */
static void *mem_sodas_elts = NULL; /* memory slice for sodas elements */


/* ************************************************** */
/* ************************************************** */
int sodas_init(void) {
    if ((mem_sodas = mem_fs_slice_declare(sizeof(sodas_t))) == NULL) {
        return -1;
    }

    if ((mem_sodas_elts = mem_fs_slice_declare(sizeof(sodas_elt_t))) == NULL) {
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
    
    sodas->size = 0;
    sodas->compare = compare;
    sodas->elts = NULL;
    
    return (void *) sodas;
}


void sodas_destroy(void *s) {
    sodas_t *sodas = (sodas_t *) s;
    sodas_elt_t *elt;

    while ((elt = sodas->elts) != NULL) {
        sodas->elts = elt->next;
        mem_fs_dealloc(mem_sodas_elts, elt);       
    }
    
    mem_fs_dealloc(mem_sodas, sodas);
}


/* ************************************************** */
/* ************************************************** */
void sodas_insert(void *s, void *key, void *data){
    sodas_t *sodas = (sodas_t *) s;
    sodas_elt_t *elt;
    
    if ((elt = (sodas_elt_t *) mem_fs_alloc(mem_sodas_elts)) == NULL) {
        return;
    }
    
    elt->key = key;
    elt->data = data;
    elt->next = NULL;
    elt->previous = NULL;

    if (sodas->elts == NULL) {
        sodas->elts = elt;
    } else {
        sodas_elt_t *cur = sodas->elts;
        sodas_elt_t *p_cur = NULL;
        
        while ((cur != NULL) && (sodas->compare(key, cur->key) == -1)) {
            p_cur = cur;
            cur = cur->next;
        }
        
        if (p_cur == NULL) {
            elt->next = cur;
            cur->previous = elt;
            sodas->elts = elt;
        } else {
            p_cur->next = elt;
            elt->previous = p_cur;
            elt->next = cur;
            if ( cur != NULL) {
                cur->previous = elt;
            }
        }
    }
    
    sodas->size++;
}

void *sodas_pop(void *s) {
    sodas_t *sodas = (sodas_t *) s;
    sodas_elt_t *elt;
    void *data;
    
    if ((elt = sodas->elts) == NULL) {
        return NULL;
    }
    data = elt->data;
    
    if ((sodas->elts = elt->next) != NULL) {
        sodas->elts->previous = NULL;
    }
    mem_fs_dealloc(mem_sodas_elts, elt);
    sodas->size--;

    return (void *) data;
}

void *sodas_see_first(void *s) {
    sodas_t *sodas = (sodas_t *) s;
    
    if (sodas->elts == NULL) {
        return NULL;
    }
    return (void *) sodas->elts->data;
}


void *sodas_delete(void *s, void *key) {
  sodas_t *sodas = (sodas_t *) s;
  void *data;

  if(sodas->size == 0 || sodas->elts == NULL) {
    return NULL;
  } 
  else {
    sodas_elt_t *cur = sodas->elts;
    sodas_elt_t *p_cur = NULL;
        
    while ((cur != NULL) && (sodas->compare(key, cur->key) != 0)) {
      p_cur = cur;
      cur = cur->next;
    }

    if (cur != NULL) {
      
      // remove the element at the beginning
      if (p_cur == NULL) {
	data = cur->data;
	if ((sodas->elts = cur->next) != NULL) {
	  sodas->elts->previous = NULL;
	}
	mem_fs_dealloc(mem_sodas_elts, cur);
	sodas->size--;
	return (void *) data;
      }
      // remove the element at the end
      else if (cur->next == NULL) {
	cur->previous = NULL;
	p_cur->next = NULL;
	data = cur->data;
	mem_fs_dealloc(mem_sodas_elts, cur);
	sodas->size--;
	return (void *) data;
      }
      // remove the element from the middle
      else {
	p_cur->next = cur->next;	
	(cur->next)->previous = p_cur;
	cur->next = NULL;
	cur->previous = NULL;
	data = cur->data;
	mem_fs_dealloc(mem_sodas_elts, cur);
	sodas->size--;
	return (void *) data;
      }

    }

  }

  return NULL;
}
