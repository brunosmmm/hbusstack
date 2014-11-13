
#ifndef _HBUS_INTERRUPT_H_INCLUDED
#define _HBUS_INTERRUPT_H_INCLUDED

#include "setup.h"

#define HBUS_INT_IDLE		0x00
#define HBUS_INT_REQ_THIS	0x01
#define HBUS_INT_REQ_OTHER	0x02
#define HBUS_INT_WAIT		0x03
#define HBUS_INT_LOCK		0x04
#define HBUS_INT_FREE		0x05

#define INTERRUPT_POLLED_MODE 1
//#define INTERRUPT_IRQ_MODE	1

void INTERRUPT_CYCLE(void);

#ifdef HBUS_INTERRUPT_SUPPORT
void INTERRUPT_EMIT(byte intNumber);
void INTERRUPT_RELEASE(void);
#endif

#endif