/**
 *  \file   hadas.h
 *  \brief  Hashed DAta Structure module declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __hadas__
#define __hadas__


/** \typedef hash_hash_t
 * \brief The prototype of a hash function 
 **/
typedef unsigned long (* hash_hash_t) (void *key);


/** \typedef hash_equal_t
 * \brief The prototype of an equal function 
 **/
typedef int (* hash_equal_t) (void *key0, void *key1);


/**
 * \brief Initialize the hadas module. Done by the wsnet core.
 * \return 0 if success, -1 otherwise.
 **/ 
int hadas_init(void);


/**
 * \brief Create an empty hadas structure.
 * \param hash the function used to hash object keys.
 * \param equal the function used to decide equality between keys.
 * \return An opaque pointer to the hadas structure.
 **/ 
void *hadas_create(hash_hash_t hash, hash_equal_t equal);


/**
 * \brief Destroy a hadas structure. Objects and keys in the structure are not deallocated.
 * \param hadas the opaque pointer to the hadas structure.
 **/ 
void hadas_destroy(void *hadas);


/**
 * \brief Insert a (key,object) in the hadas structure.
 * \param hadas the opaque pointer to the data structure.
 * \param key the key of the object to insert.
 * \param data the object to insert.
 **/
void hadas_insert(void *hadas, void *key, void *data);


/**
 * \brief Return the object associated to a key in the hadas structure.
 * \param hadas the opaque pointer to the data structure.
 * \param key the key of the object to get.
 * \return The object.
 **/
void *hadas_get(void *hadas, void *key);


/**
 * \brief Remove the object associated to a key in the hadas structure.
 * \param hadas the opaque pointer to the data structure.
 * \param key the key of the object to be removed.
 **/
void hadas_delete(void *hadas, void *key);


#endif //__hadas__
