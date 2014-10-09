/*
 * fb_transport_frame_ascii.c
 *
 *  Created on: 20 wrz 2014
 *      Author: an
 */
#include "fb_transport_frame_ascii.h"
#include "fb_crc.h"
#include "fb_const.h"

#define FB_START_CONDITION '<'
#define FB_STOP_CONDITION '>'

#define HEADERSIZE 3

struct Fb_Transport_Ascii_Frame_T{
	uint16_t index;
};


static Fb_transport_frame_inh_T transport_ascii_inh = {
		fb_transport_ascii_frame_init,
		fb_transport_ascii_set_from_frame,
		fb_transport_ascii_set_to_frame,
		fb_transport_ascii_put,
		fb_transport_ascii_get_size
};


static inline uint8_t integer_to_digit(uint8_t value)
{
	if (value < 10)
	{
		return value + '0';
	}
	else
	{
		return value - 10 + 'A';
	}
}


static inline uint8_t digit_to_integer(uint8_t value) {
	uint8_t result = 0;
	if (value>='a'){
		result =  value - 'a' + 10;
	}
	else
	if (value>='A'){
		result =  value - 'A' + 10;
	}
	else
	if (value>='0'){
		result =  value- '0';
	}

	return result;
}


static uint8_t comm_trans_crc_calc(uint8_t* header, uint8_t* buffor, uint32_t size) {
	Fb_Crc_T crc;
	crc_init(&crc, 0x107);
	crc_reset(&crc);

	for(int i=0;i<3;i++) {
		crc_calculate(&crc, header[i]);
	}

	for(int i=0;i<size;i++) {
		crc_calculate(&crc, buffor[i]);
	}
	return crc_last(&crc);

}


static uint8_t comm_trans_crc_calc_ascii(Fb_Packet_T buffer, uint8_t* crcinframe) {
	Fb_Crc_T crc;
	crc_init(&crc, 0x107);
	crc_reset(&crc);

	uint16_t size = (fb_packet_get_data_size(buffer) - 4)/2;
	uint8_t* data = fb_packet_get_data(buffer)+1;
	uint8_t bhi;
	uint8_t blo;
	uint16_t offset = 0;

	for(int i=0;i<size;i++) {
		bhi = digit_to_integer(data[offset++]);
		blo = digit_to_integer(data[offset++]);
		crc_calculate(&crc, ((bhi<<4)|blo));
	}

	// CRC z ramki
	bhi = digit_to_integer(data[offset++]);
	blo = digit_to_integer(data[offset++]);
	(*crcinframe) = ((bhi<<4)|blo);
	return crc_last(&crc);

}


static void converter_hbyte_to_char(uint8_t value, uint8_t* buffer) {
	buffer[0] = integer_to_digit((value >> 4) & 0x0f );
	buffer[1] = integer_to_digit(value & 0x0f);
}

void fb_transport_ascii_frame_init(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame) {
	if (!transport_frame)
		return;
	Fb_Transport_Ascii_Frame_T result;

	if ((result = memman->malloc(sizeof(struct Fb_Transport_Ascii_Frame_T)))) {
		fb_transport_set_transport_data(transport_frame,result);
	}

}

/*
 * ustawienie frejmu z transportu
 */
void fb_transport_ascii_set_to_frame(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame){
	if (!transport_frame)
		return;

	Fb_Packet_T buffer = fb_transport_get_buffer(transport_frame);
	Fb_Packet_T frame_buffer = fb_frame_get_buffer(frame);
	fb_packet_clear(frame_buffer);

	uint16_t size = (fb_packet_get_data_size(buffer) - 4)/2; // bez '<' '>' i 'xx' sumy kontrolnej
	uint8_t* data = fb_packet_get_data(buffer)+1;
	uint8_t bhi;
	uint8_t blo;
	uint16_t offset = 0;

	// ---------- header
	fb_frame_set_idsender(frame, digit_to_integer(data[offset++]));
	fb_frame_set_idreceiver(frame, digit_to_integer(data[offset++]));

	bhi = digit_to_integer(data[offset++]);
	blo = digit_to_integer(data[offset++]);
	uint8_t flags = ((bhi<<4)|blo);

	if ((flags&0x02)!=0) {
		fb_frame_set_last_frame(frame,1);
	}

	bhi = digit_to_integer(data[offset++]);
	blo = digit_to_integer(data[offset++]);
	fb_frame_set_index(frame,((bhi<<4)|blo));

	// ---------- data
	for(int i=0;i<(size-HEADERSIZE);i++) {
		bhi = digit_to_integer(data[offset++]);
		blo = digit_to_integer(data[offset++]);
		fb_packet_append(frame_buffer,((bhi<<4)|blo));
	}

}


/*
 * ustawienie transportu z frejmu
 */
void fb_transport_ascii_set_from_frame(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame){
	if (!transport_frame)
		return;


	uint8_t header[HEADERSIZE];

	Fb_Packet_T buffer = fb_transport_get_buffer(transport_frame);
	uint16_t size = fb_frame_get_data_size(frame);
	uint16_t newcapacity = size * 2 + 10;
	fb_packet_resize(memman, buffer,newcapacity);

	uint8_t* dst = fb_packet_get_data(buffer);
	uint8_t* src = fb_frame_get_data(frame);
	uint16_t offset = 0;


	dst[offset++] = FB_START_CONDITION;
	header[0] = (fb_frame_get_idsender(frame) <<4) | fb_frame_get_idreceiver(frame);
	header[1] = 0x0;
	if (fb_frame_is_last_index(frame)) {
		header[1] |= 0x2;
	}
	header[2] = fb_frame_get_index(frame);
	if (header[2] == 0) {
		header[1] |= 0x1;
	}

	for(int i=0;i<HEADERSIZE;i++) {
		converter_hbyte_to_char(header[i], (dst+offset));
		offset+=2;
	}

	for(int i=0;i<size;i++) {
		converter_hbyte_to_char(src[i], (dst+offset));
		offset+=2;
	}

	uint8_t crc = comm_trans_crc_calc(header, src,size);
	converter_hbyte_to_char(crc,(dst+offset));
	offset+=2;
	dst[offset++] = FB_STOP_CONDITION;

	fb_packet_write(buffer, dst, offset);




}

uint16_t fb_transport_ascii_get_size(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, Fb_Frame_T frame) {

	if (!frame)
		return 0;
	Fb_Packet_T packet = fb_frame_get_buffer(frame);
	uint16_t size = fb_packet_get_capacity(packet);
	return size*2+10; // '<nnxxyy' 'cc>' nn-adres xx-flags yy-index cc-CRC
}

void fb_transport_ascii_put(Fb_Mem_Manager_inh_T* memman, Fb_Transport_Frame_T transport_frame, uint8_t data) {

	if (!transport_frame)
		return;

	Fb_Transport_Ascii_Frame_T transport_data = fb_transport_get_transport_data(transport_frame);

	Fb_Packet_T buffer = fb_transport_get_buffer(transport_frame);
	uint16_t capacity = fb_packet_get_capacity(buffer);
	if (capacity == 0) {
		fb_packet_resize(memman, buffer,16);
	}
	capacity = fb_packet_get_capacity(buffer);
	uint16_t size = fb_packet_get_data_size(buffer);
	if (capacity == size) {
		fb_packet_resize(memman, buffer,capacity*2);
	}


	if (data == FB_START_CONDITION) {
		transport_data->index = 0;
		fb_transport_set_state(transport_frame,FB_TRANS_STATE_START);
		fb_packet_clear(buffer);
	}

	fb_packet_append(buffer, data);

	if (data == FB_STOP_CONDITION) {
		uint8_t crcinframe;
		uint8_t crccalc = comm_trans_crc_calc_ascii(buffer, &crcinframe);
		if (crcinframe == crccalc) {
			fb_transport_set_state(transport_frame,FB_TRANS_STATE_COMPLETED);
		}
		else {
			fb_transport_set_state(transport_frame,FB_TRANS_STATE_CRC_ERROR);
		}
	}

}

Fb_transport_frame_inh_T* fb_transport_ascii_get_inh(){
	return &transport_ascii_inh;
}
