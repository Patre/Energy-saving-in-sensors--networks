/**
 *  \file   ioctl_message.c
 *  \brief  ioctl message format structure
 *  \author Quentin Lampin & Elyes Ben Hamida
 *  \date   2009
 **/

#include "internals.h"



/* ************************************************** */
/* ************************************************** */
ioctl_message_t *ioctl_message_create(int type, int size, int real_size, void *body) {
    ioctl_message_t *message = (ioctl_message_t *) malloc(sizeof(ioctl_message_t));
    message->type = type;
    message->size = size;
    message->real_size = real_size;
    message->body = body;
    return message;
}

void ioctl_message_body_duplicate(void *message,void *pointer){
    memcpy(pointer, get_ioctl_message_body(message),
            get_ioctl_message_size(message));
}

void ioctl_message_dealloc(void *message){
    free(((ioctl_message_t *)message)->body);
    free((ioctl_message_t *)message);
}


/* ************************************************** */
/* ************************************************** */
int get_ioctl_message_type(void *message){
    return ((ioctl_message_t *)message)->type;
}

int get_ioctl_message_size(void *message){
    return ((ioctl_message_t *)message)->size;
}

int get_ioctl_message_real_size(void *message){
    return ((ioctl_message_t *)message)->real_size;
}

void *get_ioctl_message_body(void *message){
    return ((ioctl_message_t *)message)->body;
}
