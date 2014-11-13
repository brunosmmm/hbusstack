/// @file drv_leds.c
/// @brief Driver for LEDS including blinking patterns
/// @author Bruno Morais <brunosmmm@gmail.com>
#include "drivers/drv_leds.h"

static byte LEDCTL = 0x00;
static dword LEDCTR = 0;
extern dword TIMECOUNTER;
static byte LEDPULSE = 0;
static dword PULSECOUNTER = 0;

void LEDDRV_CYCLE(void)
{
#ifdef LEDDRV_LED0
	if (LEDCTL & LEDDRV_LED0_ON)
	{
		if (!(LEDCTL & LEDDRV_LED0_BLINK)) LEDDRV_LED0 = 1;
	}
	else LEDDRV_LED0 = 0;
#endif
	
#ifdef LEDDRV_LED1
	if (LEDCTL & LEDDRV_LED1_ON)
	{
		if (!(LEDCTL & LEDDRV_LED1_BLINK)) LEDDRV_LED1 = 1;
	}
	else LEDDRV_LED1 = 0;
#endif
	
#ifdef LEDDRV_LED2
	if (LEDCTL & LEDDRV_LED2_ON)
	{
		if (!(LEDCTL & LEDDRV_LED2_BLINK)) LEDDRV_LED2 = 1;
	}
	else LEDDRV_LED2 = 0;
#endif

#ifdef LEDDRV_LED3
	if (LEDCTL & LEDDRV_LED3_ON)
	{
		if (!(LEDCTL & LEDDRV_LED3_BLINK)) LEDDRV_LED3 = 1;
	}
	else LEDDRV_LED3 = 0;
#endif
	
	if (TIMECOUNTER > (LEDCTR + LED_BLINK_INTERVAL))
	{
#ifdef LEDDRV_LED0
		if ((LEDCTL & LEDDRV_LED0_BLINK) && (LEDCTL & LEDDRV_LED0_ON)) LEDDRV_LED0 ^= 1;
#endif
		
#ifdef LEDDRV_LED1
		if ((LEDCTL & LEDDRV_LED1_BLINK) && (LEDCTL & LEDDRV_LED1_ON)) LEDDRV_LED1 ^= 1;
#endif
		
#ifdef LEDDRV_LED2
		if ((LEDCTL & LEDDRV_LED2_BLINK) && (LEDCTL & LEDDRV_LED2_ON)) LEDDRV_LED2 ^= 1;
#endif
		
#ifdef LEDDRV_LED3
		if ((LEDCTL & LEDDRV_LED3_BLINK) && (LEDCTL & LEDDRV_LED3_ON)) LEDDRV_LED3 ^= 1;
#endif
		
		LEDCTR = TIMECOUNTER;
		
	}
	
	if ((TIMECOUNTER > (PULSECOUNTER + LED_PULSE_INTERVAL)) && (LEDPULSE))
	{
		
		LEDCTL &= ~LEDPULSE;
		LEDPULSE = 0;
		
	}
	
}

void LEDDRV_INIT(void)
{
	LEDPULSE = 0x00;
	LEDCTL = 0x00;
	LEDCTR = 0;
	
}

void LEDDRV_SETLEDS(byte b)
{
	
	LEDCTL = b;
	
}

void LEDDRV_PULSE(byte leds)
{
	
	PULSECOUNTER = TIMECOUNTER;
	
	if ((leds & LEDDRV_LED0_ON) && (!(LEDCTL & LEDDRV_LED0_ON)))
	{
		
		LEDPULSE |= LEDDRV_LED0_ON;
		LEDCTL |= LEDDRV_LED0_ON;
		
	}
	
	if ((leds & LEDDRV_LED1_ON) && (!(LEDCTL & LEDDRV_LED1_ON)))
	{
		
		LEDPULSE |= LEDDRV_LED1_ON;
		LEDCTL |= LEDDRV_LED1_ON;
		
	}
	
	if ((leds & LEDDRV_LED2_ON) && (!(LEDCTL & LEDDRV_LED2_ON)))
	{
		
		LEDPULSE |= LEDDRV_LED2_ON;
		LEDCTL |= LEDDRV_LED2_ON;
		
	}
	
	if ((leds & LEDDRV_LED3_ON) && (!(LEDCTL & LEDDRV_LED3_ON)))
	{
		
		LEDPULSE |= LEDDRV_LED3_ON;
		LEDCTL |= LEDDRV_LED3_ON;
		
	}
	
}

byte LEDDRV_GETLEDS(void)
{
	
	return LEDCTL;	
	
}
