/*
 * fb_transport_frame_ascii.h
 *
 *  Created on: 20 wrz 2014
 *      Author: an
 */

#ifndef FB_TRANSPORT_FRAME_ASCII_H_
#define FB_TRANSPORT_FRAME_ASCII_H_
#include "fb_mem_manager.h"
#include "fb_transport_frame.h"
#include "fb_frame.h"

typedef struct Fb_Transport_Ascii_Frame_T* Fb_Transport_Ascii_Frame_T;


Fb_transport_frame_inh_T* fb_transport_ascii_get_inh();
void fb_transport_ascii_frame_init(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame);
void fb_transport_ascii_set_from_frame(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame);
void fb_transport_ascii_set_to_frame(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame);
void fb_transport_ascii_put(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, uint8_t data);
uint16_t fb_transport_ascii_get_size(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame);

#endif /* FB_TRANSPORT_FRAME_ASCII_H_ */
