/**
 *  \file   das.c
 *  \brief  DAta Structure : list implementation 
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <include/mem_fs.h>
#include <include/das.h>


/* ************************************************** */
/* ************************************************** */
typedef struct _das_elt {
    void            *data;
    struct _das_elt *next;
    struct _das_elt *previous;
} das_elt_t;

typedef struct _das {
    int       size;
    das_elt_t *trav;
    das_elt_t *elts;
    das_elt_t *elts_end; 
} das_t;


/* ************************************************** */
/* ************************************************** */
static void *mem_das = NULL;      /* memory slice for das */
static void *mem_das_elts = NULL; /* memory slice for das elements */


/* ************************************************** */
/* ************************************************** */
int das_init(void) {

    if ((mem_das = mem_fs_slice_declare(sizeof(das_t))) == NULL) {
        return -1;
    }

    if ((mem_das_elts = mem_fs_slice_declare(sizeof(das_elt_t))) == NULL) {
        return -1;
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
void *das_create(void) {
    das_t *das;
    
    if ((das = (das_t *) mem_fs_alloc(mem_das)) == NULL) {
        return NULL;
    }
    
    das->size     = 0;
    das->elts     = NULL;
    das->trav     = NULL;
    das->elts_end = NULL;
    
    return (void *) das;
}

void das_destroy(void *d) {
    das_t *das = (das_t *) d;
    das_elt_t *elt;

    if (das == NULL) {
        return;
    }

    while ((elt = das->elts) != NULL) {
        das->elts = elt->next;
        mem_fs_dealloc(mem_das_elts, elt);       
        das->size--;
    }
    
    mem_fs_dealloc(mem_das, das);
    return;
}

/* ************************************************** */
/* ************************************************** */
int das_getsize(void *d) {
  das_t *das = (das_t *) d;
  return das->size;
}


/* ************************************************** */
/* ************************************************** */
void das_insert(void *d, void *data){
    das_t *das = (das_t *) d;
    das_elt_t *elt;
    
    if ((elt = (das_elt_t *) mem_fs_alloc(mem_das_elts)) == NULL) {
        return;
    }    
    elt->data     = data;
    elt->next     = NULL;
    elt->previous = NULL;
    
    if (das->elts == NULL) {
        das->elts = elt;
        das->elts_end = elt;
    } else {
        elt->next = das->elts;
        das->elts->previous = elt;
        das->elts = elt;
    }

    das->size++;
}

void *das_pop(void *d) {
    das_t *das = (das_t *) d;
    das_elt_t *elt;
    void *data;
    
    if ((elt = das->elts) == NULL) {
        return NULL;
    }
    data = elt->data;

    das->elts = elt->next;
    if (das->elts != NULL) {
        das->elts->previous = NULL;
    } else {
        das->elts_end = NULL;
    }

    das->size--;
    
    mem_fs_dealloc(mem_das_elts, elt);
    return (void *) data;
}

void *das_pop_FIFO(void *d) {
    das_t *das = (das_t *) d;
    das_elt_t *elt;
    void *data;
    
    if ((elt = das->elts_end) == NULL) {
        return NULL;
    } else if (das->elts_end == das->elts) {
        data = elt->data;
        das->elts_end = NULL;
        das->elts = NULL;
        das->size--;
        mem_fs_dealloc(mem_das_elts, elt);
        return (void *) data;
    } else {
        data = elt->data;
        das->elts_end = das->elts_end->previous;
        das->elts_end->next = NULL;
        das->size--;
        mem_fs_dealloc(mem_das_elts, elt);
        return (void *) data;
    }
}


/* ************************************************** */
/* ************************************************** */
int das_find(void *d, void *data) {
    das_t *das = (das_t *) d;
    das_elt_t *elt = das->elts;
    
    while (elt != NULL) {
        if (elt->data == data) {
            return 1;
        }
        elt = elt->next;
    }
    
    return 0;
}

void das_delete(void *d, void *data) {
    das_t *das = (das_t *) d;
    das_elt_t *elt = das->elts, *o_elt = NULL;
    
    while (elt != NULL) {
        
        if (elt->data == data) {
            /* delete from the begining*/
            if (o_elt == NULL) {
                das->elts = elt->next;
                if (das->elts != NULL) {
                    das->elts->previous = NULL;
                } else {
                    das->elts_end = NULL;
                }
            } else if (elt == das->elts_end) {
                das->elts_end = das->elts_end->previous;
                das->elts_end->next = NULL;
            } else {
                o_elt->next = elt->next;
                if (elt->next != NULL){ 
                    elt->next->previous = o_elt;
                }
            }
            
            das->size--;
	    free(elt->data);
            mem_fs_dealloc(mem_das_elts, elt);
            return;
        }
        o_elt = elt;
        elt = elt->next;
    }

    return;
}

void das_selective_delete(void *d, das_delete_func_t delete, void *arg) {
    das_t *das = (das_t *) d;
    das_elt_t *elt = das->elts, *o_elt = NULL, *c_elt;
    
    while (elt != NULL) {    
        if (delete(elt->data, arg)) {
            c_elt = elt;
            elt = c_elt->next;
            
            if (o_elt == NULL) {
                das->elts = c_elt->next;
                if (das->elts != NULL) {
                    das->elts->previous = NULL;
                } else {
                    das->elts_end = NULL;
                }
            } else if (c_elt == das->elts_end) {
                das->elts_end = das->elts_end->previous;
                das->elts_end->next = NULL;
            } else {
                o_elt->next = c_elt->next;
                if (c_elt->next != NULL){ 
                    c_elt->next->previous = o_elt;
                }
            }
            
            das->size--;
            free(c_elt->data);
	    mem_fs_dealloc(mem_das_elts, c_elt);
            
        } else {
            o_elt = elt;
            elt = elt->next;
        }
    }

    return;
}


/* ************************************************** */
/* ************************************************** */
void das_init_traverse(void *d) {
    das_t *das = (das_t *) d;
    
    das->trav = NULL;
}

void *das_traverse(void *d) {
    das_t *das = (das_t *) d;

    if (das->trav == NULL) {
        das->trav = das->elts;
    } else {
        das->trav = das->trav->next;
    }

    if (das->trav) {
        return das->trav->data;
    } else {
        return NULL;
    }
}
