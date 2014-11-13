#include "hbus/hbus_events.h"

//Connect to events here

const void (*HBUSEVT_PACKET_RECEIVED)(byte *, byte) = 0;
const void (*HBUSEVT_PACKET_SENT)(byte *,byte,word) = 0;
const void (*HBUSEVT_BUSLOCKED)(void) = 0;
const void (*HBUSEVT_BUSUNLOCKED)(void) = 0;
const void (*HBUSEVT_ADDR_START)(void) = 0;
const void (*HBUSEVT_ADDR_END)(void) = 0;
const byte (*HBUSEVT_RESET)(void) = 0;
