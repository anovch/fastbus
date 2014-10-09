/*
 * fb_util.c
 *
 *  Created on: 24-09-2014
 *      Author: an
 */
#include "fb_util.h"


void fb_bitmask_set(uint8_t* buffer, uint8_t index, uint8_t value){
	uint8_t bit;
	uint8_t address =  index / 8;
	uint8_t shift_bit =  index % 8;

	bit = 0x01 << shift_bit;
	if (value) {
		buffer[address] |= bit;
	}
	else {
		buffer[address] &= ~bit;
	}

}


uint8_t fb_bitmask_check(uint8_t* buffer, uint8_t lastindex) {

	uint8_t bit = 0x01;
	uint8_t offset = 0;

	for(int i=0;i<=lastindex;i++) {
		if((buffer[offset] & bit) == 0)
			return 0;
		if (bit == 0x80) {
			bit = 0x01;
			offset++;
		}
		else {
			bit = bit<<1;
		}

	}

	return 1;
}
