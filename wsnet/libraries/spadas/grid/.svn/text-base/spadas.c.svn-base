/**
 *  \file   spadas.c
 *  \brief  PArtitioning DAta Structure module declarations : grid implementation
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <include/modelutils.h>


/* ************************************************** */
/* ************************************************** */
#define MAX(a,b) ((a > b) ? (a) : (b))
#define MIN(a,b) ((a < b) ? (a) : (b))


/* ************************************************** */
/* ************************************************** */
typedef struct _spadas_elt {
    void               *key;
    position_t          position;
    struct _spadas_elt *next;
    struct _spadas_elt *previous;
} spadas_elt_t;

typedef struct _spadas {
    position_t      area;
    double          range;
    int             size;
    double          cell_width;
    int             cell_nbr_x;
    int             cell_nbr_y;
    int             cell_nbr_z;
    spadas_elt_t ****elts;
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
    int i = 0, j = 0, k = 0;
    
    if ((spadas = (spadas_t *) mem_fs_alloc(mem_spadas)) == NULL) {
        return NULL;
    }
    
    spadas->area.x = area->x;
    spadas->area.y = area->y;
    spadas->area.z = area->z;
    spadas->range = range;
    spadas->size = 0;
    spadas->elts = NULL;
    
    spadas->cell_width = (int) ceil((double) range / 2.0);   
	spadas->cell_nbr_x = MAX ((int) ceil((double)area->x / spadas->cell_width), 1);
	spadas->cell_nbr_y = MAX ((int) ceil((double)area->y / spadas->cell_width), 1);
	spadas->cell_nbr_z = MAX ((int) ceil((double)area->z / spadas->cell_width), 1);
    
    /* dynamic allocation of the grid data structure */
    spadas->elts = (spadas_elt_t ****) malloc(sizeof(spadas_elt_t ***) * spadas->cell_nbr_x);
    
    for (i = 0; i < spadas->cell_nbr_x; i++) {
        
        spadas->elts[i] = (spadas_elt_t ***) malloc(sizeof(spadas_elt_t **) * spadas->cell_nbr_y);
        
        for (j = 0; j < spadas->cell_nbr_y; j++) {
            spadas->elts[i][j] = (spadas_elt_t **) malloc(sizeof(spadas_elt_t *) * spadas->cell_nbr_z);
        }
        
    }
    
    /* init the grid data structure */
    for (i = 0; i < spadas->cell_nbr_x; i++) {
        for (j = 0; j < spadas->cell_nbr_y; j++) {
            for (k = 0; k < spadas->cell_nbr_z; k++) {
                spadas->elts[i][j][k] = NULL;
            }
        }
    }
    
    return (void *) spadas;
}

void spadas_destroy(void *s) {
    spadas_t *spadas = (spadas_t *) s;
    int i = 0, j = 0, k = 0; 

    /* remove entries */
    for (i = 0; i < spadas->cell_nbr_x; i++) {
        for (j = 0; j < spadas->cell_nbr_y; j++) {
            for (k = 0; k < spadas->cell_nbr_z; k++) {
                spadas_elt_t *elt = spadas->elts[i][j][k];
                spadas_elt_t *p_elt = NULL;
	    
                while (elt != NULL) {
                    p_elt = elt;
                    elt = elt->next;
                    mem_fs_dealloc(mem_spadas_elts, p_elt);
                }
            }
        }
    }
    
    for (i = 0; i < spadas->cell_nbr_x; i++) {
        for (j = 0; j < spadas->cell_nbr_y; j++) {
            mem_fs_dealloc(mem_spadas_elts, spadas->elts[i][j]);
        }
        mem_fs_dealloc(mem_spadas_elts, spadas->elts[i]);
    }
    
    /* free the data structures */
    mem_fs_dealloc(mem_spadas_elts, spadas->elts);
    mem_fs_dealloc(mem_spadas, spadas);
}


/* ************************************************** */
/* ************************************************** */
void spadas_insert(void *s, void *key, position_t *position) {
    spadas_t *spadas = (spadas_t *) s;
    spadas_elt_t *elt;
    int x = (int) position->x / spadas->cell_width;
    int y = (int) position->y / spadas->cell_width;
    int z = (int) position->z / spadas->cell_width;

    if ((elt = (spadas_elt_t *) mem_fs_alloc(mem_spadas_elts)) == NULL) {
        return;
    }

    elt->key = key;
    elt->position.x = position->x;
    elt->position.y = position->y;
    elt->position.z = position->z;
    elt->next = NULL;
    elt->previous = NULL;
    elt->next = spadas->elts[x][y][z];
    if (spadas->elts[x][y][z] != NULL) {
        spadas->elts[x][y][z]->previous = elt;
    }
    spadas->elts[x][y][z] = elt;
}

void spadas_update(void *s, void *key, position_t *n_position, position_t *o_position) {
    spadas_t *spadas = (spadas_t *) s;
    int o_x = (int) o_position->x / spadas->cell_width;
    int o_y = (int) o_position->y / spadas->cell_width;
    int o_z = (int) o_position->z / spadas->cell_width;
    int n_x = (int) n_position->x / spadas->cell_width;
    int n_y = (int) n_position->y / spadas->cell_width;
    int n_z = (int) n_position->z / spadas->cell_width;

    if ((o_x != n_x) || (o_y != n_y) || (o_z != n_z)) {

        /* remove old node entry */
        spadas_delete(s, key, o_position);
        
        /* insert node to a new row */
        spadas_insert(s, key, n_position);

    }
}


/* ************************************************** */
/* ************************************************** */
void spadas_delete(void *s, void *key, position_t *position) {
    spadas_t *spadas = (spadas_t *) s;
    int x = (int) position->x / spadas->cell_width;
    int y = (int) position->y / spadas->cell_width;
    int z = (int) position->z / spadas->cell_width;
    spadas_elt_t *elt = spadas->elts[x][y][z];
    
    while ((elt != NULL) && (elt->key != key)) {
        elt = elt->next;
    }
    
    if (elt == NULL) {
        return;
    }

    if (elt->previous != NULL) {
        elt->previous->next = elt->next;
    } else {
        spadas->elts[x][y][z] = elt->next;
    }
    if (elt->next != NULL) {
        elt->next->previous = elt->previous;
    } 
    
    mem_fs_dealloc(mem_spadas_elts, elt);
}


/* ************************************************** */
/* ************************************************** */
void *spadas_rangesearch(void *s, void *key, position_t *position, double range) {
    spadas_t *spadas = (spadas_t *) s;
    void *das;
    int i, j, k;
    
    if ((das = das_create()) == NULL) {
        return NULL;
    }

    if (spadas->cell_width != -1) {
        int x = (int) position->x / spadas->cell_width;
        int y = (int) position->y / spadas->cell_width;
        int z = (int) position->z / spadas->cell_width;

        int adj = range / spadas->cell_width;

        int m_x = MAX(0, x - adj);
        int M_x = MIN(spadas->cell_nbr_x - 1, x + adj);
        int m_y = MAX(0, y - adj);
        int M_y = MIN(spadas->cell_nbr_y - 1, y + adj);
        int m_z = MAX(0, z - adj);
        int M_z = MIN(spadas->cell_nbr_z - 1, z + adj);

        for (i = m_x; i <= M_x; i++) {
            for (j = m_y; j <= M_y; j++) {
                for (k = m_z; k <= M_z; k++) {
                    spadas_elt_t *elt = spadas->elts[i][j][k];
                    
                    while (elt != NULL) {
                        
                        if (elt->key == key) {
                            elt = elt->next;
                            continue;
                        }

                        if (distance(&(elt->position), position) <= range) {
                            das_insert(das, elt->key);
                        }
                      
                        elt = elt->next;
                    }   
                }
            }
        }
    }

    return das;
}
