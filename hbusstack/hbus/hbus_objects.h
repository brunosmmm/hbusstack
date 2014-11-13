#ifndef _HBUS_OBJECTS_H_INCLUDED
#define _HBUS_OBJECTS_H_INCLUDED

#include "setup.h"

#define HBUSOBJ_READ  		0x01
#define HBUSOBJ_WRITE 		0x02
#define HBUSOBJ_CRYPTO 		0x04
#define HBUSOBJ_HIDDEN		0x08
#define HBUSOBJ_DTYPE_INT	0x00
#define HBUSOBJ_DTYPE_UINT	0x10
#define HBUSOBJ_DTYPE_FIXP	0x20
#define HBUSOBJ_DTYPE_BYTE	0x30

#define HBUSOBJ_LEVEL_0		0x00
#define HBUSOBJ_LEVEL_1		0x40
#define HBUSOBJ_LEVEL_2		0x80
#define HBUSOBJ_LEVEL_3		0xC0

#define HBUSOBJ_DTYPE_BYTE_BASE_HEX		0x01
#define HBUSOBJ_DTYPE_BYTE_BASE_DEC		0x02
#define HBUSOBJ_DTYPE_BYTE_BASE_OCT		0x03

#define HBUSOBJ_DTYPE_UINT_PERCENT		0x04
#define HBUSOBJ_DTYPE_UINT_LIN_PERCENT	0x05
#define HBUSOBJ_DTYPE_UINT_LOG_PERCENT	0x06


#define HBUSEP_READ 	0x01
#define HBUSEP_WRITE 	0x02
#define HBUSEP_CRYPTO	0x04

#define HBUS_OBJECT_COUNT 6
#define HBUS_EP_COUNT	0
#define HBUS_INT_COUNT	1

#ifdef HBUS_CRYPTO_SUPPORT
#define HBUS_CRYPT_CAPABLE	0x01
#else
#define HBUS_CRYPT_CAPABLE 0x00
#endif

#ifdef HBUS_ENDPOINT_SUPPORT
#define HBUS_EP_CAPABLE		0x02
#else
#define HBUS_EP_CAPABLE 	0x00
#endif

#ifdef HBUS_INTERRUPT_SUPPORT
#define HBUS_INT_CAPABLE	0x04
#else
#define HBUS_INT_CAPABLE	0x00
#endif

#ifdef HBUS_AUTH_SUPPORT
#define HBUS_AUTH_CAPABLE	0x08
#else
#define HBUS_AUTH_CAPABLE	0x00
#endif

#ifdef HBUS_UCODE_SUPPORT
#define HBUS_UCODE_CAPABLE 	0x10
#else
#define HBUS_UCODE_CAPABLE	0x00
#endif

#define HBUS_CAPABILITIES	(HBUS_UCODE_CAPABLE|HBUS_AUTH_CAPABLE|HBUS_CRYPT_CAPABLE|HBUS_EP_CAPABLE|HBUS_INT_CAPABLE)

typedef struct HBUSOBJECTLISTINFOS
{
	byte objectListSize;
	byte endpointListSize;
	byte interruptListSize;
	
	byte slaveCapabilities;
	
	dword uniqueDeviceInfo;
	
} HBUSOBJLISTINFO;

typedef struct HBUSOBJECTINFOS
{
	byte objectFlags;
	byte objectSize;
	
	byte objectDataTypeInfo;
	
	byte objectDescStringSize;
	byte * objectDescString;
	
} HBUSOBJINFO;

typedef struct HBUSOBJECTS
{
	HBUSOBJINFO objectInfo;
	
	byte * objectData;
	void * (*getObjectData)(void); //compulsory even with write only objects
	void (*setObjectData)(void *, int);
	
} HBUSOBJ;

typedef struct HBUSENDPOINTINFOS
{
	byte endpointFlags;
	byte endpointBlockSize;
	
	byte endpointDescStringSize;
	byte * endpointDescString;
	
} HBUSEPINFO;

typedef struct HBUSENDPOINTS
{
	HBUSEPINFO endpointInfo;
	
	byte * (*readEPByte)(void);
	byte (*writeEPByte)(byte);
	
	void (*EPwriteBegin)(void);
	void (*EPwriteEnd)(void);
	
	void (*EPreadBegin)(void);
	void (*EPreadEnd)(void);
	
} HBUSEP;

typedef struct HBUSINTERRUPTINFOS
{
	byte interruptFlags;
	
	byte interruptDescStringSize;
	byte * interruptDescString;
	
} HBUSINTINFO;

typedef struct HBUSINTERRUPTS
{
	
	HBUSINTINFO interruptInfo;
	
	byte interruptNumber;
	
} HBUSINT;

extern const HBUSOBJ * MCU_OBJECTS[HBUS_OBJECT_COUNT];
#ifdef HBUS_ENDPOINT_SUPPORT
extern const HBUSEP  * MCU_ENDPOINTS[HBUS_EP_COUNT];
#endif

#ifdef HBUS_INTERRUPT_SUPPORT
extern const HBUSINT * MCU_INTERRUPTS[HBUS_INT_COUNT];
#endif


#endif
