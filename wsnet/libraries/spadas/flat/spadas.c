/**
 *  \file   spadas.c
 *  \brief  Space PArtitioning DAta Structure module : flat implementation
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <include/mem_fs.h>
#include <include/das.h>
#include <include/spadas.h>


/* ************************************************** */
/* ************************************************** */
typedef struct _spadas_elt {
    void               *key;
    position_t          position;
    struct _spadas_elt *next;
    struct _spadas_elt *previous;
} spadas_elt_t;

typedef struct _spadas {
    position_t    area;
    double        range;
    int           size;
    spadas_elt_t *elts;
} spadas_t;


/* ************************************************** */
/* ************************************************** */
static void *mem_spadas = NULL;      /* memory slice for spadas */
static void *mem_spadas_elts = NULL; /* memory slice for spadas elements */


/* ************************************************** */
/* ************************************************** */
int spadas_init(void) {
    if (das_init() == -1) {
        return -1;
    } 

    if ((mem_spadas = mem_fs_slice_declare(sizeof(spadas_t))) == NULL) {
        return -1;
    }

    if ((mem_spadas_elts = mem_fs_slice_declare(sizeof(spadas_elt_t))) == NULL) {
        return -1;
    }

    return 0;
}


/* ************************************************** */
/* ************************************************** */
void *spadas_create(position_t *area, double range) {
    spadas_t *spadas;

    if ((spadas = (spadas_t *) mem_fs_alloc(mem_spadas)) == NULL) {
        return NULL;
    }
    
    spadas->area.x = area->x;
    spadas->area.y = area->y;
    spadas->area.z = area->z;
    spadas->range = range;
    spadas->size = 0;
    spadas->elts = NULL;
    
    return (void *) spadas;
}

void spadas_destroy(void *s) {
    spadas_t *spadas = (spadas_t *) s;
    spadas_elt_t *elt;

    while ((elt = spadas->elts) != NULL) {
      spadas->elts = elt->next;
      mem_fs_dealloc(mem_spadas_elts, elt);
    }
    
    mem_fs_dealloc(mem_spadas, spadas);
}


/* ************************************************** */
/* ************************************************** */
void spadas_insert(void *s, void *key, position_t *position) {
    spadas_t *spadas = (spadas_t *) s;
    spadas_elt_t *elt;

    if ((elt = (spadas_elt_t *) mem_fs_alloc(mem_spadas_elts)) == NULL) {
        return;
    }

    elt->key = key;
    elt->position.x = position->x;
    elt->position.y = position->y;
    elt->position.z = position->z;
    elt->next = NULL;
    elt->previous = NULL;
    
    if (spadas->elts == NULL) {
      spadas->elts = elt;
    } else {
      elt->next = spadas->elts;
      spadas->elts->previous = elt;
      spadas->elts = elt;
    }
    
    return;
}

void spadas_update(void *s, void *key, position_t *n_position, position_t *o_position) {
    spadas_t *spadas = (spadas_t *) s;
    spadas_elt_t *elt = spadas->elts;
    
    while ((elt != NULL) && (elt->key != key)) {
        elt = elt->next;
    }
    
    if (elt != NULL) {
        elt->position.x = n_position->x;
        elt->position.y = n_position->y;
        elt->position.z = n_position->z;
    } else {
        spadas_insert(s, key, n_position);
    }
    
    return;
}


/* ************************************************** */
/* ************************************************** */
void spadas_delete(void *s, void *key, position_t *position) {
    spadas_t *spadas = (spadas_t *) s;
    spadas_elt_t *elt = spadas->elts;

    while ((elt != NULL) && (elt->key != key)) {
        elt = elt->next;
    }

    if (elt == NULL) {
        return;
    }
      
    if (elt->previous != NULL) {
        elt->previous->next = elt->next;
    } else {
        spadas->elts = elt->next;
    }
    if (elt->next != NULL) {
        elt->next->previous = elt->previous;
    } 
    
    mem_fs_dealloc(mem_spadas_elts, elt);
    return;
}

/* ************************************************** */
/* ************************************************** */
void *spadas_rangesearch(void *s, void *key, position_t *position, double range) {
    spadas_t *spadas = (spadas_t *) s;
    spadas_elt_t *elt = spadas->elts;
    void *das;
    
    if ((das = das_create()) == NULL) {
        return NULL;
    }

    while (elt != NULL) {
        double dist;

        if (elt->key == key) {
            elt = elt->next;
            continue;
        }

        dist = sqrt((elt->position.x - position->x) * (elt->position.x - position->x)
                    + (elt->position.y-position->y)*(elt->position.y-position->y)
                    + (elt->position.z - position->z) * (elt->position.z - position->z));
        
        if (dist <= range) {
            das_insert(das, elt->key);
        }

        
        elt = elt->next;
    }    
    
    return das;
}
