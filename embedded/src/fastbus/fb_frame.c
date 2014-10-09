/*
 * fb_frame.c
 *
 *  Created on: 18-09-2014
 *      Author: an
 */
#include "fb_frame.h"
#include "fb_const.h"
#include "fb_packet.h"
#include "fb_util.h"
#include "string.h"


#define FRAMESIZEMASK 32

struct Fb_Frame_T{
	Fb_Packet_T buffer;
	Fb_Packet_T packet;

	// bierzacy index ramki
	uint16_t index;
	// adres nadajnika
	uint16_t id_sender;
	// adres odbiornika
	uint16_t id_receiver;

	uint16_t error;


	//--------------calculate (do generowania frejmów z pakietu)-------------------
	// ilosc frejmow
	uint16_t frame_amount;
	// ilosc bajt w ostatnim frejmie
	uint16_t last_frame_size;

	//--------------  read (do generowania pakeitu z frejmów)-------------------
	//stan - odebrania pakietu
	//FB_PACK_INIT -> odbieranie w trakcie. Nie by³ odebrany ostatni pakiet
	//FB_PACK_INCOMPLETED -> byl odebrany ostatni pakiet ale nie wszystkie pakety odebrane
	//FB_PACK_COMPLETED -> paket odebrany w komplecie
	uint8_t state;
	// maksylany adres odebranych danych
	uint16_t write_offset;
	// index ostatniego pakietu
	uint16_t last_frame_index;
	// odebrane frejmy (maksymalnie w pakiecie 255 frejmów)
	uint8_t framemask[FRAMESIZEMASK];
	// ostatni frejm
	uint8_t lastframe;

};


Fb_Frame_T fb_frame_new(Fb_Mem_Manager_inh_T* memman, uint16_t capacity) {

	Fb_Frame_T result;

	if ((result = memman->malloc(sizeof(struct Fb_Frame_T)))) {
		result->buffer = fb_packet_new(memman,capacity);
		result->index = 0;
		result->write_offset = 0;
		result->error = FB_ERROR_NONE;

	}
	return result;
}

void fb_frame_free(Fb_Mem_Manager_inh_T* memman, Fb_Frame_T frame){
	if (!frame)
		return;
	fb_packet_free(memman, frame->buffer);
	memman->free(frame);
}

void fb_frame_set_packet(Fb_Frame_T frame, Fb_Packet_T pack){
	if (!frame)
		return;
	frame->packet = pack;
	fb_frame_calculate(frame);
}


uint16_t fb_frame_get_capacity(Fb_Frame_T frame) {
	if (!frame)
		return 0;

	return fb_packet_get_capacity(frame->buffer);
}


uint16_t fb_frame_calculate(Fb_Frame_T frame){
	if (!frame)
		return FB_ERROR_OTHER;
	if (!frame->packet) {
		frame->error = FB_ERROR_NOTPACKET;
		return FB_ERROR_NOTPACKET;
	}


	uint16_t framecapacity = fb_packet_get_capacity(frame->buffer);
	uint16_t packsize = fb_packet_get_data_size(frame->packet);

	uint16_t result = packsize / framecapacity + 1;
	frame->last_frame_size = (packsize % framecapacity);
	if (frame->last_frame_size == 0) {
		result -= 1;
		frame->last_frame_size = framecapacity;
	}

	frame->frame_amount = result;

	frame->error = FB_ERROR_NONE;
	return FB_ERROR_NONE;

}


uint16_t fb_frame_get_frame_amount(Fb_Frame_T frame){
	if (!frame)
		return 0;

	return frame->frame_amount;
}

uint16_t fb_frame_get_last_frame_size(Fb_Frame_T frame){
	if (!frame)
		return 0;

	return frame->last_frame_size;
}

/*
 * Ustawienie danych frejmu po indexie
 * index zaczyna siê od 0
 *
 */
void fb_frame_from_packet(Fb_Frame_T frame, uint16_t index){
	if (!frame)
			return;
	if (index >= frame->frame_amount) {
		fb_packet_clear(frame->buffer);
		frame->error = FB_ERROR_SIZE;
		return;
	}

	uint16_t framesize = fb_packet_get_capacity(frame->buffer);
	uint16_t offset = (framesize*index);
	uint8_t* packdata = fb_packet_get_data(frame->packet);

	if ((index+1) == frame->frame_amount) {
		// ostatni frejm
		framesize = frame->last_frame_size;
	}

	frame->index = index;

	fb_packet_write(frame->buffer, (packdata+offset), framesize);
	frame->error = FB_ERROR_NONE;

}


void fb_frame_set_last_frame(Fb_Frame_T frame, uint8_t value){
	if (!frame)
			return;
	frame->lastframe = value;
}

uint8_t fb_frame_get_last_frame(Fb_Frame_T frame){
	return frame->lastframe;
}

void fb_frame_set_state(Fb_Frame_T frame, uint8_t state){
	if (!frame)
			return;
	frame->state = state;
}

uint8_t fb_frame_get_state(Fb_Frame_T frame){
	return frame->state;
}

uint8_t* fb_frame_get_mask(Fb_Frame_T frame, uint16_t* size){
	if (!frame){
		return 0;
	}

	*size = FRAMESIZEMASK;
	return frame->framemask;
}


void fb_frame_start_receive(Fb_Frame_T frame){
	if (!frame)
			return;
	frame->write_offset = 0;
	frame->last_frame_index = 0;
	frame->lastframe = 0;
	frame->state = FB_PACK_INIT;
	memset(frame->framemask, 0, FRAMESIZEMASK);
}


/*
 * Ustawienie danych do pakietu z frejmu
 *
 */
void fb_frame_to_packet(Fb_Frame_T frame){
	if (!frame)
			return;

	uint16_t framesize = fb_packet_get_capacity(frame->buffer);
	uint16_t offset = (framesize*frame->index);
	uint8_t* packdata = fb_packet_get_data(frame->packet);
	uint8_t* buffdata = fb_packet_get_data(frame->buffer);

	for(int i=0;i<fb_packet_get_data_size(frame->buffer);i++) {
		packdata[offset++] = buffdata[i];
		if (offset > frame->write_offset) {
			frame->write_offset = offset;
			fb_packet_set_data_size(frame->packet, offset);
		}
	}


	fb_bitmask_set(frame->framemask,frame->index, 1);

	if (frame->lastframe) {
		if(fb_bitmask_check(frame->framemask,frame->index)) {
			frame->state = FB_PACK_COMPLETED;
		}
		else {
			frame->state = FB_PACK_INCOMPLETED;
		}
		frame->last_frame_index = frame->index;
	}

}



Fb_Packet_T fb_frame_get_buffer(Fb_Frame_T frame) {
	if (!frame)
		return 0;

	return frame->buffer;
}

uint8_t* fb_frame_get_data(Fb_Frame_T frame) {
	if (!frame)
		return 0;

	return fb_packet_get_data(frame->buffer);
}

uint16_t fb_frame_get_data_size(Fb_Frame_T frame) {
	if (!frame)
		return 0;

	return fb_packet_get_data_size(frame->buffer);
}

uint8_t fb_frame_is_last_index(Fb_Frame_T frame){
	if (!frame)
		return 0;
	if ((frame->index+1) == frame->frame_amount)
		return 1;
	return 0;
}

uint16_t fb_frame_get_index(Fb_Frame_T frame){
	if (!frame)
		return 0;

	return frame->index;
}

void fb_frame_set_index(Fb_Frame_T frame, uint16_t index){
	if (frame) {
		frame->index = index;
	}
}

uint16_t fb_frame_get_idsender(Fb_Frame_T frame){
	if (!frame)
		return 0;

	return frame->id_sender;
}

void fb_frame_set_idsender(Fb_Frame_T frame, uint16_t id_sender){
	if (frame) {
		frame->id_sender = id_sender;
	}
}

uint16_t fb_frame_get_idreceiver(Fb_Frame_T frame){
	if (!frame)
		return 0;

	return frame->id_receiver;
}

void fb_frame_set_idreceiver(Fb_Frame_T frame, uint16_t id_receiver){
	if (frame) {
		frame->id_receiver = id_receiver;
	}
}
