#ifndef _HBUS_EVENTS_H_INCLUDED
#define _HBUS_EVENTS_H_INCLUDED

#include "setup.h"

//HBUS event system
//Function pointers to callee when event is triggered

extern const void (*HBUSEVT_PACKET_RECEIVED)(byte *,byte);  //New packet reveiced
extern const void (*HBUSEVT_PACKET_SENT)(byte *,byte,word); //Packet sent
extern const void (*HBUSEVT_BUSLOCKED)(void);               //Buslock 
extern const void (*HBUSEVT_BUSUNLOCKED)(void);             //Busunlock
extern const void (*HBUSEVT_ADDR_START)(void);              //Addressing start
extern const void (*HBUSEVT_ADDR_END)(void);                //Addressing finish
extern const byte (*HBUSEVT_RESET)(void);                   //Soft reset


#endif
