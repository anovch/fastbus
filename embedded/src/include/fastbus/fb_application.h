/*
 * fb_application.h
 *
 *  Created on: 25-09-2014
 *      Author: an
 */

#ifndef FB_APPLICATION_H_
#define FB_APPLICATION_H_

#include <inttypes.h>
#include <stdlib.h>

typedef struct Fb_Application_T* Fb_Application_T;


typedef struct
{
	uint8_t (*get_transmition_state)(Fb_Application_T app);
	void (*set_transmition_state)(Fb_Application_T app, uint8_t state);
	uint8_t (*get_address)(Fb_Application_T app);
	uint8_t* (*get_packet)(Fb_Application_T app, uint32_t* size);
	uint8_t (*set_packet)(Fb_Application_T app, uint8_t* data, uint32_t size);
} Comm_Application_inh_T;



#endif /* FB_APPLICATION_H_ */
