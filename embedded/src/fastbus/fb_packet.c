/*
 * packet.c
 *
 *  Created on: 18-09-2014
 *      Author: an
 */
#include <stdio.h>
#include <string.h>

#include "fb_packet.h"
#include "fb_const.h"

struct Fb_Packet_T {

	uint8_t* data;
	uint16_t capacity;
	uint16_t size;

	uint16_t error;

};

Fb_Packet_T fb_packet_new(Fb_Mem_Manager_inh_T* memman,
		uint16_t capacity) {

	Fb_Packet_T result;

	if ((result = memman->malloc(sizeof(struct Fb_Packet_T)))) {
		result->data = memman->malloc(capacity);
		result->capacity = capacity;
		result->size = 0;
		result->error = 0;
		fb_packet_clear(result);
	}
	return result;
}

/*
 * zmiana rozmiaru pakietu z zahowaniem danych
 */
void fb_packet_resize(Fb_Mem_Manager_inh_T* memman, Fb_Packet_T packet, uint16_t newcapacity) {
	if (!packet)
		return;

	if (newcapacity > packet->capacity) {
		uint8_t* ptr = packet->data;
		packet->data = memman->malloc(newcapacity);
		memset(packet->data, 0, newcapacity);
		memcpy(packet->data, ptr, packet->size);
		memman->free(ptr);

	}
	packet->capacity = newcapacity;

}

void fb_packet_free(Fb_Mem_Manager_inh_T* memman, Fb_Packet_T packet) {
	if (!packet)
		return;
	memman->free(packet->data);
	memman->free(packet);

}

uint16_t fb_packet_append(Fb_Packet_T packet, uint8_t data) {
	if (!packet)
		return FB_ERROR_OTHER;

	if (packet->size == packet->capacity) {
		packet->error = FB_ERROR_SIZE;
		return FB_ERROR_SIZE;
	}

	packet->data[packet->size] = data;
	packet->size++;

	packet->error = FB_ERROR_NONE;
	return FB_ERROR_NONE;

}

uint16_t fb_packet_write(Fb_Packet_T packet, uint8_t* data, uint16_t size) {
	if (!packet)
		return FB_ERROR_OTHER;

	if (size > packet->capacity) {
		packet->error = FB_ERROR_SIZE;
		return FB_ERROR_SIZE;
	}

	memcpy(packet->data, data, size);
	packet->size = size;

	packet->error = FB_ERROR_NONE;
	return FB_ERROR_NONE;

}

uint16_t fb_packet_clear(Fb_Packet_T packet) {
	if (!packet)
		return FB_ERROR_OTHER;

	memset(packet->data, 0, packet->capacity);
	packet->size = 0;

	packet->error = FB_ERROR_NONE;
	return FB_ERROR_NONE;

}

uint16_t fb_packet_get_error(Fb_Packet_T packet) {
	return packet->error;
}

uint16_t fb_packet_read(Fb_Packet_T packet, uint8_t* data, uint16_t size) {
	if (!packet)
		return 0;

	memcpy(data, packet->data, packet->size);
	return packet->size;
}

uint8_t* fb_packet_get_data(Fb_Packet_T packet) {
	if (!packet)
		return 0;

	return packet->data;
}

uint16_t fb_packet_get_data_size(Fb_Packet_T packet) {
	if (!packet)
		return 0;

	return packet->size;
}

void fb_packet_set_data_size(Fb_Packet_T packet, uint16_t size) {
	if (!packet)
		return;
	packet->size = size;
}

uint16_t fb_packet_get_capacity(Fb_Packet_T packet) {
	if (!packet)
		return 0;

	return packet->capacity;
}
