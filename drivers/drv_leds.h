#ifndef _DRV_LEDS_H_INCLUDED
#define _DRV_LEDS_H_INCLUDED

#include "setup.h"

//DEFINE LEDS HERE

#define LEDDRV_LED0 LED0
#define LEDDRV_LED1 LED1
#define LEDDRV_LED2 LED2
#define LEDDRV_LED3 LED3

#define LEDDRV_LED0_ON		0x01
#define LEDDRV_LED0_BLINK	0x02
#define LEDDRV_LED1_ON		0x04
#define LEDDRV_LED1_BLINK	0x08
#define LEDDRV_LED2_ON		0x10
#define LEDDRV_LED2_BLINK	0x20
#define LEDDRV_LED3_ON		0x40
#define LEDDRV_LED3_BLINK	0x80


#define LED_PULSE_INTERVAL 50
#define LED_BLINK_INTERVAL 500

void LEDDRV_CYCLE(void);
void LEDDRV_INIT(void);
void LEDDRV_SETLEDS(byte b);
void LEDDRV_PULSE(byte leds);
byte LEDDRV_GETLEDS(void);

#endif