#include "grayScaleSensor.h"

//第一个灰度传感器，夏令营物资
void grayScaleSensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);												   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);												   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);												   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	

}

u8 grayScaleSensor_Read(void)
{
	u8 res = 0x00;
	res += (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_13) << 6);
	res += (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_15) << 5);
	res += (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_1) << 4);
	res += (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8) << 3);
	res += (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10) << 2);
	res += (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12) << 1);
	res += (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14) << 0);
	
	return res;
}

void grayScaleSensor_Send(void)
{
	u8 gray;
	u16 i;
	gray = grayScaleSensor_Read();
	for(i = 0; i < 7; i++)
	{
		if(gray & (1 << i))
		{
			uart1_send("1 ");
		}
		else
		{
			uart1_send("0 ");
		}
	}
	uart1_send("\r\n");

}

//第二个灰度传感器，智能车队
void grayScaleSensor2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);												   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);												   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);												   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	

}

u16 grayScaleSensor2_Read(void)
{
	u16 res = 0x00;
//	res += (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_4) << 5);
//	res += (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_2) << 4);
//	res += (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) << 3);
//	res += (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) << 2);
//	res += (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) << 1);
//	res += (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) << 0);
	res += (L1 << 0);
	res += (L2 << 1);
	res += (L3 << 2);
	res += (L4 << 3);
	res += (L5 << 4);
	res += (M << 5);
	res += (R5 << 6);
	res += (R4 << 7);
	res += (R3 << 8);
	res += (R2 << 9);
	res += (R1 << 10);

	
	return res;
}
u8 grayScaleSensor2_Read_down(void)
{
	u8 res = 0x00;
	res += (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_5) << 5);
	res += (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_3) << 4);
	res += (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_1) << 3);
	res += (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) << 2);
	res += (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) << 1);
	res += (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) << 0);

	
	return res;
}

void grayScaleSensor2_Send(void)
{
	u8 gray_up,gray_down;
	u16 i;
	gray_up = grayScaleSensor2_Read_up();
			for(i = 0; i < 6; i++)
			{
				if(gray_up & (1 << (5-i)))
				{
					uart1_send("1 ");
					
				}
				else
				{
					uart1_send("0 ");
				}
			}
			uart1_send("\r\n");
			gray_down = grayScaleSensor2_Read_down();
			for(i = 0; i < 6; i++)
			{
				if(gray_down & (1 << (5-i)))
				{
					uart1_send("1 ");
				}
				else
				{
					uart1_send("0 ");
				}
			}
			uart1_send("\r\n");
			uart1_send("\r\n");

}

