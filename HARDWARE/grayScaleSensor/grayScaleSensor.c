#include "grayScaleSensor.h"

void grayScaleSensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);												   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);												   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);												   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);												   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

u8 grayScaleSensor_Read(void)
{
	u8 res = 0x00;
	res += GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_15) << 0;
	res += GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_1) << 1;
	res += GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8) << 2;
	res += GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10) << 3;
	res += GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12) << 4;
	res += GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14) << 5;
	res += GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_10) << 6;
	
	return res;
}
