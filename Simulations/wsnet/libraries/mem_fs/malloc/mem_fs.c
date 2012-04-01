/**
 *  \file   mem_fs.c
 *  \brief  Fixed size memory management without preallocation
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#include <stdlib.h>
#include <stdio.h>

#include <include/mem_fs.h>


/* ************************************************** */
/* ************************************************** */
typedef struct _slice {
    int            size; /* size of a container */
    struct _slice *next; /* next declared slice */
} slice_t;

slice_t *slices = NULL;


/* ************************************************** */
/* ************************************************** */
void mem_fs_clean(void) {
    /* free all declared slices */
    while (slices) {
        slice_t *slice = slices;
        slices = slice->next;
        free(slice);
    }

    return;
}


/* ************************************************** */
/* ************************************************** */
void *mem_fs_slice_declare(int size) {
    slice_t *slice = slices;
    
    /* protect against too small containers*/
    if (size <= 0) {
        fprintf(stderr, "mem_fs: size too small (%d) in slice declaration (mem_fs_slice_declare())\n", size);
        return NULL;
    }
    
    /* search for an already declared slice */
    while (slice) {
        if (slice->size == size) {
            return slice;
        }
        slice = slice->next;
    }
    
    /* allocated new slice */
    if ((slice = (slice_t *) malloc(sizeof(slice_t))) == NULL) {
        fprintf(stderr, "mem_fs: malloc error in slice declaration (mem_fs_slice_declare())\n");
        return NULL;
    }
    slice->size = size;
    
    /* update slices */
    slice->next = slices;
    slices = slice;
    
    return (void *) slice;
}


/* ************************************************** */
/* ************************************************** */
void *mem_fs_alloc(void *slice) {
    return malloc(((slice_t *) slice)->size);
}

void mem_fs_dealloc(void *slice, void *pointer) {
    free(pointer);
}
