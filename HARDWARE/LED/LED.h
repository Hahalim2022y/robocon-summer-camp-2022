#ifndef LED_H
#define LED_H

#include <stm32f10x.h>

void LEDInit(void);
void LED0(u8 bit);
void LED1(u8 bit);

//#define LED0 PEout(5)// PB5
//#define LED1 PEout(6)// PE5	

#endif
