/*
 * fb_session.h
 *
 *  Created on: 23-09-2014
 *      Author: an
 */

#ifndef FB_SESSION_H_
#define FB_SESSION_H_


#include <inttypes.h>
#include <stdlib.h>
#include "fb_mem_manager.h"
#include "fb_transport_frame.h"
#include "fb_packet.h"
#include "fb_frame.h"

typedef struct Fb_Session_T* Fb_Session_T;

Fb_Session_T fb_session_new(Fb_Mem_Manager_inh_T* memman,Fb_transport_frame_inh_T *inh, uint16_t packet_capacity, uint16_t frame_capacity);

// send ----------
void fb_session_write(Fb_Session_T session, uint8_t* data, uint16_t size);
uint8_t* fb_session_read(Fb_Session_T session,  uint16_t* size);
uint8_t* fb_session_get_next_part(Fb_Session_T session,  uint16_t* size);
uint8_t* fb_session_get_part(Fb_Session_T session,  uint16_t* size);
// receive --------
uint8_t fb_session_put(Fb_Session_T session,  uint8_t data);
void fb_session_start_receive(Fb_Session_T session);
uint8_t* fb_session_get_packet_state(Fb_Session_T session,  uint8_t* state,  uint16_t* sizemask, uint8_t* lastframe);

uint16_t fb_session_get_idsender(Fb_Session_T session);
void fb_session_set_idsender(Fb_Session_T session, uint16_t id_sender);
uint16_t fb_session_get_idreceiver(Fb_Session_T session);
void fb_session_set_idreceiver(Fb_Session_T session, uint16_t id_receiver);

#endif /* FB_SESSION_H_ */
