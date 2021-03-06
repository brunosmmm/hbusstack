#include "hbus/hbus_objects.h"

//DESCRIPTOR UID:
//BYTE 0:2 = SERIAL NUM
//BYTE 3   = FAMILY ID

#define S_FAMILY 0x00000000
#define S_NUM    0x00000000

#pragma MESSAGE DISABLE C4800

const HBUSOBJLISTINFO OBJECT_INFO = {HBUS_OBJECT_COUNT, HBUS_EP_COUNT, HBUS_INT_COUNT, HBUS_CAPABILITIES, ((dword)(S_FAMILY) | (dword)(S_NUM))};
const HBUSOBJ UNIT_INFO = {{HBUSOBJ_READ,sizeof(HBUSOBJLISTINFO),0,11,"HBUS Device"},(byte*)&OBJECT_INFO,0x00,0x00};

extern byte UCODE_WRITE_IMEM_BYTE(byte b);
extern void UCODE_IMEM_WRITE_START(void);
extern void UCODE_IMEM_WRITE_END(void);

const HBUSOBJ OBJ_TEMPL  = {{HBUSOBJ_READ|HBUSOBJ_DTYPE_UINT|HBUSOBJ_LEVEL_0,1,0,6,"Object"},0x00,READ_OBJ,WRITE_OBJ};

//Invisible objects template
const char INVIS_UNIT = 'd';
const HBUSOBJ OBJ_INVIS_UNIT = {{HBUSOBJ_READ|HBUSOBJ_HIDDEN,1,0,6,"1:UNIT"},0x00,0x00,0x00};

//INTERRUPTS
const HBUSINT INT_FATAL = {{0x00,5,"FATAL"},0x00};	//FATAL ERROR

//ENDPOINTS
//const HBUSEP EP_UCODE = {{HBUSEP_WRITE,64,9,"UCODE EP"},0x00,UCODE_WRITE_IMEM_BYTE,UCODE_IMEM_WRITE_START,UCODE_IMEM_WRITE_END,0x00,0x00};


const HBUSOBJ * MCU_OBJECTS[HBUS_OBJECT_COUNT] = {&UNIT_INFO, //compulsory
												  &OBJ_TEMPL
												  &OBJ_INVIS_UNIT

};

#if HBUS_EP_COUNT
const HBUSEP * MCU_ENDPOINTS[HBUS_EP_COUNT] = {&EP_UCODE,
		
};
#endif

#if HBUS_INT_COUNT
const HBUSINT * MCU_INTERRUPTS[HBUS_INT_COUNT] = {&INT_FATAL,
		
};
#endif
