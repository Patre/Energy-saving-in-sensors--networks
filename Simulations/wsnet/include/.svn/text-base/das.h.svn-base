/**
 *  \file   das.h
 *  \brief  DAta Structure module declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __das__
#define __das__


/** \typedef das_delete_func_t
 * \brief The prototype of a delete function 
 **/
typedef int (* das_delete_func_t)(void *, void *);


/**
 * \brief Initialize the das module. Done by the wsnet core.
 * \return 0 if success, -1 otherwise.
 **/ 
int das_init(void);


/**
 * \brief Create an empty data structure.
 * \return An opaque pointer to the data structure.
 **/ 
void *das_create(void);


/**
 * \brief Destroy a data structure. Objects in the structure are not deallocated.
 * \param das the opaque pointer to the data structure.
 **/ 
void das_destroy(void *das);


/**
 * \brief Return the number of elements in the data structure.
 * \param das the opaque pointer to the data structure.
 * \return The number of elements in the data structure.
 **/
int das_getsize(void *das);


/**
 * \brief Insert an object in the data structure.
 * \param das the opaque pointer to the data structure.
 * \param data the object to insert.
 **/
void das_insert(void *das, void *data);


/**
 * \brief Remove an arbitrary object from the data structure.
 * \param das the opaque pointer to the data structure.
 * \return An arbitrary object.
 **/
void *das_pop(void *das);


/**
 * \brief Remove the oldest object from the data structure.
 * \param das the opaque pointer to the data structure.
 * \return The oldest object.
 **/
void *das_pop_FIFO(void *das);


/**
 * \brief Tell if an object belong to the data structure.
 * \param das the opaque pointer to the data structure.
 * \param data the object to find.
 * \return 1 if the object is in the data structure, 0 otherwise.
 **/
int das_find(void *das, void *data);


/**
 * \brief Remove a particular object from the data structure.
 * \param das the opaque pointer to the data structure.
 * \param data the object to remove.
 **/
void das_delete(void *das, void *data);


/**
 * \brief Remove objects selected by a delete function from the data structure.
 * \param das the opaque pointer to the data structure.
 * \param delete the function that selects objects to be removed.
 * \param arg an argument passed to the delete function.
 **/
void das_selective_delete(void *d, das_delete_func_t delete, void *arg);


/**
 * \brief Initialize a data structure traversal.
 * \param das the opaque pointer to the data structure.
 **/
void das_init_traverse(void *das);


/**
 * \brief Traverse the data structure.
 * \param das the opaque pointer to the data structure.
 * \return The next object in the traversal.
 **/
void *das_traverse(void *das);


#endif //__das__
