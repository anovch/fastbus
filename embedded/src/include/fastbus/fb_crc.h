/*
 * crc.h
 *
 *  Created on: 2010-12-15
 *      Author: sj
 */

#ifndef CRC_H_INCLUDED
#define CRC_H_INCLUDED

#include <inttypes.h>

typedef struct
{
	uint64_t poly;
	uint64_t last_value;
	uint8_t power;
} Fb_Crc_T;

Fb_Crc_T* crc_init(Fb_Crc_T* crc, uint64_t poly);
uint64_t crc_calculate(Fb_Crc_T* crc, uint64_t value);
void crc_reset(Fb_Crc_T* crc);
uint64_t crc_last(Fb_Crc_T* crc);

#endif /* CRC_H_ */
