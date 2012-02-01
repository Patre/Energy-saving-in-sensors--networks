/**
 *  \file   spadas.h
 *  \brief  Space PArtitioning DAta Structure module declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __spadas__
#define __spadas__

#include <include/types.h>


/**
 * \brief Initialize the spadas module. Done by the wsnet core.
 * \return 0 if success, -1 otherwise.
 **/ 
int spadas_init(void);


/**
 * \brief Create an empty spadas structure.
 * \param area the area where objects of the spadas will be.
 * \param range a distance value used for spadas optimization.
 * \return An opaque pointer to the spadas structure.
 **/ 
void *spadas_create(position_t *area, double range);


/**
 * \brief Destroy a spadas structure. Objects in the structure are not deallocated.
 * \param spadas the opaque pointer to the spadas structure.
 **/ 
void spadas_destroy(void *spadas);


/**
 * \brief Insert an object in the spadas structure.
 * \param spadas the opaque pointer to the spadas structure.
 * \param data the object to insert.
 * \param position the object position.
 **/
void spadas_insert(void *spadas, void *data, position_t *position);


/**
 * \brief Update an object position in the spadas structure.
 * \param spadas the opaque pointer to the spadas structure.
 * \param data the object.
 * \param n_position the new object position.
 * \param o_position the old object position.
 **/
void spadas_update(void *spadas, void *data, position_t *n_position, position_t *o_position);


/**
 * \brief Remove an object from the spadas structure.
 * \param spadas the opaque pointer to the spadas structure.
 * \param data the object to remove.
 * \param position the object position.
 **/
void spadas_delete(void *spadas, void *data, position_t *position);


/**
 * \brief Return all objects close to a given position.
 * \param spadas the opaque pointer to the spadas structure.
 * \param data an object which can be excluded from the search.
 * \param position the central position.
 * \param range distance to the central position.
 * \return A das containing all objects that are at distance <= range from the central position.
 **/
void *spadas_rangesearch(void *spadas, void *data, position_t *position, double range);


#endif //__spadas__
