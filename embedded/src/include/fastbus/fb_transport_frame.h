/*
 * fb_transport_frame.h
 *
 *  Created on: 20 wrz 2014
 *      Author: an
 */

#ifndef FB_TRANSPORT_FRAME_H_
#define FB_TRANSPORT_FRAME_H_

#include <inttypes.h>
#include <stdlib.h>
#include "fb_mem_manager.h"
#include "fb_transport_frame.h"
#include "fb_frame.h"
#include "fb_packet.h"


typedef struct Fb_Transport_Frame_T* Fb_Transport_Frame_T;

typedef struct
{
	void (*init)(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame);
	void (*set_from_frame)(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame);
	void (*set_to_frame)(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame);
	void (*put)(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, uint8_t data);
	uint16_t (*get_size)(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame);
} Fb_transport_frame_inh_T;

Fb_Transport_Frame_T fb_transport_frame_new(Fb_Mem_Manager_inh_T* memman, Fb_transport_frame_inh_T *inh, Fb_Frame_T frame);
void fb_transport_frame_free(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame);
void fb_transport_frame_init(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame);
void* fb_transport_get_transport_data(Fb_Transport_Frame_T transport_frame);
void fb_transport_set_transport_data(Fb_Transport_Frame_T transport_frame, void* transport_data);


void fb_transport_set_inh(Fb_Transport_Frame_T transport_frame, Fb_transport_frame_inh_T *inh);


// funkcje interfejsu
void fb_transport_set_from_frame(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame);
void fb_transport_set_to_frame(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame);
void fb_transport_put(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, uint8_t data);
uint16_t fb_transport_get_size(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame);

Fb_Packet_T fb_transport_get_buffer(Fb_Transport_Frame_T transport_frame);

uint8_t* fb_transport_get_data(Fb_Transport_Frame_T transport_frame);
uint16_t fb_transport_get_data_size(Fb_Transport_Frame_T transport_frame);

uint8_t fb_transport_get_state(Fb_Transport_Frame_T transport_frame);
void fb_transport_set_state(Fb_Transport_Frame_T transport_frame, uint8_t state);


#endif /* FB_TRANSPORT_FRAME_H_ */
