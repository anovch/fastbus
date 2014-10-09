/*
 * fb_session.c
 *
 *  Created on: 20 wrz 2014
 *      Author: an
 */


#include "fb_session.h"
#include "fb_const.h"


struct Fb_Session_T{
	Fb_Frame_T frame;
	Fb_Packet_T packet;
	Fb_Transport_Frame_T transport;

	uint16_t id_source;
	uint16_t id_destination;

	uint16_t index;
	Fb_Mem_Manager_inh_T* memman;


};


Fb_Session_T fb_session_new(Fb_Mem_Manager_inh_T* memman, Fb_transport_frame_inh_T *inh , uint16_t packet_capacity, uint16_t frame_capacity) {

	Fb_Session_T result;

	if ((result = memman->malloc(sizeof(struct Fb_Session_T)))) {
		result->memman = memman;
		result->packet = fb_packet_new(memman, packet_capacity);
		result->frame = fb_frame_new(memman, frame_capacity);
		result->transport = fb_transport_frame_new(memman,inh,result->frame);
		fb_frame_set_packet(result->frame, result->packet);

	}
	return result;
}


uint16_t fb_session_get_idsender(Fb_Session_T session){
	if (!session)
		return 0;

	return fb_frame_get_idsender(session->frame);
}

void fb_session_set_idsender(Fb_Session_T session, uint16_t id_sender){
	if (session) {
		fb_frame_set_idsender(session->frame,id_sender);
	}
}

uint16_t fb_session_get_idreceiver(Fb_Session_T session){
	if (!session)
		return 0;

	return fb_frame_get_idreceiver(session->frame);
}

void fb_session_set_idreceiver(Fb_Session_T session, uint16_t id_receiver){
	if (session) {
		fb_frame_set_idreceiver(session->frame, id_receiver);
	}
}


void fb_session_write(Fb_Session_T session, uint8_t* data, uint16_t size) {
	if (!session)
		return;
	fb_packet_write(session->packet, data, size);
	fb_frame_calculate(session->frame);
	session->index = 0;
}


uint8_t* fb_session_read(Fb_Session_T session,  uint16_t* size) {
	if (!session)
		return 0;

	*size = fb_packet_get_data_size(session->packet);
	return fb_packet_get_data(session->packet);

}

uint8_t fb_session_read_state(Fb_Session_T session) {
	return 0;
}

uint8_t* fb_session_get_next_part(Fb_Session_T session,  uint16_t* size) {
	if (!session) {
		*size = 0;
		return 0;
	}


	uint16_t amount = fb_frame_get_frame_amount(session->frame);

	if (session->index<amount) {
		fb_frame_from_packet(session->frame, session->index);
		fb_transport_set_from_frame(session->memman, session->transport,session->frame);

		*size = fb_transport_get_data_size(session->transport);

		session->index++;
		return fb_transport_get_data(session->transport);;
	}
	*size = 0;
	return 0;
}


uint8_t* fb_session_get_part(Fb_Session_T session,  uint16_t* size) {
	if (!session) {
		*size = 0;
		return 0;
	}

	*size = fb_transport_get_data_size(session->transport);
	return 	fb_transport_get_data(session->transport);
}


void fb_session_start_receive(Fb_Session_T session) {
	if (!session)
		return;
	fb_frame_start_receive(session->frame);
}

uint8_t fb_session_put(Fb_Session_T session,  uint8_t data) {
	if (!session) {
		return 0;
	}

	fb_transport_put(session->memman, session->transport, data);

	if (fb_transport_get_state(session->transport) == FB_TRANS_STATE_COMPLETED) {
		// ramka odebrana
		fb_transport_set_to_frame(session->memman, session->transport,session->frame);
		fb_frame_to_packet(session->frame);
		return FB_TRANS_STATE_COMPLETED;
	}

	if (fb_transport_get_state(session->transport) == FB_TRANS_STATE_CRC_ERROR) {
		return FB_TRANS_STATE_CRC_ERROR;
	}

	return FB_TRANS_STATE_INCOMPLETED;
}


uint8_t* fb_session_get_packet_state(Fb_Session_T session,  uint8_t* state,  uint16_t* sizemask, uint8_t* lastframe) {
	if (!session)
		return 0;

	*state = fb_frame_get_state(session->frame);
	*lastframe = fb_frame_get_last_frame(session->frame);
	return fb_frame_get_mask(session->frame,sizemask);

}

