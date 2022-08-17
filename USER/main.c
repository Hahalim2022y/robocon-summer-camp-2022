#include <stm32f10x.h>
#include "LED.h"
#include "grayScaleSensor.h"
#include "gyro.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "can.h"
#include "motor3508.h"
#include "key.h"
#include "chassis.h"
#include "cycleArray.h"
#include "linetracker.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	u8 res;
	char command[100];
	char buff[100];
	u8 commandCnt = 0;
	u8 commandFinish = 0;
	u16 grayScaleSensor_res = grayScaleSensor2_Read();
	can_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_3tq, 6, CAN_Mode_Normal);
	//KEY_Init();
	usart2Init(115200);
	//u32 time = 0;
	motorInit();
	//LEDInit();
	chassisInit();
	grayScaleSensor2_Init();
	angle_match_init();
	
	
	usart1Init(115200);
	uart4Init(115200);
	
	LED0(0);
	LED1(0);
	
	float speed = 0;
	float angle = 0;
	
	u8 linetracker_switch=0;
	
	u16 i;
	
	for(i = 0; i < 10000; i++)
	{
		chassis.angleBias = attitude.yaw;
		chassis.angle = 0;
		chassis.lastAngle = 361;
		chassis.numOfTurns = 0;
	}
	
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
				chassisSetState(0, speed, angle);
			}
			else if(strcmp(command, "S") == 0)
			{
				chassisSetState(0, -speed, angle);
			}
			else if(strcmp(command, "A") == 0)
			{
				chassisSetState(-speed, 0, angle);
			}
			else if(strcmp(command, "D") == 0)
			{
				chassisSetState(speed, 0, angle);
			}
			else if(strcmp(command, "Q") == 0)
			{
				linetracker_switch = !linetracker_switch;
			}
			else if(strcmp(command, "gray") == 0)
			{
				grayScaleSensor_res = grayScaleSensor2_Read();
				for(i = 0; i < 11; i++)
				{
					if((grayScaleSensor_res & (1 << i)) != 0)
					{
						uart4_send("1");
					}
					else
					{
						uart4_send("0");
					}
				}	
				uart4_send("\n");
			}
			else if(strcmp(command, "reset") == 0)
			{
				chassis.angleBias = attitude.yaw;
				chassis.angle = 0;
				chassis.lastAngle = 361;
				chassis.numOfTurns = 0;
				chassis.targetAngle =0;
				chassis.world_x = 0;
				chassis.world_y = 0;
			}
			else if(strstr(command, "speed") != NULL)
			{
				speed = atof(command + 5);
			}
			else if(strcmp(command, "angle") == 0)
			{
				sprintf(buff, "%f\n", chassis.angle + chassis.numOfTurns * 360);
				uart4_send(buff);
			} 
			else if(strstr(command, "angle") != NULL)
			{
				angle = atof(command + 5);
				chassisSetState(0, 0, angle);
			}
			else if(strcmp(command, "xy") == 0)
			{
				sprintf(buff, "%f , %f\n", chassis.world_x * 1.0175 + 114.55, chassis.world_y * 1.0175 + 114.55);
				uart4_send(buff);
			}
			else
			{
				chassisSetState(0, 0, chassis.angle);
				linetracker_switch = 0;
			}
			
			uart4_send(command);
			uart4_send("\n");
			commandFinish = 0;
		}
		if(linetracker_switch == 1) 
		{
			correspond(speed);
		}
		
			
	}
}
