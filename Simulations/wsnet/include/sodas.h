/**
 *  \file   sodas.h
 *  \brief  SOrted DAta Structure module declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __sodas__
#define __sodas__


/** \typedef sodas_compare_t
 * \brief The prototype of a comparison function 
 **/
typedef int (* sodas_compare_t) (void *key0, void *key1);


/**
 * \brief Initialize the sodas module. Done by the wsnet core.
 * \return 0 if success, -1 otherwise.
 **/ 
int sodas_init(void);


/**
 * \brief Create an empty sodas structure.
 * \param comapre the comparison function used to sort the objects.
 * \return An opaque pointer to the sodas structure.
 **/ 
void *sodas_create(sodas_compare_t compare);


/**
 * \brief Destroy a sodas structure. Objects in the structure are not deallocated.
 * \param sodas the opaque pointer to the sodas structure.
 **/ 
void sodas_destroy(void *sodas);


/**
 * \brief Insert an object in the sodas structure.
 * \param sodas the opaque pointer to the sodas structure.
 * \param key the object key.
 * \param data the object to insert.
 **/
void sodas_insert(void *sodas, void *key, void *data);


/**
 * \brief Remove the first object from the sodas structure.
 * \param sodas the opaque pointer to the data structure.
 * \return The first object.
 **/
void *sodas_pop(void *sodas);


/**
 * \brief Remove a particular object from the sodas structure.
 * \param sodas the opaque pointer to the data structure.
 * \param key the key of the object to remove.
 **/
void *sodas_delete(void *sodas, void *key);


/**
 * \brief Show the first object in the sodas structure. The object is not removed.
 * \param sodas the opaque pointer to the data structure.
 * \return The first object.
 **/ 
void *sodas_see_first(void *sodas);


#endif //__sodas__
