/*
 * test1.c
 *
 *  Created on: 18-09-2014
 *      Author: an
 */
#include <stdio.h>
#include <string.h>

#include "fb_packet.h"
#include "fb_frame.h"
#include "fb_const.h"

const char* mess="1234567890qwertyuiopasdfghjklzxcvbnm";
const char* mess1="1234567890qwertyuiopasdfghjklzxcvbnm1";
const char* mess2="12";
const char* mess3="123";
const char* mess4="1234";


Fb_Mem_Manager_inh_T mem = {
		malloc,
		free,
};



static void show_frameparam(const char* str, Fb_Frame_T frame, Fb_Packet_T pack) {
	fb_packet_write(pack, (uint8_t*)str, (uint16_t)strlen(str));
	fb_frame_calculate(frame);
	uint16_t frame_amount  = fb_frame_get_frame_amount(frame);
	uint16_t last_frame_size = fb_frame_get_last_frame_size(frame);
	printf(" strlen=%i, frame_amount=%i, last_frame_size=%i\n\r", (uint16_t)strlen(str), frame_amount, last_frame_size);

}


static void show_frameindex(const char* str, Fb_Frame_T frame, Fb_Packet_T pack) {
	fb_packet_write(pack, (uint8_t*)str, (uint16_t)strlen(str));
	fb_frame_calculate(frame);


	uint16_t frame_amount  = fb_frame_get_frame_amount(frame);
	for(int i=0;i<frame_amount;i++) {
		fb_frame_from_packet(frame, i);
		uint8_t* data = fb_frame_get_data(frame);
		uint16_t size = fb_frame_get_data_size(frame);

		printf("%i'",i);
		for(int y=0;y<(size);y++) {
			printf("%c",*data);
			data++;
		}
		printf("' ");
	}
	printf("\n\r");

}

void test1() {
	printf("Test 1 \n\r");
	Fb_Packet_T pack = fb_packet_new(&mem,10);

	uint16_t result;
	if ((result = fb_packet_write(pack, (uint8_t*)mess, (uint16_t)strlen(mess))) != FB_ERROR_SIZE) {
		printf("1. Error fb_packet_write: %i \n\r", result);
	}
	printf("1. fb_packet_write pass\n\r");


	fb_packet_free(&mem, pack);
	pack = fb_packet_new(&mem,1024);
	if ((result = fb_packet_write(pack, (uint8_t*)mess, (uint16_t)strlen(mess))) != FB_ERROR_NONE) {
		printf("2. Error fb_packet_write: %i \n\r", result);
	}
	printf("2. fb_packet_write pass\n\r");



	Fb_Frame_T frame = fb_frame_new(&mem,3);
	fb_frame_set_packet(frame, pack);

	printf("mess "); show_frameparam(mess, frame, pack);
	printf("mess1 "); show_frameparam(mess1, frame, pack);
	printf("mess2 "); show_frameparam(mess2, frame, pack);
	printf("mess3 "); show_frameparam(mess3, frame, pack);
	printf("mess4 "); show_frameparam(mess4, frame, pack);

	printf("------------------------\n\r");


	printf("mess %s \n\r",mess);
	printf("mess "); show_frameindex(mess, frame, pack);

	printf("mess1 %s \n\r",mess1);
	printf("mess1 "); show_frameindex(mess1, frame, pack);

	printf("mess2 %s \n\r",mess2);
	printf("mess2 "); show_frameindex(mess2, frame, pack);

	printf("mess3 %s \n\r",mess3);
	printf("mess3 "); show_frameindex(mess3, frame, pack);

	printf("mess4 %s \n\r",mess4);
	printf("mess4 "); show_frameindex(mess4, frame, pack);



}
