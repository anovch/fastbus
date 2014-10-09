/*
 * fb_packet.h
 *
 *  Created on: 18-09-2014
 *      Author: an
 */

#ifndef FB_PACKET_H_
#define FB_PACKET_H_

#include <inttypes.h>
#include <stdlib.h>
#include "fb_mem_manager.h"

typedef struct Fb_Packet_T* Fb_Packet_T;

Fb_Packet_T fb_packet_new(Fb_Mem_Manager_inh_T* memman,
		uint16_t capacity);
void fb_packet_free(Fb_Mem_Manager_inh_T* memman, Fb_Packet_T packet);
void fb_packet_resize(Fb_Mem_Manager_inh_T* memman, Fb_Packet_T packet,  uint16_t newcapacity);

uint16_t fb_packet_write(Fb_Packet_T packet, uint8_t* data, uint16_t size);
uint16_t fb_packet_append(Fb_Packet_T packet, uint8_t data);
uint16_t fb_packet_clear(Fb_Packet_T packet);
uint16_t fb_packet_read(Fb_Packet_T packet, uint8_t* data, uint16_t size);
uint8_t* fb_packet_get_data(Fb_Packet_T packet);
uint16_t fb_packet_get_data_size(Fb_Packet_T packet);
void fb_packet_set_data_size(Fb_Packet_T packet, uint16_t size);
uint16_t fb_packet_get_capacity(Fb_Packet_T packet);

uint16_t fb_packet_get_error(Fb_Packet_T packet);

#endif /* FB_PACKET_H_ */
