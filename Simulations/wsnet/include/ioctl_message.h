/**
 *  \file   message.h
 *  \brief  message format structure
 *  \author Quentin Lampin & Elyes Ben Hamida
 *  \date   2009
 **/
#ifndef __message_public__
#define	__message_public__



/** 
 * \brief Create an ioctl message.
 * \param type type of the message.
 * \param size size of the message.
 * \param real_size real size of the message.
 * \param body body of the message.
 **/
ioctl_message_t *ioctl_message_create(int type, int size, int real_size, void *body);

/** 
 * \brief Deallocate an ioctl message.
 * \param message the message to deallocate.
 **/
void ioctl_message_dealloc(void *message);

/** 
 * \brief Duplicate an ioctl message.
 * \param message the message to duplicate.
 **/
void ioctl_message_body_duplicate(void *message,void *pointer);


/** 
 * \brief Return the message body.
 * \param message the message.
 **/
void *get_ioctl_message_body(void *message);

/** 
 * \brief Return the message type.
 * \param message the message.
 **/
int get_ioctl_message_type(void *message);

/** 
 * \brief Return the size of the message body.
 * \param message the message.
 **/
int get_ioctl_message_size(void *message);

/** 
 * \brief Return the real size of the message body.
 * \param message the message.
 **/
int get_ioctl_message_real_size(void *message);

#endif

