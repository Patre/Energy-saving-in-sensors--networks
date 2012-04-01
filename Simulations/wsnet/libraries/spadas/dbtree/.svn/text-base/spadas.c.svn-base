/**
 *  \file   spadas.h
 *  \brief  Space PArtitioning DAta Structure module: Dynamic Balanced Tree
 *  \author Elyes Ben Hamida
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
#define MAX(a,b) ((a > b) ? (a) :(b))
#define MIN(a,b) ((a < b) ? (a) : (b))

#define SET(value, bit)   ((value) |= (1 << (bit)))
#define CLEAR(value, bit) ((value) &= ~(1 << (bit)))
#define GET(value, bit)   ((value) & (1 << (bit)))

/* ************************************************** */
/* ************************************************** */
typedef struct _spadas_node {
    void                *key;
    position_t           pos;
    struct _spadas_node *next;
    struct _spadas_node *previous;
} spadas_node_t;


typedef struct _spadas_tree {
    int                  code;
    position_t           pos;
    int                  size;
    int                  height;
    spadas_node_t       *nodes_list;
    struct _spadas_tree *left;
    struct _spadas_tree *right;
} spadas_tree_t;


typedef struct _spadas {
    position_t       area;
    double           range;
    int              size;
    double           cell_width;
    int              cell_nbr_x;
    int              cell_nbr_y;
    int              cell_nbr_z;
    int            **cache_table;
    spadas_tree_t   *nodes;
} spadas_t;


/* ************************************************** */
/* ************************************************** */
int spadas_get_Zvalue(int x, int y);
int spadas_get_Zvalue_cache(spadas_t *s, int x, int y);
void spadas_get_LITMAX_BIGMIN(spadas_t *s, int min_code, int max_code, position_t *min_pos, position_t *max_pos, int *BIGMIN, position_t *BIGMIN_pos, int *LITMAX, position_t *LITMAX_pos, int code);


/* ************************************************** */
/* ************************************************** */
int spadas_init(void) {
    return 0;
}

void spadas_clean(void) {
}


/* ************************************************** */
/* ************************************************** */
void *spadas_create(position_t *area, double range) {
    spadas_t *spadas;
    int i = 0;
    int j = 0;
    
    if ((spadas = (spadas_t *) malloc(sizeof(spadas_t))) == NULL) {
        return NULL;
    }
    
    if (area->z > 0) {
        fprintf(stderr,"[Spadas/DBtree]: 3D space filling curve is not implemented yet ! using 2D transformation...\n");
    }
    
    spadas->area.x = area->x;
    spadas->area.y = area->y;
    spadas->area.z = area->z;

    spadas->range = range;
    spadas->size = 0;
    spadas->nodes = NULL;

    spadas->cell_width = (int) ceil(range / 2.0);

    spadas->cell_nbr_x = MAX ((int) ceil(area->x / spadas->cell_width), 1);
    spadas->cell_nbr_y = MAX ((int) ceil(area->y / spadas->cell_width), 1);
    spadas->cell_nbr_z = MAX ((int) ceil(area->z / spadas->cell_width), 1);

    /* defining cache table for space filling curve computation */
    spadas->cache_table = (int **) malloc(sizeof(int*) * spadas->cell_nbr_y);
    for (i = 0; i < spadas->cell_nbr_y; i++) {
        spadas->cache_table[i] = (int *) malloc(sizeof(int) * spadas->cell_nbr_x);
    }    
    
    /* init cache table */
    for (i = 0; i < spadas->cell_nbr_y; i++) {
        for (j = 0; j < spadas->cell_nbr_x; j++) {
            spadas->cache_table[i][j] = spadas_get_Zvalue(j, i);
        }
    }
    
    return (void *) spadas;
}

/* ************************************************** */
/* ************************************************** */
void spadas_destroy_recursive(spadas_tree_t *s) {
    if (s != NULL) {
        spadas_node_t *tmp;
        
        if (s->left != NULL) {
            spadas_destroy_recursive(s->left);
        }
        if (s->right != NULL) {
            spadas_destroy_recursive(s->right);    
        }

        while((tmp = s->nodes_list) != NULL) {
            s->nodes_list = tmp->next;
            free(tmp);
        }
        
        free(s);
    }
}

void spadas_destroy(void *s) {
    spadas_t *spadas = (spadas_t *) s;
    spadas_destroy_recursive(spadas->nodes);
    free(spadas);
}


/* ************************************************** */
/* ************************************************** */
int spadas_height(spadas_tree_t *s) {
    if (s == NULL) {
        return -1;
    } else {
        return s->height;
    }
}

spadas_tree_t* spadas_singlerotate_withleft(spadas_tree_t *s2) {
    spadas_tree_t *s1;

    s1 = s2->left;
    s2->left = s1->right;
    s1->right = s2;
    s2->height = MAX(spadas_height(s2->left), spadas_height(s2->right)) + 1;
    s1->height = MAX(spadas_height(s1->left), s2->height) + 1;
    return s1;
}

spadas_tree_t* spadas_singlerotate_withright(spadas_tree_t *s1) {
    spadas_tree_t *s2;
    
    s2 = s1->right;
    s1->right = s2->left;
    s2->left = s1;
    s1->height = MAX(spadas_height(s1->left), spadas_height(s1->right)) + 1;
    s2->height = MAX(spadas_height(s2->right), s1->height) + 1;
    return s2;  
}

spadas_tree_t * spadas_doublerotate_withleft(spadas_tree_t *s) {
    s->left = spadas_singlerotate_withright(s->left);
    return spadas_singlerotate_withleft(s);
}

spadas_tree_t *spadas_doublerotate_withright(spadas_tree_t *s) {
    s->right = spadas_singlerotate_withleft(s->right);
    return spadas_singlerotate_withright(s);
}

spadas_tree_t * spadas_insert_recursive(spadas_tree_t *s, spadas_tree_t *n_elt ) {
    if (s == NULL) {
        s = n_elt;
    } else if (n_elt->code < s->code ) { 
        s->left = spadas_insert_recursive(s->left, n_elt );
        
        if (spadas_height(s->left) - spadas_height(s->right) == 2) {
            if (n_elt->code < s->left->code) {
                s = spadas_singlerotate_withleft(s);
            } else  {
                s = spadas_doublerotate_withleft(s);
            }
        }
    } else if (n_elt->code > s->code ) { 
        s->right = spadas_insert_recursive(s->right, n_elt );
        if (spadas_height(s->right) - spadas_height(s->left) == 2) {
            if (n_elt->code > s->right->code) { 
                s = spadas_singlerotate_withright(s);
            } else  {
                s = spadas_doublerotate_withright(s);
            }
        }
    } else {
        spadas_node_t *elt = (spadas_node_t*) malloc(sizeof(spadas_node_t));

        elt->key      = n_elt->nodes_list->key;
        elt->pos.x    = n_elt->nodes_list->pos.x;
        elt->pos.y    = n_elt->nodes_list->pos.y;
        elt->pos.z    = n_elt->nodes_list->pos.z;
        elt->next     = NULL;
        elt->previous = NULL;
        elt->next = s->nodes_list;

        if (s->nodes_list != NULL) {
            s->nodes_list->previous = elt;
        }
        s->nodes_list = elt;
        
        s->size++;
        free(n_elt);
    }
    
    s->height = MAX(spadas_height(s->left), spadas_height(s->right)) + 1;
    return s;
}


void spadas_insert(void *s, void *key, position_t *position){
    spadas_t *spadas = (spadas_t *) s;
    int x, y, z, code;
    spadas_tree_t* n_elt = (spadas_tree_t*) malloc(sizeof(spadas_tree_t));

    x = (int) position->x / spadas->cell_width;
    y = (int) position->y / spadas->cell_width;
    z = (int) position->z / spadas->cell_width;
    
    code = spadas_get_Zvalue_cache(spadas, x, y);
    
    n_elt->code = code;
    n_elt->pos.x = x;
    n_elt->pos.y = y;
    n_elt->pos.z = z;
    n_elt->size = 1;
    n_elt->height = 0;
    n_elt->left = NULL;
    n_elt->right = NULL;

    n_elt->nodes_list = (spadas_node_t*) malloc(sizeof(spadas_node_t));
    n_elt->nodes_list->key = key;
    n_elt->nodes_list->pos.x = position->x;
    n_elt->nodes_list->pos.y = position->y;
    n_elt->nodes_list->pos.z = position->z;
    n_elt->nodes_list->next = NULL;
    n_elt->nodes_list->previous = NULL;
    
    spadas->nodes = spadas_insert_recursive(spadas->nodes, n_elt);
    spadas->size++;

}


/* ************************************************** */
/* ************************************************** */
void spadas_update(void *s, void *key, position_t *n_position, position_t *o_position) {
    spadas_t *spadas = (spadas_t *) s;

    int o_x = (int) o_position->x / spadas->cell_width;
    int o_y = (int) o_position->y / spadas->cell_width;
    int o_z = (int) o_position->z / spadas->cell_width;
    int n_x = (int) n_position->x / spadas->cell_width;
    int n_y = (int) n_position->y / spadas->cell_width;
    int n_z = (int) n_position->z / spadas->cell_width;

    if ( (o_x != n_x) || (o_y != n_y) || (o_z != n_z) ) {
        /* remove old node entry */
        spadas_delete(s, key, o_position);
        /* insert node to a new row */
        spadas_insert(s, key, n_position);
    }
}


/* ************************************************** */
/* ************************************************** */
void spadas_delete_recursive(spadas_tree_t *s, int code, void *key) {
    if (s != NULL) {
        if (code > s->code) {
            /* go to the right child */
            spadas_delete_recursive(s->right, code, key);
        } else if (code < s->code) {
            /* go to left child */
            spadas_delete_recursive(s->left, code, key);
        } else {
            spadas_node_t *cur   = s->nodes_list;
            spadas_node_t *p_cur = NULL;
            
            while(cur!= NULL && cur->key != key) {
                p_cur = cur;
                cur = cur->next;
            }

            if (cur != NULL) {
                spadas_node_t *tmp = cur;
                
                if (p_cur != NULL) {
                    p_cur->next = cur->next;
                    if (cur->next != NULL) {
                        cur->next->previous = p_cur;
                    }
                    free(tmp);
                    s->size--;
                } else {
                    s->nodes_list = s->nodes_list->next;
                    if (s->nodes_list != NULL) {
                        s->nodes_list->previous = NULL;
                    }
                    free(tmp);
                    s->size--;
                }
            }
        }
    }
}


void spadas_delete(void *s, void *key, position_t *position) {
    spadas_t *spadas = (spadas_t *) s;
    int code, x, y, z;

    x = (int) position->x / spadas->cell_width;
    y = (int) position->y / spadas->cell_width;
    z = (int) position->z / spadas->cell_width;
    
    code = spadas_get_Zvalue_cache(spadas, x, y);
    spadas_delete_recursive(spadas->nodes, code, key);
}


/* ************************************************** */
/* ************************************************** */
int spadas_within_range(position_t *pos, position_t *min_pos, position_t *max_pos) {
    if ((pos->x >= min_pos->x) && (pos->x <= max_pos->x) && 
        (pos->y >= min_pos->y) && (pos->y <= max_pos->y)) {
        return 1;
    } else {
        return -1;
    }
}

/* recursive version where all elements are checked */
void spadas_rangesearch_recursive(spadas_tree_t *s, void *key, position_t *pos,  double range, void *das, int minimum_code, int maximum_code) {
    if (s != NULL) {
        spadas_node_t *tmp = s->nodes_list;

        if (s->code > minimum_code ) {
            spadas_rangesearch_recursive(s->left, key, pos, range, das, minimum_code, maximum_code);
        }

        while (tmp != NULL) {
            if ((distance(&(tmp->pos), pos) <= range) && (tmp->key != key)) { 
                das_insert(das, tmp->key);
            }
            tmp = tmp->next;
        }
        
        if (s->code < maximum_code) {
            spadas_rangesearch_recursive(s->right, key, pos, range, das, minimum_code, maximum_code);
        }
    }
}


/* recursive version where all elements that are within search range are checked */
void spadas_rangesearch_recursive_optimized(spadas_t *spadas, spadas_tree_t *s, void *key, int minimum_code, int maximum_code, 
                                            position_t *pos,  double range, position_t *min_searchrange, 
                                            position_t *max_searchrange, void *das) {
    if (s != NULL) {
        if (s->code > minimum_code) {
            spadas_rangesearch_recursive_optimized(spadas, s->left, key, minimum_code, maximum_code, 
                                                   pos, range, min_searchrange, max_searchrange, das);
        }

        if (s->code < maximum_code) {
            spadas_rangesearch_recursive_optimized(spadas, s->right, key, minimum_code, maximum_code, 
                                                   pos, range, min_searchrange, max_searchrange, das);
        }

        if ((s->code >= minimum_code) && (s->code <= maximum_code) && (spadas_within_range(&(s->pos), min_searchrange, max_searchrange) == 1)) {
            spadas_node_t *tmp = s->nodes_list;

            while (tmp != NULL) {
                if ((distance(&(tmp->pos), pos) <= range) && (tmp->key != key)) { 
                    das_insert(das, tmp->key);
                }
                tmp = tmp->next;
            }
        }
    }
}


/* recursive version with LITMAX and BIGMIN optimization */
void spadas_rangesearch_recursive_LITMAX_BIGMIN(spadas_t *spadas, spadas_tree_t *s, void *key, position_t *pos,  double range, void *das,
                                                int minimum_code, int maximum_code, position_t *min_searchrange, 
                                                position_t *max_searchrange, position_t *omin_searchrange, 
                                                position_t *omax_searchrange, int omin, int omax) {
    if (s != NULL) {
        if (s->code < minimum_code ) {
            spadas_rangesearch_recursive_LITMAX_BIGMIN(spadas, s->right, key, pos, range, das, minimum_code, maximum_code, 
                                                       min_searchrange, max_searchrange, omin_searchrange, omax_searchrange, omin, omax);
        }

        if (s->code > maximum_code) {
            spadas_rangesearch_recursive_LITMAX_BIGMIN(spadas, s->left, key, pos, range, das, minimum_code, maximum_code, 
                                                       min_searchrange, max_searchrange, omin_searchrange, omax_searchrange, omin, omax);
        }

        if (s->code >= minimum_code && s->code <= maximum_code) {
     
            if (spadas_within_range(&(s->pos), omin_searchrange, omax_searchrange) == 1) {
                spadas_node_t *tmp = s->nodes_list;

                while (tmp != NULL) {
                    if (distance(&(tmp->pos), pos) <= range && tmp->key != key) { 
                        das_insert(das, tmp->key);
                    }
                    tmp = tmp->next;
                }
	
                spadas_rangesearch_recursive_LITMAX_BIGMIN(spadas, s->left, key, pos, range, das, minimum_code, maximum_code, 
                                                           min_searchrange, max_searchrange, omin_searchrange, omax_searchrange, omin, omax);
                spadas_rangesearch_recursive_LITMAX_BIGMIN(spadas, s->right, key, pos, range, das, minimum_code, maximum_code, 
                                                           min_searchrange, max_searchrange, omin_searchrange, omax_searchrange, omin, omax);
            } else {
                int BIGMIN = 0, LITMAX = 0;
                position_t BIGMIN_pos, LITMAX_pos;

                spadas_get_LITMAX_BIGMIN(spadas, omin, omax, omin_searchrange, omax_searchrange, 
                                         &BIGMIN, &BIGMIN_pos, &LITMAX, &LITMAX_pos, s->code);

                spadas_rangesearch_recursive_LITMAX_BIGMIN(spadas, s->left, key, pos, range, das, minimum_code, LITMAX, 
                                                           min_searchrange, &LITMAX_pos, omin_searchrange, omax_searchrange, omin, omax);

                spadas_rangesearch_recursive_LITMAX_BIGMIN(spadas, s->right, key, pos, range, das, BIGMIN, maximum_code, 
                                                           &BIGMIN_pos, max_searchrange, omin_searchrange, omax_searchrange, omin, omax);

            }
        }
    }
}


/* performs circular range search*/
void *spadas_rangesearch(void *s, void *key, position_t *position, double range) {
    spadas_t *spadas = (spadas_t *) s;
    position_t min_pos, max_pos;
    void *das;

    int minimum_code;
    int maximum_code;
    /*int code; */

    int node_x = (int) position->x / spadas->cell_width;
    int node_y = (int) position->y / spadas->cell_width;
    int node_z = (int) position->z / spadas->cell_width;
    int adj = (int) ceil(range / spadas->cell_width);
  
    if ((das = das_create()) == NULL) {
        return NULL;
    }
    
    min_pos.x = MAX(0, node_x-adj);
    max_pos.x = MIN(spadas->cell_nbr_x-1, node_x+adj);
    min_pos.y = MAX(0, node_y-adj);
    max_pos.y = MIN(spadas->cell_nbr_y-1, node_y+adj);
    min_pos.z = MAX(0, node_z-adj);
    max_pos.z = MIN(spadas->cell_nbr_z-1, node_z+adj);
  
    /*code = spadas_get_Zvalue_cache(spadas, node_x, node_y); */
    minimum_code = spadas_get_Zvalue_cache(spadas, min_pos.x, min_pos.y);
    maximum_code = spadas_get_Zvalue_cache(spadas, max_pos.x, max_pos.y);
  
    if (spadas->area.z == 0 || spadas->area.z == -1) {
        min_pos.z = 0;
        max_pos.z = 0;
    }
  
    /*
spadas_rangesearch_recursive(spadas->nodes, key, position, range, das, minimum_code, maximum_code);
    spadas_rangesearch_recursive_optimized(spadas, spadas->nodes, key, minimum_code, maximum_code, position, range, &min_pos, &max_pos, das);
    */
    spadas_rangesearch_recursive_LITMAX_BIGMIN(spadas, spadas->nodes, key, position, range, das, minimum_code, maximum_code, &min_pos, &max_pos, &min_pos, &max_pos, minimum_code, maximum_code);

    return das;
}


/* ************************************************** */
/* ************************************************** */

/* function which computes and returns the Z-values associated to x and y */
int spadas_get_Zvalue(int x, int y) {
    int i = 0, j = 31, res = 0;

    x = x & 0x00FF;
    y = y & 0x00FF;

    for (i = 15; i >= 0; i--) {
        if ((GET(x,i) >> i) == 1) {
            SET(res,j);
        }
        j--;
        if ((GET(y,i) >> i) == 1) {
            SET(res,j);
        }
        j--;
    }

    return res;
}

/* function which returns the Z-values from the cache */
int spadas_get_Zvalue_cache(spadas_t *s, int x, int y) {
    return s->cache_table[y][x];
}

/* function which computes LITMIN and BIGMAX */
void spadas_get_LITMAX_BIGMIN(spadas_t *s, int min_code, int max_code, position_t *min_pos, position_t *max_pos, 
                              int *BIGMIN, position_t *BIGMIN_pos, int *LITMAX, position_t *LITMAX_pos, int code) {
    int i = 0, j = 0;

    *LITMAX = min_code;
    LITMAX_pos->x = min_pos->x;
    LITMAX_pos->y = min_pos->y;	

    *BIGMIN = max_code;
    BIGMIN_pos->x = max_pos->x;
    BIGMIN_pos->y = max_pos->y;	

    for (j = min_pos->y; j <= max_pos->y; j++) {
        for (i = min_pos->x; i <= max_pos->x; i++) {
            int tmp = spadas_get_Zvalue_cache(s, i , j);

            if ((tmp > code) && (tmp < *BIGMIN)) {
                *BIGMIN = tmp;
                BIGMIN_pos->x = i;
                BIGMIN_pos->y = j;
            }      
            if ((tmp < code) && (tmp > *LITMAX)) {
                *LITMAX = tmp;
                LITMAX_pos->x = i;
                LITMAX_pos->y = j;	
            }
        }
    }

}

