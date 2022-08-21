#ifndef GRAY_SCALE_SENSOR_H
#define GRAY_SCALE_SENSOR_H

#include <stm32f10x.h>
#include "uart.h"

#define L1 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)
#define L2 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define L3 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define L4 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)
#define L5 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6)
#define M GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)

#define R5 GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_1)
#define R4 GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_2)
#define R3 GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_3)
#define R2 GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_4)
#define R1 GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_5)


void grayScaleSensor_Init(void);
u8 grayScaleSensor_Read(void);

void grayScaleSensor2_Init(void);
u16 grayScaleSensor2_Read(void);

#endif
