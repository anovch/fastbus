/*
 * fb_server.c
 *
 *  Created on: 25-09-2014
 *      Author: an
 */
#include "fb_master.h"
#include "fb_session.h"
#include "fb_application.h"

#define MAXSLAVESDEVICE 8

#define MASTER_STATE_NEXT_DEVICE 0

struct Fb_Master_T{
	Fb_Application_T slaves[MAXSLAVESDEVICE];
//	Fb_Session_T sess;
	uint8_t state;
};


Fb_Master_T fb_master_new(Fb_Mem_Manager_inh_T* memman) {

	Fb_Master_T result;
	if ((result = memman->malloc(sizeof(struct Fb_Master_T)))) {
		for(int i=0;i<MAXSLAVESDEVICE;i++) {
			result->slaves[i]=0;
		}
	}
	return result;
}


 uint8_t fb_master_set_application(Fb_Master_T master, Fb_Application_T slave) {
	 if(!master)
		 return 0;
	 for(int i=0;i<MAXSLAVESDEVICE;i++) {
		 if (master->slaves[i]==0) {
			 master->slaves[i]= slave;
			 return 1;
		 }
	 }
	 return 0;
}


 /*
  * Timeout na odebranie danych
  */
 void fb_master_read_timeout(Fb_Master_T master) {

 }

 /*
  * Odebranie danych z magitrali (transportu)
  */
 void fb_master_read_from_transport(Fb_Master_T master, uint8_t value) {

}

 /*
  * Wys³anie danych na magitrali (transportu)
  */
uint8_t* fb_master_write_to_transport(Fb_Master_T master, uint16_t* size) {
	if (!master)
		return 0;


	if (master->state == MASTER_STATE_NEXT_DEVICE) {

	}

	return 0;
}
