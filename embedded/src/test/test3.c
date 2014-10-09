/*
 * test3.c
 *
 *  Created on: 23-09-2014
 *      Author: an
 */
#include <stdio.h>
#include <string.h>

#include "fb_session.h"
#include "fb_packet.h"
#include "fb_frame.h"
#include "fb_const.h"


extern Fb_Mem_Manager_inh_T mem;

extern const char* mess;


static void printstate(Fb_Session_T sess) {
	uint8_t* mask;
	uint16_t sizemask;
	uint8_t state;
	uint8_t lastframe;

	mask = fb_session_get_packet_state(sess,&state, &sizemask, &lastframe);
	if (state == FB_PACK_INCOMPLETED) {
		printf("|FB_PACK_INCOMPLETED|\nmask=");
	}
	if (state == FB_PACK_INIT) {
		printf("|FB_PACK_INIT|\nmask=");
	}
	if (state == FB_PACK_COMPLETED) {
		printf("|FB_PACK_COMPLETED|\nmask=");
	}
	for(int i=0;i<sizemask;i++) {
		printf("%x", mask[i]);
	}
	printf("\n");

}

void test3() {
	printf("Test 3 \n\r");


	uint8_t backdata[1024];
	uint16_t backsize = 0;


	Fb_Session_T src_sess = fb_session_new(&mem, fb_transport_ascii_get_inh(), 1024, 8);
	Fb_Session_T dst_sess = fb_session_new(&mem, fb_transport_ascii_get_inh(), 1024, 8);

	fb_session_set_idsender(src_sess, 0);
	fb_session_set_idreceiver(src_sess, 5);


	fb_session_write(src_sess, (uint8_t*) mess, (uint16_t) strlen(mess));

	printf("Dane: '%s'\n", mess);

	fb_session_start_receive(dst_sess);

	uint16_t size;
	uint8_t* data;


	uint8_t* mask;
	uint16_t sizemask;
	uint8_t state;
	uint8_t lastframe;

	uint8_t index=0;
	while((data = fb_session_get_next_part(src_sess, &size))!=0) {



		if (index == 4) {
			// pomijamy ramke
			printf("src %i !!! pomijamy ramke !!!\n",index);
			backsize = size;
			memcpy(backdata,data,size);
			index++;
			continue;
		}


		printf("src %i'",index);
		for (int y = 0; y < (size); y++) {
			printf("%c", *data);
			fb_session_put(dst_sess,*data);
			mask = fb_session_get_packet_state(dst_sess,&state, &sizemask, &lastframe);
			if (state == FB_PACK_COMPLETED) {
				printf("|COMPLETED|");
			}
			data++;
		}
		printf("' \n");

		index++;
	}



	printf("1 odebrano '%s'\n", fb_session_read(dst_sess, &size));

	printstate(dst_sess);


	if (backsize !=0) {
		// dohodzi przepuszczona ramka
		for (int y = 0; y < (backsize); y++) {
			printf("%c", backdata[y]);
			fb_session_put(dst_sess,backdata[y]);
			mask = fb_session_get_packet_state(dst_sess,&state, &sizemask, &lastframe);
			if (state == FB_PACK_COMPLETED) {
				printf("|COMPLETED|");
			}
		}
		printf("' \n");
	}

	printstate(dst_sess);

	printf("2 odebrano '%s'\n", fb_session_read(dst_sess, &size));

}
