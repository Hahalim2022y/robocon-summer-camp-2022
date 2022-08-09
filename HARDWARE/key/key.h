#ifndef KEY_H
#define KEY_H

#include <stm32f10x.h>

#define KEY0 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define KEY1 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define WK_UP GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)

#define KEY0_PRES 1
#define KEY1_PRES 2
#define WKUP_PRES 3

void KEY_Init(void);
u8 KEY_Scan(u8 mode);

#endif
