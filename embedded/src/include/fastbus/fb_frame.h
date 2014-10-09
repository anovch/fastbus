/*
 * fb_frame.h
 *
 *  Created on: 18-09-2014
 *      Author: an
 */

#ifndef FB_FRAME_H_
#define FB_FRAME_H_

#include <inttypes.h>
#include <stdlib.h>
#include "fb_packet.h"
#include "fb_mem_manager.h"


typedef struct Fb_Frame_T* Fb_Frame_T;


Fb_Frame_T fb_frame_new(Fb_Mem_Manager_inh_T* memman, uint16_t capacity);
void fb_frame_free(Fb_Mem_Manager_inh_T* memman, Fb_Frame_T frame);

void fb_frame_set_packet(Fb_Frame_T frame, Fb_Packet_T pack);

uint16_t fb_frame_get_capacity(Fb_Frame_T frame);

uint16_t fb_frame_calculate(Fb_Frame_T frame);
uint16_t fb_frame_get_frame_amount(Fb_Frame_T frame);
uint16_t fb_frame_get_last_frame_size(Fb_Frame_T frame);

uint8_t* fb_frame_get_data(Fb_Frame_T frame);
uint16_t fb_frame_get_data_size(Fb_Frame_T frame);
Fb_Packet_T fb_frame_get_buffer(Fb_Frame_T frame);

// ustawienie ramki z pakietu
void fb_frame_from_packet(Fb_Frame_T frame, uint16_t index);
// ustawienie danych pakietu z ramki
void fb_frame_to_packet(Fb_Frame_T frame);
// start odebrania pakietu
void fb_frame_start_receive(Fb_Frame_T frame);

void fb_frame_set_last_frame(Fb_Frame_T frame, uint8_t value);
uint8_t fb_frame_get_last_frame(Fb_Frame_T frame);
void fb_frame_set_state(Fb_Frame_T frame, uint8_t state);
uint8_t fb_frame_get_state(Fb_Frame_T frame);
uint8_t* fb_frame_get_mask(Fb_Frame_T frame, uint16_t* size);



uint16_t fb_frame_get_index(Fb_Frame_T frame);
void fb_frame_set_index(Fb_Frame_T frame, uint16_t index);
uint8_t fb_frame_is_last_index(Fb_Frame_T frame);
uint16_t fb_frame_get_idsender(Fb_Frame_T frame);
void fb_frame_set_idsender(Fb_Frame_T frame, uint16_t id_sender);
uint16_t fb_frame_get_idreceiver(Fb_Frame_T frame);
void fb_frame_set_idreceiver(Fb_Frame_T frame, uint16_t id_receiver);


#endif /* FB_FRAME_H_ */
