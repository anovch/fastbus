/*
 * test2.c
 *
 *  Created on: 20 wrz 2014
 *      Author: an
 */

#include <stdio.h>
#include <string.h>

#include "fb_packet.h"
#include "fb_frame.h"
#include "fb_const.h"
#include "fb_transport_frame.h"
#include "fb_transport_frame_ascii.h"

extern Fb_Mem_Manager_inh_T mem;

extern const char* mess;

void test2() {
	printf("Test 2 \n\r");

	// inicjalizacja
	Fb_Packet_T src_pack = fb_packet_new(&mem, 1024);
	Fb_Packet_T dst_pack = fb_packet_new(&mem, 1024);
	Fb_Frame_T src_frame = fb_frame_new(&mem, 7);
	Fb_Frame_T dst_frame = fb_frame_new(&mem, 7);
	Fb_Transport_Frame_T src_transport = fb_transport_frame_new(&mem,fb_transport_ascii_get_inh(),src_frame);
	Fb_Transport_Frame_T dst_transport = fb_transport_frame_new(&mem,fb_transport_ascii_get_inh(),dst_frame);

	fb_frame_set_packet(src_frame, src_pack);
	fb_frame_set_packet(dst_frame, dst_pack);
	fb_frame_set_idsender(src_frame,0);
	fb_frame_set_idreceiver(src_frame,5);


	// wgranie danych
	fb_packet_write(src_pack, (uint8_t*) mess, (uint16_t) strlen(mess)); //!!!!!!!!!!!!
	fb_frame_calculate(src_frame); //!!!!!!!!!!!!

	printf("do wyslania '%s'", mess);
	printf("\n");


	// wybieranie
	uint16_t frame_amount = fb_frame_get_frame_amount(src_frame);
	for (int i = 0; i < frame_amount; i++) {
		// wybieramy ramke -----------------------------
		fb_frame_from_packet(src_frame, i); // !!!!!!!!!!!!!!!!!!!!!!!!! packet -> frame
		uint8_t* data = fb_frame_get_data(src_frame);
		uint16_t size = fb_frame_get_data_size(src_frame);
		printf("%i src frame'", i);
		for (int y = 0; y < (size); y++) {
			printf("%c", *data);
			data++;
		}
		printf("' ");
		printf(" idsource=%i, idreceiver=%i, index=%i, size=%i", fb_frame_get_idsender(src_frame), fb_frame_get_idreceiver(src_frame),fb_frame_get_index(src_frame), fb_frame_get_data_size(src_frame));
		printf("\n");


		// kodujemy do postaci transportowej  -----------------------------
		fb_transport_set_from_frame(&mem, src_transport,src_frame); // !!!!!!!!!!!!!!!!!!!!!!!!! frame -> transport
		data = fb_transport_get_data(src_transport);
		size = fb_transport_get_data_size(src_transport);
		printf("%i src trans'", i);
		for (int y = 0; y < (size); y++) {
			printf("%c", *data);
			data++;
		}
		printf("' ");
		printf("\n");


		// wysylamy ------------------------
		data = fb_transport_get_data(src_transport);
		for (int y = 0; y < fb_transport_get_data_size(src_transport); y++) {
			fb_transport_put(&mem, dst_transport,*data); // !!!!!!!!!!!!!!!!!!!!!!!!! transport -> transport
			data++;
		}

		// odebrana ramka trasportowa -----------------------------
		data = fb_transport_get_data(dst_transport);
		size = fb_transport_get_data_size(dst_transport);
		printf("%i dst trans'", i);
		for (int y = 0; y < (size); y++) {
			printf("%c", *data);
			data++;
		}
		printf("' ");

		uint8_t state = fb_transport_get_state(dst_transport);
		if (state == FB_TRANS_STATE_COMPLETED) {
			printf("OK");
		}
		else {
			printf("ERR=%i",state);
		}
		printf("\n");


		// trasportowa do ramki -----------------------------
		fb_transport_set_to_frame(&mem, dst_transport,dst_frame); // !!!!!!!!!!!!!!!!!!!!!!!!! transport->frame
		data = fb_frame_get_data(dst_frame);
		size = fb_frame_get_data_size(dst_frame);
		printf("%i dst frame'", i);
		for (int y = 0; y < (size); y++) {
			printf("%c", *data);
			data++;
		}
		printf("' ");
		printf(" idsource=%i, idreceiver=%i, index=%i, size=%i", fb_frame_get_idsender(dst_frame), fb_frame_get_idreceiver(dst_frame),fb_frame_get_index(dst_frame), fb_frame_get_data_size(dst_frame));
		printf("\n");

		// ramka do pakietu
		fb_frame_to_packet(dst_frame); // !!!!!!!!!!!!!!!!!!!!!!!!! frame -> pakiet

	}



	printf("odebrano '%s'", fb_packet_get_data(dst_pack));
	printf("\n");



}
