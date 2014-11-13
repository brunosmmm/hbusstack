#include "hbus/hbus_interrupt.h"

byte HBUS_INTERRUPT_STATUS = HBUS_INT_IDLE;

#ifdef HBUS_INTERRUPT_SUPPORT
byte HBUS_INTERRUPT_ACTIVE = 0;

void INTERRUPT_END(void);
#endif

void INTERRUPT_POLL(void)
{
#ifdef HBUS_INTERRUPT_SUPPORT
	if (FREEBUS_DIR)
	{
#endif
		
		if (!FREEBUS) HBUS_INTERRUPT_STATUS = HBUS_INT_REQ_OTHER;
		else HBUS_INTERRUPT_STATUS = HBUS_INT_IDLE;
#ifdef HBUS_INTERRUPT_SUPPORT
	}
	else
	{
		
		if (HBUS_INTERRUPT_STATUS == HBUS_INT_LOCK) INTERRUPT_RELEASE(); //releases freebus
		else if (HBUS_INTERRUPT_STATUS == HBUS_INT_FREE) INTERRUPT_END();
		
	}
#endif
	
}

void INTERRUPT_CYCLE(void)
{
	
#ifdef INTERRUPT_POLLED_MODE
	INTERRUPT_POLL();
#endif
		
}

#ifdef INTERRUPT_IRQ_MODE
void INTERRUPT_EVENT(void)
{
	
	//falta
}
#endif

#ifdef HBUS_INTERRUPT_SUPPORT
void INTERRUPT_EMIT(byte intNumber)
{
	
	if (HBUS_INTERRUPT_STATUS) return;
	
	FREEBUS_DIR = 0;
	FREEBUS = 0;
	
	HBUS_INTERRUPT_ACTIVE = intNumber;
	
	HBUS_INTERRUPT_STATUS = HBUS_INT_REQ_THIS;
	
}

void INTERRUPT_RELEASE(void)
{
	
	FREEBUS_DIR = 1;
	FREEBUS = 1;
	
}

void INTERRUPT_END(void)
{
	
	HBUS_INTERRUPT_ACTIVE = 0;
	
	HBUS_INTERRUPT_STATUS = HBUS_INT_IDLE;
	
}
#endif
