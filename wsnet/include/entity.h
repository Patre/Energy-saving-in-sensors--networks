/**
 *  \file   entity.h
 *  \brief  Entity declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida
 *  \date   2007
 **/
#ifndef __entity_public__
#define __entity_public__

#include <include/types.h>


/**
 * \brief Return the name of an entity.
 * \param c sould be {entity id, -1, -1}.
 * \return The name of the entity.
 **/
char *get_entity_name(call_t *c);


/**
 * \brief Return the private data associated to an entity.
 * \param c sould be {entity id, -1, -1}.
 * \return A (void *) pointer to the entity private data.
 **/
void *get_entity_private_data(call_t *c);


/**
 * \brief Associate a private data memory space with an entity.
 * \param c sould be {entity id, -1, -1}.
 * \param data a (void *) pointer to the private memory space.
 **/
void set_entity_private_data(call_t *c, void *data);


/**
 * \brief Return the private data associated to an (entity, node).
 * \param c sould be {entity id, node id, -1}.
 * \return A (void *) pointer to the (entity, node) private data.
 **/
void *get_node_private_data(call_t *c);


/**
 * \brief Associate a private data memory space with an (entity, node).
 * \param c sould be {entity id, node id, -1}.
 * \param data a (void *) pointer to the private memory space.
 **/
void set_node_private_data(call_t *c, void *data);


/**
 * \brief Return the type of an entity.
 * \param c sould be {entity id, -1, -1}.
 * \return The entity type.
 **/
int get_entity_type(call_t *c);


/**
 * \brief Return an array containing the entities that are up "c->entity" in "c->node".
 * \param c sould be {entity id, node id, -1}.
 * \return An array of entity ids.
 **/
array_t *get_entity_bindings_up(call_t *c);


/**
 * \brief Return an array containing the entities that are down "c->entity" in "c->node".
 * \param c sould be {entity id, node id, -1}.
 * \return An array of entity ids.
 **/
array_t *get_entity_bindings_down(call_t *c);


/**
 * \brief Return an array containing the application entities in "c->node".
 * \param c sould be {-1, node id, -1}.
 * \return An array of entity ids.
 **/
array_t *get_application_entities(call_t *c);


/**
 * \brief Return an array containing the routing entities in "c->node".
 * \param c sould be {-1, node id, -1}.
 * \return An array of entity ids.
 **/
array_t *get_routing_entities(call_t *c);


/**
 * \brief Return an array containing the mac entities in "c->node".
 * \param c sould be {-1, node id, -1}.
 * \return An array of entity ids.
 **/
array_t *get_mac_entities(call_t *c);


/**
 * \brief Return an array containing the radio entities in "c->node".
 * \param c sould be {-1, node id, -1}.
 * \return An array of entity ids.
 **/
array_t *get_radio_entities(call_t *c);


/**
 * \brief Return an array containing the antenna entities in "c->node".
 * \param c sould be {-1, node id, -1}.
 * \return An array of entity ids.
 **/
array_t *get_antenna_entities(call_t *c);


/**
 * \brief Return an array containing the energy entity in "c->node".
 * \param c sould be {-1, node id, -1}.
 * \return An entity id.
 **/
entityid_t get_energy_entity(call_t *c);


/**
 * \brief Return an array containing the mobility entity in "c->node".
 * \param c sould be {-1, node id, -1}.
 * \return An entity id.
 **/
entityid_t get_mobility_entity(call_t *c);


/**
 * \brief Return the number of entities that are linked up with "c->entity" in "c->node".
 * \param c sould be {entity id, node id, -1}.
 * \return The number of linked up entities.
 * \deprecated Should use get_entity_bindings_up() instead.
 **/
int get_entity_links_up_nbr(call_t *c);


/**
 * \brief Return an array containing the entity ids of the entities linked up with "c->entity" in "c->node".
 * \param c sould be {entity id, node id, -1}.
 * \return Array of entity ids.
 * \deprecated Should use get_entity_bindings_up() instead.
 **/
entityid_t *get_entity_links_up(call_t *c);


/**
 * \brief Return the number of entities that are linked down with "c->entity" in "c->node".
 * \param c sould be {entity id, node id, -1}.
 * \return The number of linked down entities.
 * \deprecated Should use get_entity_bindings_down() instead.
 **/
int get_entity_links_down_nbr(call_t *c);


/**
 * \brief Return an array containing the entity ids of the entities linked down with "c->entity" in "c->node".
 * \param c sould be {entity id, node id, -1}.
 * \return Array of entity ids.
 * \deprecated Should use get_entity_bindings_down() instead.
 **/
entityid_t *get_entity_links_down(call_t *c);


#endif //__entity_public__
