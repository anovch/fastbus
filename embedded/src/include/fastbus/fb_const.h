/*
 * fb_const.h
 *
 *  Created on: 18-09-2014
 *      Author: an
 */

#ifndef FB_CONST_H_
#define FB_CONST_H_

// bledy
#define FB_ERROR_NONE 0
#define FB_ERROR_SIZE 1
#define FB_ERROR_NOTPACKET 2
#define FB_ERROR_OTHER 999

// odbieranie stan ramki transportowej
#define FB_TRANS_STATE_START 0
#define FB_TRANS_STATE_INCOMPLETED 1
#define FB_TRANS_STATE_COMPLETED 2
#define FB_TRANS_STATE_CRC_ERROR 3
#define FB_TRANS_STATE_FORMAT_ERROR 4

// odbieranie stan pakietu
#define FB_PACK_INIT 0
#define FB_PACK_INCOMPLETED 1
#define FB_PACK_COMPLETED 2

#endif /* FB_CONST_H_ */
