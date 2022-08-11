#ifndef GRAY_SCALE_SENSOR_H
#define GRAY_SCALE_SENSOR_H

#include <stm32f10x.h>
#include "uart.h"

void grayScaleSensor_Init(void);
u8 grayScaleSensor_Read(void);
void grayScaleSensor_Send(void);

void grayScaleSensor2_Init(void);
u8 grayScaleSensor2_Read_up(void);
u8 grayScaleSensor2_Read_down(void);
void grayScaleSensor2_Send(void);

#endif
