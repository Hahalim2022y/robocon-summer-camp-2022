#include <stm32f10x.h>
#include "LED.h"
#include "grayScaleSensor.h"
#include "gyro.h"
#include <stdio.h>
#include "uart.h"
#include "can.h"
#include "motor3508.h"
#include "key.h"
#include "chassis.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	char buff[100];
	can_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_3tq, 6, CAN_Mode_Normal);
	usart1Init(115200);
	KEY_Init();
	usart2Init(115200);
	u32 time = 0;
	motorInit();
	LEDInit();
	chassisInit();
	
	LED0(1);
	LED1(1);
	
	while(1)
	{
		time++;
		if(KEY0 == 0)
		{
			LED0(0);
			LED1(1);
			//motorSetTargetRpm(&motor[0], 250);
			motorSetTargetRpm(&motor[0], 0);
			motorSetTargetRpm(&motor[1], 0);
			motorSetTargetRpm(&motor[2], 0);
		}
		if(KEY1 == 0)
		{
			LED0(1);
			LED1(0);
			motorSetTargetRpm(&motor[0], 60);
			motorSetTargetRpm(&motor[1], 60);
			motorSetTargetRpm(&motor[2], 60);
		}
		if(WKUP_PRES == 0)
		{
			//motorSetTargetRpm(&motor[2], 250);
		}
		//uart1_send("???");
		uart1_send("???");
		if(time % 80000 == 0)
		{	
			//sprintf(buff, "%f\r\n", chassis.angle + 360.0 * chassis.numOfTurns);
			//uart1_send(buff);
		}
		
		//grayScaleSensor_Send();  //灰度传感器发送数据
	}
}
