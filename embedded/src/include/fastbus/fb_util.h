/*
 * fb_util.h
 *
 *  Created on: 24-09-2014
 *      Author: an
 */

#ifndef FB_UTIL_H_
#define FB_UTIL_H_

#include <stdio.h>
#include <inttypes.h>

void fb_bitmask_set(uint8_t* buffer, uint8_t index, uint8_t value);
uint8_t fb_bitmask_check(uint8_t* buffer, uint8_t lastindex);

#endif /* FB_UTIL_H_ */
