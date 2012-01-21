/**
 *  \file   mem_fs.h
 *  \brief  Fixed size memory management module declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __mem_fs__
#define __mem_fs__


/**
 * \brief Clean the mem_fs module. Done by the wsnet core.
 **/ 
void mem_fs_clean(void);


/**
 * \brief Declare a memory allocation slice. 
 * \param size the size of the memory blcoks that will be allocated using this slice.
 * \return An opaque pointer to the memory slice.
 **/
void *mem_fs_slice_declare(int size);


/**
 * \brief Allocate a memory block from a slice. 
 * \param slice the opaque pointer to the memory slice.
 * \return The allocated memory block.
 **/
void *mem_fs_alloc(void *slice);


/**
 * \brief Deallocate a memory block. 
 * \param slice the opaque pointer to the memory slice.
 * \return pointer the  memory block to deallocate.
 **/
void mem_fs_dealloc(void *slice, void *pointer);


#endif //__mem_fs__
