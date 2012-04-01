/**
 *  \file   packet.c
 *  \brief  Packet management
 *  \author Guillaume Chelius & Elyes Ben Hamida & Quentin Lampin
 *  \date   2007
 **/
#include "internals.h"


/* ************************************************** */
/* ************************************************** */
static void *mem_packet = NULL;
#if (SNR_STEP > 0)
static void *mem_snr = NULL;
#endif /*SNR_STEP */
static packetid_t id = 0;


/* ************************************************** */
/* ************************************************** */
int packet_init(void) {
    if ((mem_packet = mem_fs_slice_declare(sizeof(packet_t))) == NULL) {
        return -1;
    }
#if (SNR_STEP > 0)
    if ((mem_snr = mem_fs_slice_declare(sizeof(double) * SNR_STEP)) == NULL) {
        return -1;
    }
#endif /*SNR_STEP*/

    return 0;
}

int packet_bootstrap(void) {
    return 0;
}

void packet_clean(void) {
}


/* ************************************************** */
/* ************************************************** */

/* edit by Quentin Lampin <quentin.lampin@orange-ftgroup.com> */
packet_t *packet_create(call_t *c, int size, int real_size) {
    packet_t *packet;
 
    packet = (packet_t *) mem_fs_alloc(mem_packet);
    packet->data = (char *) malloc(size);
    memset(packet->data, 0, size);
    packet->noise_mW = NULL;
    packet->ber = NULL;   
    packet->id = id++;
    packet->size = size;
    packet->type = 0;
    if(real_size > 0){
      packet->real_size = real_size;
    }else{
      packet->real_size = 8 * size;
    }
    return packet;
}

packet_t *packet_alloc(call_t *c, int size) {
    return packet_create(c, size, -1);
}

/* end of edition */


void packet_dealloc(packet_t *packet) {
    if (packet->ber) {
#if (SNR_STEP > 0)
        mem_fs_dealloc(mem_snr, packet->noise_mW);
        mem_fs_dealloc(mem_snr, packet->ber);        
#elif (SNR_STEP < 0)
        free(packet->noise_mW);
        free(packet->ber);
#endif /*SNR_STEP*/
    }
    free(packet->data);
    mem_fs_dealloc(mem_packet, packet);
}


/* ************************************************** */
/* ************************************************** */
packet_t *packet_clone(packet_t *packet) {
    packet_t *packet0;

    packet0 = (packet_t *) mem_fs_alloc(mem_packet);
    memcpy(packet0, packet, sizeof(packet_t));
    packet0->data = malloc(packet->size);
    memcpy(packet0->data, packet->data, packet->size);    
    packet0->noise_mW = NULL;
    packet0->ber = NULL;
    
    packet0->id = id++;
    return packet0;
}

packet_t *packet_rxclone(packet_t *packet) {
    packet_t *packet0;

    packet0 = (packet_t *) mem_fs_alloc(mem_packet);
    memcpy(packet0, packet, sizeof(packet_t));
    packet0->data = malloc(packet->size);
    memcpy(packet0->data, packet->data, packet->size);    
#if (SNR_STEP > 0)
    packet0->noise_mW = (double *) mem_fs_alloc(mem_snr);
    packet0->ber = (double *) mem_fs_alloc(mem_snr);
#elif (SNR_STEP < 0)
    packet0->noise_mW = (double *) malloc(ceil(packet->real_size/8) * sizeof(double));
    packet0->ber = (double *) malloc(ceil(packet->real_size/8) * sizeof(double));
#endif /*SNR_STEP*/

    return packet0;
}
