/*
 * fb_mem_manager.h
 *
 *  Created on: 20 wrz 2014
 *      Author: an
 */

#ifndef FB_MEM_MANAGER_H_
#define FB_MEM_MANAGER_H_
#include <stdlib.h>

typedef struct
{
	void* (*malloc)(size_t size);
	void(*free)(void*);

} Fb_Mem_Manager_inh_T;



#endif /* FB_MEM_MANAGER_H_ */
