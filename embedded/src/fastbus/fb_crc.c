/*
 * crc.c
 *
 *  Created on: 2010-12-15
 *      Author: sj
 */

#include "fb_crc.h"

Fb_Crc_T* crc_init(Fb_Crc_T* crc, uint64_t poly)
{
	if (crc && poly)
	{
		uint8_t len = sizeof(poly)*8 - 1;
		/*trzeba policzyæ jakiej d³ugoœci jest wielomian*/
		while (!(poly & ((uint64_t)1 << len)))
		{
			len--;
		}

		crc->poly = poly;
		crc->power = len;
		crc->last_value = 0;

		return crc;
	}

	return 0;
}

uint64_t crc_calculate(Fb_Crc_T* crc, uint64_t value)
{
	if (crc)
	{
		uint64_t poly = crc->poly << (crc->power - 1);
		value <<= crc->power;
		value |= crc->last_value;

		for (uint8_t i = (crc->power*2 - 1); i >= crc->power; i--)
		{
			if (value & (1 << i))
			{
				value ^= poly;
			}
			poly >>= 1;
		}

		crc->last_value = value;
		return crc->last_value;
	}

	return 0;
}

void crc_reset(Fb_Crc_T* crc)
{
	if (crc)
	{
		crc->last_value = 0;
	}
}

uint64_t crc_last(Fb_Crc_T* crc)
{
	if (crc)
	{
		return crc->last_value;
	}

	return 0;
}
