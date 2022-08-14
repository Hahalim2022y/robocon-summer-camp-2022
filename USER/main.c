#include <stm32f10x.h>
#include "LED.h"
#include "grayScaleSensor.h"
#include "gyro.h"
#include <string.h>
#include "uart.h"
#include "can.h"
#include "motor3508.h"
#include "key.h"
#include "chassis.h"
#include "cycleArray.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	u8 res;
	char command[100];
	u8 commandCnt = 0;
	u8 commandFinish = 0;
	can_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_3tq, 6, CAN_Mode_Normal);
	//KEY_Init();
	//usart2Init(115200);
	//u32 time = 0;
	motorInit();
	//LEDInit();
	//chassisInit();
	//grayScaleSensor2_Init();
	
	usart1Init(115200);
	uart4Init(115200);
	
	LED0(1);
	LED1(1);
	
	while(1)
	{
		//uart1_send("123");
		//uart4_send("123");
		while(1)
		{
			if(uart4_read(&res) == 1)
			{
				if(res != '\n')
				{
					command[commandCnt] = res;
				}
				else
				{
					command[commandCnt] = '\0';
					commandFinish = 1;
					commandCnt = 0;
					break;
				}
			}
			else
			{
				break;
			}
			commandCnt++;
		}
		if(commandFinish == 1)
		{
			if(strcmp(command, "W") == 0)
			{
				motorSetTargetRpm(&motor[1], 60);
				motorSetTargetRpm(&motor[2], 60);
				motorSetTargetRpm(&motor[0], 60);
			}
			else if(strcmp(command, "S") == 0)
			{
				motorSetTargetRpm(&motor[1], -60);
				motorSetTargetRpm(&motor[2], -60);
				motorSetTargetRpm(&motor[0], -60);
			}
			else
			{
				motorSetTargetRpm(&motor[1], 0);
				motorSetTargetRpm(&motor[2], 0);
				motorSetTargetRpm(&motor[0], 0);
			}
			uart4_send(command);
			uart4_send("\n");
			commandFinish = 0;
		}
	}
}
