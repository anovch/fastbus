/*
 * fb_transport_frame.c
 *
 *  Created on: 20 wrz 2014
 *      Author: an
 */

#include "fb_transport_frame.h"

#include "fb_packet.h"
#include "fb_frame.h"

struct Fb_Transport_Frame_T{
	Fb_Frame_T frame;
	Fb_Packet_T buffer;
	Fb_transport_frame_inh_T *inh;

	uint8_t state;
	void* transport_data;

};


Fb_Transport_Frame_T fb_transport_frame_new(Fb_Mem_Manager_inh_T* memman, Fb_transport_frame_inh_T *inh, Fb_Frame_T frame) {

	Fb_Transport_Frame_T result;

	if ((result = memman->malloc(sizeof(struct Fb_Transport_Frame_T)))) {
		result->inh = inh;
		uint16_t capacity = fb_transport_get_size(memman,result,frame);
		result->buffer =  fb_packet_new(memman,capacity);
		fb_transport_frame_init(memman,result);
	}
	return result;
}

void fb_transport_frame_free(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame){
	if (!transport_frame)
		return;
	fb_packet_free(memman, transport_frame->buffer);
	free(transport_frame);
}

void fb_transport_frame_init(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame) {
	if (!transport_frame)
		return;
	if (transport_frame->inh) {
		transport_frame->inh->init(memman, transport_frame);
	}
	else {
		// brak interfejsu
	}

}


void fb_transport_set_inh(Fb_Transport_Frame_T transport_frame, Fb_transport_frame_inh_T *inh){
	if (transport_frame) {
		transport_frame->inh = inh;
	}
}

void fb_transport_set_from_frame(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame){
	if (!transport_frame)
		return;
	transport_frame->frame = frame;
	if (transport_frame->inh) {
		transport_frame->inh->set_from_frame(memman, transport_frame,  frame);
	}
	else {
		// brak interfejsu
	}

}

void fb_transport_set_to_frame(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame){
	if (!transport_frame)
		return;
	transport_frame->frame = frame;
	if (transport_frame->inh) {
		transport_frame->inh->set_to_frame(memman, transport_frame,  frame);
	}
	else {
		// brak interfejsu
	}

}

void fb_transport_put(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, uint8_t data) {
	if (!transport_frame)
		return;
	if (transport_frame->inh) {
		transport_frame->inh->put(memman, transport_frame,  data);
	}
	else {
		// brak interfejsu
	}

}

uint16_t fb_transport_get_size(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame) {
	if (!transport_frame)
		return 0;
	if (transport_frame->inh) {
		return transport_frame->inh->get_size(memman, transport_frame,  frame);
	}
	else {
		// brak interfejsu
	}
	return 0;

}


Fb_Packet_T fb_transport_get_buffer(Fb_Transport_Frame_T transport_frame){
	if (!transport_frame)
		return 0;
	return transport_frame->buffer;
}

uint8_t* fb_transport_get_data(Fb_Transport_Frame_T transport_frame) {
	if (!transport_frame)
		return 0;
	return fb_packet_get_data(transport_frame->buffer);
}

uint16_t fb_transport_get_data_size(Fb_Transport_Frame_T transport_frame) {
	if (!transport_frame)
		return 0;

	return fb_packet_get_data_size(transport_frame->buffer);
}


uint8_t fb_transport_get_state(Fb_Transport_Frame_T transport_frame) {
	if (!transport_frame)
		return 0;
	return transport_frame->state;
}

void fb_transport_set_state(Fb_Transport_Frame_T transport_frame, uint8_t state) {
	if (!transport_frame)
		return;
	transport_frame->state = state;
}


void* fb_transport_get_transport_data(Fb_Transport_Frame_T transport_frame){
	if (!transport_frame)
		return 0;
	return transport_frame->transport_data;
}

void fb_transport_set_transport_data(Fb_Transport_Frame_T transport_frame, void* transport_data){
	if (!transport_frame)
		return;
	transport_frame->transport_data = transport_data;
}
